#include "include/storage_engine/buffer/buffer_pool.h"

using namespace common;
using namespace std;

static const int MEM_POOL_ITEM_NUM = 20;


FileBufferPool::FileBufferPool(BufferPoolManager &bp_manager, FrameManager &frame_manager)
    : bp_manager_(bp_manager), frame_manager_(frame_manager)
{}

FileBufferPool::~FileBufferPool()
{
  close_file();
  LOG_INFO("disk buffer pool exit");
}

/**
 * @brief 打开文件
 * 1. 打开指定文件，如果文件不存在，则创建一个新的文件
 * 2. 分配一个帧用于存储文件头
 * 3. 加载文件头
 */
RC FileBufferPool::open_file(const char *file_name)
{
  int fd = open(file_name, O_RDWR);
  if (fd < 0) {
    LOG_ERROR("Failed to open file %s, because %s.", file_name, strerror(errno));
    return RC::IOERR_ACCESS;
  }
  LOG_INFO("Successfully open buffer pool file %s.", file_name);

  file_name_ = file_name;
  file_desc_ = fd;

  RC rc = RC::SUCCESS;
  rc = allocate_frame(BP_HEADER_PAGE, &hdr_frame_);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("failed to allocate frame for header. file name %s", file_name_.c_str());
    close(fd);
    file_desc_ = -1;
    return rc;
  }

  hdr_frame_->set_file_desc(fd);
  hdr_frame_->access();

  if ((rc = load_page(BP_HEADER_PAGE, hdr_frame_)) != RC::SUCCESS) {
    LOG_ERROR("Failed to load first page of %s, due to %s.", file_name, strerror(errno));
    evict_page(BP_HEADER_PAGE, hdr_frame_);
    close(fd);
    file_desc_ = -1;
    return rc;
  }

  file_header_ = (FileHeader *)hdr_frame_->data();

  LOG_INFO("Successfully open %s. file_desc=%d, hdr_frame=%p, file header=%s",
           file_name, file_desc_, hdr_frame_, file_header_->to_string().c_str());
  return RC::SUCCESS;
}

/**
 * @brief 关闭文件
 * 1. 清理所有的frame
 * 2. 关闭文件
 */
RC FileBufferPool::close_file()
{
  RC rc = RC::SUCCESS;
  if (file_desc_ < 0) {
    return rc;
  }

  hdr_frame_->unpin();

  rc = evict_all_pages();
  if (rc != RC::SUCCESS) {
    LOG_ERROR("failed to close %s, due to failed to purge pages. rc=%s", file_name_.c_str(), strrc(rc));
    return rc;
  }

  disposed_pages_.clear();

  if (close(file_desc_) < 0) {
    LOG_ERROR("Failed to close fileId:%d, fileName:%s, error:%s", file_desc_, file_name_.c_str(), strerror(errno));
    return RC::IOERR_CLOSE;
  }
  LOG_INFO("Successfully close file %d:%s.", file_desc_, file_name_.c_str());
  file_desc_ = -1;

  bp_manager_.close_file(file_name_.c_str());
  return RC::SUCCESS;
}

/**
 * @brief 用于获取指定页面编号 page_num 对应的帧 Frame
 * 如果帧已在缓冲池中，它将直接返回该帧；如果不在，则会分配一个新的帧并从磁盘加载页面数据。
 */
RC FileBufferPool::get_this_page(PageNum page_num, Frame **frame)
{
  RC rc = RC::SUCCESS;
  *frame = nullptr;

  Frame *used_match_frame = frame_manager_.get(file_desc_, page_num);
  if (used_match_frame != nullptr) {
    used_match_frame->access();
    *frame = used_match_frame;
    return RC::SUCCESS;
  }

  std::scoped_lock lock_guard(lock_); // 直接加了一把大锁，其实可以根据访问的页面来细化提高并行度

  // Allocate one page and load the data into this page
  Frame *allocated_frame = nullptr;
  rc = allocate_frame(page_num, &allocated_frame);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to alloc frame %s:%d, due to failed to alloc page.", file_name_.c_str(), page_num);
    return rc;
  }

  allocated_frame->set_file_desc(file_desc_);
  // allocated_frame->pin(); // pined in manager::get
  allocated_frame->access();

  if ((rc = load_page(page_num, allocated_frame)) != RC::SUCCESS) {
    LOG_ERROR("Failed to load page %s:%d", file_name_.c_str(), page_num);
    evict_page(page_num, allocated_frame);
    return rc;
  }

  *frame = allocated_frame;
  return RC::SUCCESS;
}

