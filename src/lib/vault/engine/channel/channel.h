#ifndef SRC_LIB_VAULT_ENGINE_CHANNEL_CHANNEL_H_
#define SRC_LIB_VAULT_ENGINE_CHANNEL_CHANNEL_H_

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>

namespace hhullen {

template <class T>
class Channel {
 public:
  ~Channel() {
    Close();
    condition_.notify_one();
  }

  operator bool() const { return !closed_; }

  void Send(T item) {
    std::unique_lock<decltype(mutex_)> locker(mutex_);
    buffer_.emplace(item);
    condition_.notify_one();
  }

  T Get() {
    std::unique_lock<decltype(mutex_)> locker(mutex_);
    condition_.wait(locker, [this]() { return !buffer_.empty(); });
    if (!closed_) {
      T item = buffer_.front();
      buffer_.pop();
      return item;
    }
    return T();
  }

  void Close() {
    closed_ = true;
    buffer_.emplace(T());
    condition_.notify_one();
  }

 private:
  std::queue<T> buffer_;
  std::mutex mutex_;
  std::condition_variable condition_;
  std::atomic<bool> closed_{false};
};

}  // namespace hhullen

#endif  // SRC_LIB_VAULT_ENGINE_CHANNEL_CHANNEL_H_
