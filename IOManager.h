#ifndef __IO_MANAGER_H__
#define __IO_MANAGER_H__

#include <atomic>
#include <functional>
#include <memory>
#include <shared_mutex>
#include <vector>

#include "IOContext.h"

struct IOContext;
struct IOManager {
  IOManager();
  virtual ~IOManager();
  IOManager &addContext(Entry entry, void *param);
  void start(std::function<void()> func);
  void doRead(int ctxId, char *buf, int len);

 private:
  std::shared_mutex _routinesMutex;
  std::vector<std::shared_ptr<IOContext>> _routines;
};

#endif