RC FileBufferPool::allocate_page(Frame **frame)
{
  RC rc = RC::SUCCESS;

  lock_.lock();

  int byte = 0, bit = 0;
  if ((file_header_->allocated_pages) < (file_header_->page_count)) {
    // There is one free page
    for (int i = 0; i < file_header_->page_count; i++) {
      byte = i / 8;
      bit = i % 8;
      if (((file_header_->bitmap[byte]) & (1 << bit)) == 0) {
        (file_header_->allocated_pages)++;
        file_header_->bitmap[byte] |= (1 << bit);
        hdr_frame_->mark_dirty();

        lock_.unlock();
        return get_this_page(i, frame);
      }
    }
  }

  if (file_header_->page_count >= FileHeader::MAX_PAGE_NUM) {
    LOG_WARN("file buffer pool is full. page count %d, max page count %d",
        file_header_->page_count, FileHeader::MAX_PAGE_NUM);
    lock_.unlock();
    return RC::BUFFERPOOL_NOBUF;
  }

  PageNum page_num = file_header_->page_count;
  Frame *allocated_frame = nullptr;
  if ((rc = allocate_frame(page_num, &allocated_frame)) != RC::SUCCESS) {
    LOG_ERROR("Failed to allocate frame %s, due to no free page.", file_name_.c_str());
    lock_.unlock();
    return rc;
  }

  LOG_INFO("allocate new page. file=%s, pageNum=%d, pin=%d",
           file_name_.c_str(), page_num, allocated_frame->pin_count());

  file_header_->allocated_pages++;
  file_header_->page_count++;

  byte = page_num / 8;
  bit = page_num % 8;
  file_header_->bitmap[byte] |= (1 << bit);
  hdr_frame_->mark_dirty();

  allocated_frame->set_file_desc(file_desc_);
  allocated_frame->access();
  allocated_frame->clear_page();
  allocated_frame->set_page_num(file_header_->page_count - 1);

  lock_.unlock();

  *frame = allocated_frame;
  return RC::SUCCESS;
}

RC FileBufferPool::unpin_page(Frame *frame)
{
  frame->unpin();
  return RC::SUCCESS;
}

/**
 * TODO [Lab1] 需要同学们实现页面刷盘，下面是可参考的思路
 */
RC FileBufferPool::flush_page(Frame &frame)
{
  std::scoped_lock lock_guard(lock_);
  return flush_page_internal(frame);
}
/**
 * TODO [Lab1] 需要同学们实现页面刷盘，下面是可参考的思路
 */
RC FileBufferPool::flush_page_internal(Frame &frame)
{
//  1. 获取页面Page
//  2. 计算该Page在文件中的偏移量
//  3. 写入数据到文件的目标位置
//  4. 清除frame的脏标记
//  5. 记录和返回成功
  return RC::SUCCESS;
}

/**
 * 将所有的脏页刷盘
 */
RC FileBufferPool::flush_all_pages()
{
  std::scoped_lock lock_guard(lock_);
  RC rc = RC::SUCCESS;
  for (Frame *frame : frame_manager_.find_list(file_desc_)) {
    if (frame->dirty()) {
      RC _rc = flush_page_internal(*frame);
      if (_rc != RC::SUCCESS) {
        LOG_ERROR("Failed to flush page %s:%d, rc=%s", file_name_.c_str(), frame->page_num(), strrc(_rc));
        rc = _rc;
      }
    }
    frame->unpin();
  }
  return rc;
}

/**
 * TODO [Lab1] 需要同学们实现某个指定页面的驱逐
 */
RC FileBufferPool::evict_page(PageNum page_num, Frame *buf)
{
  return RC::SUCCESS;
}
/**
 * TODO [Lab1] 需要同学们实现该文件所有页面的驱逐
 */
RC FileBufferPool::evict_all_pages()
{
  return RC::SUCCESS;
}

/**
 * @brief 申请一个frame，如果没有空闲的frame，则驱逐一些frame
 */
