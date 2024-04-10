#pragma once

#include <string.h>
#include <sstream>
#include <functional>
#include <memory>

#include "include/storage_engine/recorder/record_manager.h"
#include "include/storage_engine/buffer/buffer_pool.h"
#include "include/query_engine/parser/parse_defs.h"
#include "common/lang/comparator.h"
#include "common/log/log.h"

/**
 * @brief B+树的实现
 * @defgroup BPlusTree
 */

/**
 * @brief B+树的操作类型
 */
enum class BplusTreeOperationType
{
  READ,
  INSERT,
  DELETE,
};

/**
 * @brief 属性比较器
 */
class AttrComparator
{
 public:
  void init(AttrType type, int length)
  {
    attr_type_ = type;
    attr_length_ = length;
  }

  int attr_length() const
  {
    return attr_length_;
  }

  int operator()(const char *v1, const char *v2) const
  {
    switch (attr_type_) {
      case INTS: {
        return common::compare_int((void *)v1, (void *)v2);
      } break;
      case FLOATS: {
        return common::compare_float((void *)v1, (void *)v2);
      }
      case CHARS: {
        return common::compare_string((void *)v1, attr_length_, (void *)v2, attr_length_);
      }
      case DATES: {
        return common::compare_int((void *)v1, (void *)v2);
      }
      default: {
        ASSERT(false, "unknown attr type. %d", attr_type_);
        return 0;
      }
    }
  }

 private:
  AttrType attr_type_;
  int attr_length_;
};

/**
 * @brief 键的比较器
 * @details BplusTree的键值除了字段属性，还有RID，它是为了避免属性值重复而增加的。
 */
class KeyComparator
{
 public:
  void init(AttrType type, int length)
  {
    attr_comparator_.init(type, length);
  }

  const AttrComparator &attr_comparator() const
  {
    return attr_comparator_;
  }

  int operator()(const char *v1, const char *v2) const
  {
    int result = attr_comparator_(v1, v2);
    if (result != 0) {
      return result;
    }

    const RID *rid1 = (const RID *)(v1 + attr_comparator_.attr_length());
    const RID *rid2 = (const RID *)(v2 + attr_comparator_.attr_length());
    return RID::compare(rid1, rid2);
  }

 private:
  AttrComparator attr_comparator_;
};

/**
 * @brief 属性打印器,调试使用
 */
class AttrPrinter
{
 public:
  void init(AttrType type, int length)
  {
    attr_type_ = type;
    attr_length_ = length;
  }

  int attr_length() const
  {
    return attr_length_;
  }

  std::string operator()(const char *v) const
  {
    switch (attr_type_) {
      case INTS: {
        return std::to_string(*(int *)v);
      }
      case FLOATS: {
        return std::to_string(*(float *)v);
      }
      case CHARS: {
        std::string str;
        for (int i = 0; i < attr_length_; i++) {
          if (v[i] == 0) {
            break;
          }
          str.push_back(v[i]);
        }
        return str;
      }
      case DATES: {
        return std::to_string(*(int *)v);
      }
      default: {
        ASSERT(false, "unknown attr type. %d", attr_type_);
      }
    }
    return std::string();
  }

 private:
  AttrType attr_type_;
  int attr_length_;
};

/**
 * @brief 键的打印器,调试使用
 */
class KeyPrinter
{
 public:
  void init(AttrType type, int length)
  {
    attr_printer_.init(type, length);
  }

  const AttrPrinter &attr_printer() const
  {
    return attr_printer_;
  }

  std::string operator()(const char *v) const
  {
    std::stringstream ss;
    ss << "{key:" << attr_printer_(v) << ",";

    const RID *rid = (const RID *)(v + attr_printer_.attr_length());
    ss << "rid:{" << rid->to_string() << "}}";
    return ss.str();
  }

 private:
  AttrPrinter attr_printer_;
};

/**
 * @brief the meta information of bplus tree
 * @details this is the first page of bplus tree.
 */
struct IndexFileHeader
{
  IndexFileHeader()
  {
    memset(this, 0, sizeof(IndexFileHeader));
    root_page = BP_INVALID_PAGE_NUM;
  }
  PageNum root_page;          // 根节点在磁盘中的页号
  int32_t internal_max_size;  // 内部节点最大的键值对数
  int32_t leaf_max_size;      // 叶子节点最大的键值对数
  int32_t attr_amount;        // 索引字段的个数
  int32_t multi_attr_lengths[MAX_FIELD_AMOUNT];  // 每个索引字段的长度
  AttrType multi_attr_types[MAX_FIELD_AMOUNT];   // 每个索引字段的类型
  int32_t attrs_length;       // 索引字段的总长度
  int32_t key_length;         // 索引键的总长度，attrs length + sizeof(RID)
  AttrType attrs_type;        // 索引字段的整体类型：如果是多列索引，则统一视为CHAR类型
  bool is_unique_;            // 是否是唯一索引

