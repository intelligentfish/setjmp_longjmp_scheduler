#ifndef __IO_CONTEXT_H__
#define __IO_CONTEXT_H__

#include <setjmp.h>

struct IOContext;
struct IOManager;
typedef void (*Entry)(IOContext *context);

struct IOContext {
  IOContext(IOManager &manager, int id, Entry entry, void *param);
  virtual ~IOContext();
  int getId() const;
  void *getParam();
  IOContext &setTimes(int times);
  int getTimes() const;
  int read(char *buf, int len);

 private:
  friend struct IOManager;
  IOManager &_ioManager;
  int _times;
  int _id;
  Entry _entry;
  void *_param;
  bool _setjmpFlag;
  jmp_buf _jmpBuf;
};

#endif