RC FileBufferPool::allocate_frame(PageNum page_num, Frame **buffer)
{
  auto evict_action = [this](Frame *frame) {
    if (!frame->dirty()) {
      return RC::SUCCESS;
    }
    RC rc = RC::SUCCESS;
    if (frame->file_desc() == file_desc_) {
      rc = this->flush_page_internal(*frame);
    } else {
      rc = bp_manager_.flush_page(*frame);
    }
    if (rc != RC::SUCCESS) {
      LOG_ERROR("Failed to aclloc block due to failed to flush old block. rc=%s", strrc(rc));
    }
    return rc;
  };

  while (true) {
    Frame *frame = frame_manager_.alloc(file_desc_, page_num);
    if (frame != nullptr) {
      *buffer = frame;
      return RC::SUCCESS;
    }
    LOG_TRACE("frames are all allocated, so we should evict some frames to get one free frame");
    (void)frame_manager_.evict_frames(1, evict_action);
  }
  return RC::BUFFERPOOL_NOBUF;
}

/**
 * @brief 从磁盘上加载一个页面到frame中
 * 1. 根据page_num计算出该页面在文件中的偏移量
 * 2. 读取数据到frame中
 */
RC FileBufferPool::load_page(PageNum page_num, Frame *frame)
{
  int64_t offset = ((int64_t)page_num) * BP_PAGE_SIZE;
  if (lseek(file_desc_, offset, SEEK_SET) == -1) {
    LOG_ERROR("Failed to load page %s:%d, due to failed to lseek:%s.", file_name_.c_str(), page_num, strerror(errno));

    return RC::IOERR_SEEK;
  }

  Page &page = frame->page();
  int ret = readn(file_desc_, &page, BP_PAGE_SIZE);
  if (ret != 0) {
    LOG_ERROR("Failed to load page %s, file_desc:%d, page num:%d, due to failed to read data:%s, ret=%d, page count=%d",
              file_name_.c_str(), file_desc_, page_num, strerror(errno), ret, file_header_->allocated_pages);
    return RC::IOERR_READ;
  }
  return RC::SUCCESS;
}

int FileBufferPool::file_desc() const
{
  return file_desc_;
}

RC FileBufferPool::recover_page(PageNum page_num)
{
  return RC::SUCCESS;
}

