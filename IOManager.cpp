#include "IOManager.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>

IOManager::IOManager() {}
IOManager::~IOManager() {}
IOManager &IOManager::addContext(Entry entry, void *param) {
  std::lock_guard<std::shared_mutex> lockGuard(_routinesMutex);
  auto routine =
      std::make_shared<IOContext>(*this, _routines.size(), entry, param);
  _routines.push_back(routine);
  return *this;
}
void IOManager::start(std::function<void()> func) { func(); }
void IOManager::doRead(int ctxId, char *buf, int len) {
  if (buf && 0 < len) {
    if (rand() % 2) {
      // has data
      const char *data = "hello";
      auto dataLen = strlen(data);
      if (dataLen > len) dataLen = len;
      memcpy(buf, data, dataLen);
      auto ok = false;
      jmp_buf target;
      {
        std::shared_lock<std::shared_mutex> lockGuard(_routinesMutex);
        for (auto routine : _routines) {
          if (routine->getId() == ctxId) {
            routine->setTimes(routine->getTimes() + 1);
            memcpy(target, routine->_jmpBuf, sizeof(routine->_jmpBuf));
            ok = true;
            break;
          }
        }
      }
      if (ok) longjmp(target, dataLen);
      return;
    }
  }
  // switch to another
  std::shared_ptr<IOContext> ctx;
  {
    std::lock_guard<std::shared_mutex> lockGuard(_routinesMutex);
    ctx = _routines.at(0);
    for (auto i = 1; i < _routines.size(); ++i) {
      if (ctx->getTimes() > _routines.at(i)->getTimes()) {
        ctx = _routines.at(i);
        break;
      }
    }
  }
  if (ctx) ctx->_entry(ctx.get());
}
