#ifndef RUNNER_MOCK_H
#define RUNNER_MOCK_H

#include <gmock/gmock.h>

#include "runner_interface.h"

class MockRunner : public IRunner {
public:
  MOCK_METHOD(bool, RunPerPixInWorkers,
              (size_t sizeInBytes, std::function<void(size_t i)> func),
              (override));

  MOCK_METHOD(size_t, getWorkers, (), (override));

  static bool MockRunPerPixInWorkers(size_t sizeInBytes,
                                     std::function<void(size_t i)> func) {
    constexpr size_t pixelSizeInBytes_{4};
    for (size_t i = 0; i < sizeInBytes; i += pixelSizeInBytes_) {
      if (func) {
        func(i);
      }
    }
    return true;
  }

  static size_t MockGetWorkers() { return 1; }
};

#endif // RUNNER_MOCK_H
