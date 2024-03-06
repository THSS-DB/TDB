#include <inttypes.h>

#include "storage/clog/clog.h"

using namespace std;

void dump(const char *filename)
{
  CLogFile file;
  RC rc = file.init(filename);
  if (RC_FAIL(rc)) {
    printf("failed to init file: '%s'. syserr=%s, rc=%s\n", filename, strerror(errno), strrc(rc));
    return;
  }

  CLogRecordIterator iterator;
  rc = iterator.init(file);
  if (RC_FAIL(rc)) {
    printf("failed to init iterator. rc=%s\n", strrc(rc));
    return;
  }
  
  int64_t offset = 0;
  int index = 0;
  for (index++, rc = iterator.next(); RC_SUCC(rc) && iterator.valid(); rc = iterator.next(), ++index) {
    const CLogRecord &log_record = iterator.log_record();
    
    printf("index:%d, file_offset:%" PRId64 ", %s\n", index, offset, log_record.to_string().c_str());
    (void)file.offset(offset);
  }

  if (rc != RC::RECORD_EOF) {
    printf("something error. error=%s\n", strrc(rc));
  }
}

int main(int argc, char *argv[])
{
  if (argc < 2) {
    printf("please give me a clog file name\n");
    return 1;
  }

  dump(argv[1]);
  return 0;
}