  const std::string to_string()
  {
    std::stringstream ss;
    ss << "attr_length:" << multi_attr_lengths[0];
    for (int i = 1; i < attr_amount; i++) {
      ss << "|" << multi_attr_lengths[i];
    }
    ss << ","
       << "key_length:" << key_length << ","
       << "attr_type:" << multi_attr_types[0];
    for (int i = 1; i < attr_amount; i++) {
      ss << "|" << multi_attr_types[i];
    }
    ss << "is unique:" << is_unique_ << ","
       << "root_page:" << root_page << ","
       << "internal_max_size:" << internal_max_size << ","
       << "leaf_max_size:" << leaf_max_size << ";";
    return ss.str();
  }
};

/**
 * @brief the common part of page describtion of bplus tree
 * @code
 * storage format:
 * | page type | item number | parent page id |
 * @endcode
 */
struct IndexNode
{
  static constexpr int HEADER_SIZE = 12;

  bool    is_leaf;  // whether is leaf node
  int     key_num;  // the number of keys
  PageNum parent;   // the parent page id
};

/**
 * @brief leaf page of bplus tree
 * @code
 * storage format:
 * | common header | next page id |
 * | key0, rid0 | ... | keyn, ridn |
 * @endcode
 * the key is in format: the key value of record and rid.
 * so the key in leaf page must be unique.
 * the value is rid.
 */
struct LeafIndexNode : public IndexNode
{
  static constexpr int HEADER_SIZE = IndexNode::HEADER_SIZE + 4;

  PageNum next_brother;
  /**
   * leaf can store order keys and rids at most
   */
  char array[0];
};

/**
 * @brief internal page of bplus tree
 * @code
 * storage format:
 * | common header |
 * | key(0),page_id(0) | key(1), page_id(1) | ... | key(n), page_id(n) |
 * @endcode
 * the first key is ignored(key0).
 */
struct InternalIndexNode : public IndexNode
{
  static constexpr int HEADER_SIZE = IndexNode::HEADER_SIZE;

  /**
   * internal node just store key_num-1 keys and key_num page_ids, the last page_id is last right child.
   */
  char array[0];
};

/**
 * @brief IndexNode 仅作为数据在内存或磁盘中的表示；IndexNodeHandler 负责对IndexNode做各种操作。
 * 作为一个类来说，虚函数会影响“结构体”真实的内存布局，所以将数据存储与操作分开
 */
class IndexNodeHandler
{
 public:
  IndexNodeHandler(const IndexFileHeader &header, Frame *frame);
  virtual ~IndexNodeHandler() = default;

  void init_empty(bool leaf);

  bool is_leaf() const;
  int  key_size() const;
  int  value_size() const;
  int  item_size() const;

  void increase_size(int n);
  int  size() const;
  int  max_size() const;
  int  min_size() const;
  void set_parent_page_num(PageNum page_num);
  PageNum parent_page_num() const;
  PageNum page_num() const;

  bool is_safe(BplusTreeOperationType op, bool is_root_node);

  bool validate() const;

  friend std::string to_string(const IndexNodeHandler &handler);

 protected:
  const IndexFileHeader &header_;  // 索引文件的头部信息
  PageNum page_num_;  // 当前索引节点在磁盘中的页号
  IndexNode *node_;  // 当前索引节点的数据
};

/**
 * @brief 叶子节点的操作
 */
class LeafIndexNodeHandler : public IndexNodeHandler
{
 public:
  LeafIndexNodeHandler(const IndexFileHeader &header, Frame *frame);
  virtual ~LeafIndexNodeHandler() = default;

  void init_empty();
  void set_next_page(PageNum page_num);
  PageNum next_page() const;

  char *key_at(int index);
  char *value_at(int index);

  /**
   * 查找指定key的插入位置(注意不是key本身)
   * 如果key已经存在，会设置found的值。
   */
  int lookup(const KeyComparator &comparator, const char *key, bool *found = nullptr) const;

  void insert(int index, const char *key, const char *value);
  void remove(int index);
  int  remove(const char *key, const KeyComparator &comparator);
  RC move_half_to(LeafIndexNodeHandler &other, FileBufferPool *bp);
  /**
   * move the first item of current leaf node to the end of the left leaf node
   */
  RC move_first_to_end(LeafIndexNodeHandler &other, FileBufferPool *bp);
  /**
   * move the last item of current leaf node to the front of the right leaf node
   */
  RC move_last_to_front(LeafIndexNodeHandler &other, FileBufferPool *bp);
  /**
   * move all items to left page
   */
  RC move_to(LeafIndexNodeHandler &other, FileBufferPool *bp);

