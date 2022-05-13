#include "IOContext.h"

#include "IOManager.h"

IOContext::IOContext(IOManager &scheduler, int id, Entry entry, void *param)
    : _ioManager(scheduler),
      _id(id),
      _entry(entry),
      _param(param),
      _setjmpFlag(),
      _jmpBuf() {}
IOContext::~IOContext() {}
int IOContext::getId() const { return _id; }
void *IOContext::getParam() { return _param; }
IOContext &IOContext::setTimes(int times) {
  _times = times;
  return *this;
}
int IOContext::getTimes() const { return _times; }
int IOContext::read(char *buf, int len) {
  auto code = 0;
  if (!_setjmpFlag) {
    _setjmpFlag = true;
    code = setjmp(_jmpBuf);
  }
  if (0 == code) _ioManager.doRead(_id, buf, len);
  return code;
}