RC FileBufferPool::dispose_page(PageNum page_num)
{
  std::scoped_lock lock_guard(lock_);
  Frame *used_frame = frame_manager_.get(file_desc_, page_num);
  if (used_frame != nullptr) {
    ASSERT("the page try to dispose is in use. frame:%s", to_string(*used_frame).c_str());
    frame_manager_.free(file_desc_, page_num, used_frame);
  } else {
    LOG_WARN("failed to fetch the page while disposing it. pageNum=%d", page_num);
    return RC::NOTFOUND;
  }

  hdr_frame_->mark_dirty();
  file_header_->allocated_pages--;
  char tmp = 1 << (page_num % 8);
  file_header_->bitmap[page_num / 8] &= ~tmp;
  return RC::SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////

BufferPoolIterator::BufferPoolIterator()
{}
BufferPoolIterator::~BufferPoolIterator()
{}
RC BufferPoolIterator::init(FileBufferPool &bp, PageNum start_page /* = 0 */)
{
  bitmap_.init(bp.file_header_->bitmap, bp.file_header_->page_count);
  if (start_page <= 0) {
    current_page_num_ = 0;
  } else {
    current_page_num_ = start_page;
  }
  return RC::SUCCESS;
}

bool BufferPoolIterator::has_next()
{
  return bitmap_.next_setted_bit(current_page_num_ + 1) != -1;
}

PageNum BufferPoolIterator::next()
{
  PageNum next_page = bitmap_.next_setted_bit(current_page_num_ + 1);
  if (next_page != -1) {
    current_page_num_ = next_page;
  }
  return next_page;
}

RC BufferPoolIterator::reset()
{
  current_page_num_ = 0;
  return RC::SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////

BufferPoolManager::BufferPoolManager(int memory_size /* = 0 */)
{
  if (memory_size <= 0) {
    memory_size = MEM_POOL_ITEM_NUM * DEFAULT_ITEM_NUM_PER_POOL * BP_PAGE_SIZE;
  }
  const int pool_num = std::max(memory_size / BP_PAGE_SIZE / DEFAULT_ITEM_NUM_PER_POOL, 1);
  frame_manager_.init(pool_num);
  LOG_INFO("buffer pool manager init with memory size %d, page num: %d, pool num: %d",
           memory_size, pool_num * DEFAULT_ITEM_NUM_PER_POOL, pool_num);
}

BufferPoolManager::~BufferPoolManager()
{
  std::unordered_map<std::string, FileBufferPool *> tmp_bps;
  tmp_bps.swap(buffer_pools_);
  for (auto &iter : tmp_bps) {
    delete iter.second;
  }
}

RC BufferPoolManager::create_file(const char *file_name)
{
  int fd = open(file_name, O_RDWR | O_CREAT | O_EXCL, S_IREAD | S_IWRITE);
  if (fd < 0) {
    LOG_ERROR("Failed to create %s, due to %s.", file_name, strerror(errno));
    return RC::SCHEMA_DB_EXIST;
  }
  close(fd);

  fd = open(file_name, O_RDWR);
  if (fd < 0) {
    LOG_ERROR("Failed to open for readwrite %s, due to %s.", file_name, strerror(errno));
    return RC::IOERR_ACCESS;
  }

  Page page;
  memset(&page, 0, BP_PAGE_SIZE);

  FileHeader *file_header = (FileHeader *)page.data;
  file_header->allocated_pages = 1;
  file_header->page_count = 1;

  char *bitmap = file_header->bitmap;
  bitmap[0] |= 0x01;
  if (lseek(fd, 0, SEEK_SET) == -1) {
    LOG_ERROR("Failed to seek file %s to position 0, due to %s .", file_name, strerror(errno));
    close(fd);
    return RC::IOERR_SEEK;
  }

  if (writen(fd, (char *)&page, BP_PAGE_SIZE) != 0) {
    LOG_ERROR("Failed to write header to file %s, due to %s.", file_name, strerror(errno));
    close(fd);
    return RC::IOERR_WRITE;
  }
  close(fd);

  LOG_INFO("Successfully create %s.", file_name);
  return RC::SUCCESS;
}

RC BufferPoolManager::open_file(const char *_file_name, FileBufferPool *&_bp)
{
  std::string file_name(_file_name);

  std::scoped_lock lock_guard(lock_);
  if (buffer_pools_.find(file_name) != buffer_pools_.end()) {
    LOG_WARN("file already opened. file name=%s", _file_name);
    return RC::BUFFERPOOL_OPEN;
  }

  FileBufferPool *bp = new FileBufferPool(*this, frame_manager_);
  RC rc = bp->open_file(_file_name);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open file name");
    delete bp;
    return rc;
  }

  buffer_pools_.insert(std::pair<std::string, FileBufferPool *>(file_name, bp));
  fd_buffer_pools_.insert(std::pair<int, FileBufferPool *>(bp->file_desc(), bp));
  LOG_DEBUG("insert buffer pool into fd buffer pools. fd=%d, bp=%p, lbt=%s", bp->file_desc(), bp, lbt());
  _bp = bp;
  return RC::SUCCESS;
}

RC BufferPoolManager::close_file(const char *_file_name)
{
  std::string file_name(_file_name);

  lock_.lock();

  auto iter = buffer_pools_.find(file_name);
  if (iter == buffer_pools_.end()) {
    LOG_TRACE("file has not opened: %s", _file_name);
    lock_.unlock();
    return RC::INTERNAL;
  }

  int fd = iter->second->file_desc();
  if (0 == fd_buffer_pools_.erase(fd)) {
    int count = 0;
    for (auto fd_iter = fd_buffer_pools_.begin(); fd_iter != fd_buffer_pools_.end(); ++fd_iter) {
      if (fd_iter->second == iter->second) {
        fd_buffer_pools_.erase(fd_iter);
        count = 1;
        break;
      }
    }
    ASSERT(count == 1, "the buffer pool was not erased from fd buffer pools.");
  }

  FileBufferPool *bp = iter->second;
  buffer_pools_.erase(iter);
  lock_.unlock();

  delete bp;
  return RC::SUCCESS;
}

/**
 * TODO [Lab1] 需要同学们实现页面刷盘
 */
RC BufferPoolManager::flush_page(Frame &frame)
{
  return RC::SUCCESS;
}

static BufferPoolManager *default_bpm = nullptr;
void BufferPoolManager::set_instance(BufferPoolManager *bpm)
{
  if (default_bpm != nullptr && bpm != nullptr) {
    LOG_ERROR("default buffer pool manager has been setted");
    abort();
  }
  default_bpm = bpm;
}
BufferPoolManager &BufferPoolManager::instance()
{
  return *default_bpm;
}
