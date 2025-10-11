#ifndef CHANNEL_H
#define CHANNEL_H

#include <condition_variable>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <vector>

template <typename T> class Channel {
public:
  Channel(int buffer = 10) {
    queue_.resize(buffer);
    if (buffer < 1) {
      throw std::invalid_argument("Shit man! Do u really wanna set 0 beffer?!");
    }
  }

  ~Channel() {}

  Channel(const Channel &) = delete;
  Channel &operator=(const Channel &) = delete;

  bool Send(T task) {
    {
      std::unique_lock<std::mutex> lock(mutex_);
      cvSend_.wait(lock, [this]() {
        return (idxSend_ - idxRecv_) < queue_.size() || closed_;
      });

      if (closed_) {
        return false;
      }

      queue_[idxSend_ % queue_.size()] = std::move(task);
      idxSend_++;

      if (idxSend_ == std::numeric_limits<size_t>::max()) {
        size_t diff = idxSend_ - idxRecv_;
        idxRecv_ = idxRecv_ % queue_.size();
        idxSend_ = idxRecv_ + diff;
      }
    }
    cvRecv_.notify_one();
    return true;
  }

  std::optional<T> Recv() {
    std::unique_lock<std::mutex> lock(mutex_);
    cvRecv_.wait(lock, [this]() { return idxRecv_ < idxSend_ || closed_; });

    if (closed_) {
      return std::nullopt;
    }

    T value = std::move(queue_[idxRecv_ % queue_.size()]);
    idxRecv_++;

    cvSend_.notify_one();
    return std::move(value);
  }

  void Close() {
    std::lock_guard<std::mutex> lock(mutex_);
    closed_ = true;
    cvRecv_.notify_all();
    cvSend_.notify_all();
  }

  bool IsClosed() {
    std::lock_guard<std::mutex> lock(mutex_);
    return closed_;
  }

private:
  bool closed_{false};
  std::mutex mutex_;
  std::vector<T> queue_;
  size_t idxSend_{0}, idxRecv_{0};
  std::condition_variable cvSend_, cvRecv_;
};

#endif // CHANNEL_H
