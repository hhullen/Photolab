#ifndef RUNNER_H
#define RUNNER_H

#include <algorithm>
#include <cstddef>
#include <future>
#include <optional>
#include <vector>

#include "channel.h"
#include "runner_interface.h"

using Task = std::packaged_task<void()>;
using TaskChan = Channel<Task>;

class Runner : public IRunner {
public:
  explicit Runner(size_t pixelSizeInBytes = 4)
      : pixelSizeInBytes_{pixelSizeInBytes} {
    size_t cores = std::thread::hardware_concurrency();
    cores /= 2;
    if (cores < 1) {
      cores = 2;
    }

    for (size_t i = 0; i < cores; i++) {
      auto job = [](TaskChan &ch) {
        for (; !ch.IsClosed();) {
          if (std::optional<Task> task = ch.Recv(); task.has_value()) {
            task.value()();
          }
        }
      };

      std::thread *thr = new std::thread(std::move(job), std::ref(ch_));
      workers_.emplace_back(thr);
    }
  }

  Runner(const Runner &) = delete;
  Runner &operator=(const Runner &) = delete;

  ~Runner() {
    ch_.Close();
    for (auto &thr : workers_) {
      thr->join();
    }
  }

  bool RunPerPixInWorkers(size_t sizeInBytes,
                          std::function<void(size_t i)> func) override {
    if ((sizeInBytes % pixelSizeInBytes_) != 0) {
      return false;
    }

    std::vector<std::future<void>> futures;

    size_t pixelPerWorker = sizeInBytes / pixelSizeInBytes_ / workers_.size();
    size_t bytesPerWorker = pixelPerWorker * pixelSizeInBytes_;

    if (bytesPerWorker <= minByteSize_) {
      for (size_t i = 0; i < sizeInBytes; i += pixelSizeInBytes_) {
        if (func) {
          func(i);
        }
      }
      return true;
    }

    for (size_t from = 0; from < sizeInBytes; from += bytesPerWorker) {
      size_t to = std::clamp<size_t>(from + bytesPerWorker, 0, sizeInBytes);
      Task task([from, to, &func, this] {
        for (size_t i = from; i < to; i += pixelSizeInBytes_) {
          if (func) {
            func(i);
          }
        }
      });

      futures.emplace_back(task.get_future());
      ch_.Send(std::move(task));
    }

    for (auto &fut : futures) {
      fut.wait();
    }

    return true;
  }

  size_t getWorkers() override { return workers_.size(); }

private:
  size_t pixelSizeInBytes_;
  TaskChan ch_;
  std::vector<std::thread *> workers_;

  constexpr static size_t minByteSize_{10000};
};

#endif // RUNNER_H