  bool validate(const KeyComparator &comparator, FileBufferPool *bp) const;

  friend std::string to_string(const LeafIndexNodeHandler &handler, const KeyPrinter &printer);

 private:
  char *__item_at(int index) const;
  char *__key_at(int index) const;
  char *__value_at(int index) const;

  void append(const char *item);
  /**
   * insert one item to the first slot
   */
  void preappend(const char *item);

 private:
  LeafIndexNode *leaf_node_;
};

/**
 * @brief 内部节点的操作
 */
class InternalIndexNodeHandler : public IndexNodeHandler
{
 public:
  InternalIndexNodeHandler(const IndexFileHeader &header, Frame *frame);
  virtual ~InternalIndexNodeHandler() = default;

  void init_empty();
  void create_new_root(PageNum first_page_num, const char *key, PageNum page_num);

  char *key_at(int index);
  void set_key_at(int index, const char *key);
  PageNum value_at(int index);
  /**
   * 返回指定子节点在当前节点中的索引
   */
  int value_index(PageNum page_num);


  /**
   * 与Leaf节点不同，lookup返回指定key应该属于哪个子节点，返回这个子节点在当前节点中的索引
   * 如果想要返回插入位置，就提供 `insert_position` 参数
   * @param[in] comparator 用于键值比较的函数
   * @param[in] key 查找的键值
   * @param[out] found 如果是有效指针，将会返回当前是否存在指定的键值
   * @param[out] insert_position 如果是有效指针，将会返回可以插入指定键值的位置
   */
  int lookup(const KeyComparator &comparator, const char *key, bool *found = nullptr, int *insert_position = nullptr) const;

  void insert(const char *key, PageNum page_num, const KeyComparator &comparator);
  void remove(int index);

  RC move_half_to(LeafIndexNodeHandler &other, FileBufferPool *bp);
  RC move_half_to(InternalIndexNodeHandler &other, FileBufferPool *bp);
  RC move_first_to_end(InternalIndexNodeHandler &other, FileBufferPool *bp);
  RC move_last_to_front(InternalIndexNodeHandler &other, FileBufferPool *bp);
  RC move_to(InternalIndexNodeHandler &other, FileBufferPool *bp);

  bool validate(const KeyComparator &comparator, FileBufferPool *bp) const;

  friend std::string to_string(const InternalIndexNodeHandler &handler, const KeyPrinter &printer);

 private:
  RC copy_from(const char *items, int num, FileBufferPool *disk_buffer_pool);
  RC append(const char *item, FileBufferPool *bp);
  RC preappend(const char *item, FileBufferPool *bp);

 private:
  char *__item_at(int index) const;
  char *__key_at(int index) const;
  char *__value_at(int index) const;

  int value_size() const;
  int item_size() const;

 private:
  InternalIndexNode *internal_node_ = nullptr;
};

/**
 * @brief B+树的实现
 */
class BplusTreeHandler
{
 public:
  /**
   * @details 此函数创建一个名为fileName的索引。
   * @param file_name 索引文件的名字
   * @param is_unique 是否是唯一索引
   * @param multi_attr_types 每个索引字段的类型
   * @param multi_attr_length 每个索引字段的长度
   * @param internal_max_size 内部节点最大的键值对数
   * @param leaf_max_size 叶子节点最大的键值对数
   */
  RC create(const char *file_name,
            bool is_unique,
            std::vector<AttrType> multi_attr_types,
            std::vector<int> multi_attr_length,
            int internal_max_size = -1,
            int leaf_max_size = -1);

  /**
   * 打开名为fileName的索引文件。
   * 如果方法调用成功，则indexHandle为指向被打开的索引句柄的指针。
   * 索引句柄用于在索引中插入或删除索引项，也可用于索引的扫描
   */
  RC open(const char *file_name);

  /**
   * 关闭句柄indexHandle对应的索引文件
   */
  RC close();

  /**
   * 此函数向IndexHandle对应的索引中插入一个索引项。
   * 参数multi_keys指向要插入的属性值（之所以是数组，因为可能是多字段索引），参数rid标识该索引项对应的元组位置，
   * 即向索引中插入一个值为（multi_keys，rid）的键值对
   */
  RC insert_entry(const char *multi_keys[], const RID *rid, int multi_keys_amount = 1);

  /**
   * 从IndexHandle句柄对应的索引中删除一个值为（multi_keys，rid）的索引项
   * @return RECORD_INVALID_KEY 指定值不存在
   */
  RC delete_entry(const char *multi_keys[], const RID *rid, int multi_keys_amount = 1);

  bool is_empty() const;

