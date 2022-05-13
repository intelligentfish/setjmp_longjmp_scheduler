#include <iostream>

#include "IOContext.h"
#include "IOManager.h"

int main(int argc, char **argv, char **env) {
  IOManager ioManager;
  ioManager.start([&ioManager]() {
    for (auto i = 0; i < 20; i++) {
      ioManager.addContext(
          [](IOContext *ctx) {
            char buf[128]{};
            for (;;) {
              const auto n = ctx->read(buf, sizeof(buf));
              if (0 >= n) break;
              std::cout << "Context: " << ctx->getId() << ", read " << n
                        << std::endl;
            }
          },
          nullptr);
    }
    ioManager.doRead(1, nullptr, 0);
  });
  return 0;
}
