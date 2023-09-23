#ifndef SRC_LIB_VAULT_ENGINE_CHANNEL_CHANNEL_H_
#define SRC_LIB_VAULT_ENGINE_CHANNEL_CHANNEL_H_

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>

namespace hhullen {

template <class Type>
class Channel {
 public:
  ~Channel() {
    Close();
    condition_.notify_one();
  }

  operator bool() const { return !closed_; }

  void Send(Type item) {
    std::unique_lock<decltype(mutex_)> locker(mutex_);
    buffer_.emplace(item);
    condition_.notify_one();
  }

  Type Get() {
    std::unique_lock<decltype(mutex_)> locker(mutex_);
    condition_.wait(locker, [this]() { return !buffer_.empty(); });
    if (!closed_) {
      Type item = buffer_.front();
      buffer_.pop();
      return item;
    }
    return Type();
  }

  void Close() {
    closed_ = true;
    buffer_.emplace(Type());
    condition_.notify_one();
  }

 private:
  std::queue<Type> buffer_;
  std::mutex mutex_;
  std::condition_variable condition_;
  std::atomic<bool> closed_{false};
};

}  // namespace hhullen

#endif  // SRC_LIB_VAULT_ENGINE_CHANNEL_CHANNEL_H_