  /**
   * 获取指定值的record对应的RID
   * @param multi_keys 索引字段的属性值数组（之所以是数组，因为可能是多字段索引）
   * @param rids  返回值，存储记录所在的位置（之所以是列表，因为可能该字段上有重复值）
   */
  RC get_entry(const char *multi_keys[], std::list<RID> &rids, int multi_keys_amount = 1);

  RC sync();

  /**
   * Check whether current B+ tree is invalid or not.
   * @return true means current tree is valid, return false means current tree is invalid.
   * @note thread unsafe
   */
  bool validate_tree();

 public:
  /**
   * 这些函数都是线程不安全的，不要在多线程的环境下调用
   */
  RC print_tree();
  RC print_leafs();

 private:
  /**
   * 这些函数都是线程不安全的，不要在多线程的环境下调用
   */
  RC print_leaf(Frame *frame);
  RC print_internal_node_recursive(Frame *frame);

  bool validate_leaf_link();
  bool validate_node_recursive(Frame *frame);

 protected:
  RC find_leaf(BplusTreeOperationType op, const char *key, Frame *&frame);
  RC left_most_page(Frame *&frame);
  RC find_leaf_internal(BplusTreeOperationType op,
                        const std::function<PageNum(InternalIndexNodeHandler &)> &child_page_getter,
                        Frame *&frame);
  RC crabing_protocal_fetch_page(BplusTreeOperationType op, PageNum page_num, bool is_root_page,
                                 Frame *&frame);

  RC insert_into_parent(PageNum parent_page, Frame *left_frame, const char *pkey,
                        Frame &right_frame);

  RC delete_entry_internal(Frame *leaf_frame, const char *key);

  template <typename IndexNodeHandlerType>
  RC split(Frame *frame, Frame *&new_frame);
  template <typename IndexNodeHandlerType>
  RC coalesce_or_redistribute(Frame *frame);
  template <typename IndexNodeHandlerType>
  RC coalesce(Frame *neighbor_frame, Frame *frame, Frame *parent_frame, int index);
  template <typename IndexNodeHandlerType>
  RC redistribute(Frame *neighbor_frame, Frame *frame, Frame *parent_frame, int index);

  RC insert_entry_into_parent(Frame *frame, Frame *new_frame, const char *key);
  RC insert_entry_into_leaf_node(Frame *frame, const char *pkey, const RID *rid);
  RC create_new_tree(const char *key, const RID *rid);

  void update_root_page_num(PageNum root_page_num);
  void update_root_page_num_locked(PageNum root_page_num);

  RC adjust_root(Frame *root_frame);

 private:
  common::MemPoolItem::unique_ptr make_key(const char *multi_keys[], const RID &rid, int multi_keys_num = 1, int left_or_right = 0, bool all_in_one_input_key = false);
  void free_key(char *key);

 protected:
  FileBufferPool *file_buffer_pool_ = nullptr;
  bool            header_dirty_ = false;
  IndexFileHeader file_header_;

  KeyComparator   key_comparator_;
  KeyPrinter      key_printer_;

  std::unique_ptr<common::MemPoolItem> mem_pool_item_;

 private:
  friend class BplusTreeScanner;
  friend class BplusTreeTester;
};

/**
 * @brief B+树的扫描器
 * @ingroup BPlusTree
 */
class BplusTreeScanner
{
 public:
  BplusTreeScanner(BplusTreeHandler &tree_handler);
  ~BplusTreeScanner();

  /**
   * @brief 扫描指定范围的数据
   * @param left_user_key 扫描范围的左边界，如果是null，则没有左边界
   * @param left_len left_user_key 的内存大小(只有在变长字段中才会关注)
   * @param left_inclusive 左边界的值是否包含在内
   * @param right_user_key 扫描范围的右边界。如果是null，则没有右边界
   * @param right_len right_user_key 的内存大小(只有在变长字段中才会关注)
   * @param right_inclusive 右边界的值是否包含在内
   */
  RC open(const char *left_user_key, int left_len, bool left_inclusive,
          const char *right_user_key, int right_len, bool right_inclusive);

  RC next_entry(RID &rid, bool isdelete);

  RC close();

 private:
  /**
   * 如果key的类型是CHARS, 扩展或缩减user_key的大小刚好是schema中定义的大小
   */
  RC fix_user_key(const char *user_key, int key_len, bool want_greater, char **fixed_key, bool *should_inclusive);

  void fetch_item(RID &rid);
  bool touch_end();

 private:
  bool inited_ = false;
  BplusTreeHandler &tree_handler_;

  /// 使用左右叶子节点和位置来表示扫描的起始位置和终止位置
  /// 起始位置和终止位置都是有效的数据
  Frame *current_frame_ = nullptr;

  common::MemPoolItem::unique_ptr right_key_;
  int iter_index_ = 0;
  bool first_emitted_ = false;
};
