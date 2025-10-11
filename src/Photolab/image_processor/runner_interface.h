#ifndef RUNNER_INTERFACE_H
#define RUNNER_INTERFACE_H

class IRunner {
public:
  IRunner() = default;
  virtual ~IRunner() = 0;
  IRunner(const IRunner &) = delete;
  IRunner &operator=(const IRunner &) = delete;

  virtual bool RunPerPixInWorkers(size_t sizeInBytes,
                                  std::function<void(size_t i)> func) = 0;

  virtual size_t getWorkers() = 0;
};

inline IRunner::~IRunner() {}

#endif // RUNNER_INTERFACE_H
