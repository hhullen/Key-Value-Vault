#ifndef SRC_TIMER_H_
#define SRC_TIMER_H_

#include <chrono>
#include <string>

using std::sprintf;
using std::to_string;

namespace hhullen {

using Str = std::string;
using Clock = std::chrono::steady_clock;
using TimePoint = std::chrono::time_point<Clock>;
using Ms = std::chrono::milliseconds;
using Sec = std::chrono::seconds;

class DTime {
 public:
  DTime(const long long duration) : elapsed_milliseconds_(duration) {}
  long long InMs() { return elapsed_milliseconds_; }
  long long InSec() { return elapsed_milliseconds_ / 1'000; }
  int InMin() { return static_cast<int>(elapsed_milliseconds_ / 60'000); }
  int InHours() { return static_cast<int>(elapsed_milliseconds_ / 3'600'000); }
  int InDays() { return static_cast<int>(elapsed_milliseconds_ / 86'400'000); }

  long long Ms() { return elapsed_milliseconds_ % 1000; }
  long long Sec() { return elapsed_milliseconds_ % 60'000 / 1000; }
  int Min() {
    return static_cast<int>(elapsed_milliseconds_ % 3'600'000 / 60'000);
  }
  int Hours() {
    return static_cast<int>(elapsed_milliseconds_ % 86'400'000 / 3'600'000);
  }
  int Days() { return InDays(); }

  Str SMs(const unsigned char width = 3) { return GetFormatted(Ms(), width); }
  Str SSec() { return GetFormatted(Sec(), 2); }
  Str SMin() { return GetFormatted(Min(), 2); }
  Str SHours() { return GetFormatted(Hours(), 2); }
  Str SDays() { return to_string(Days()); }

 private:
  long long elapsed_milliseconds_;

  Str GetFormatted(const long long value, const unsigned char width) {
    Str str(width, '0');
    sprintf(str.data(), Str("%0" + to_string(width) + "d").data(), value);
    return Str(str);
  }
};

class Timer {
 public:
  Timer() : start_(Clock::now()) {}
  Timer(const Timer& other) = delete;
  Timer(const Timer&& other) = delete;

  void Reset() { start_ = Clock::now(); }
  DTime Elapsed() {
    long long elapded =
        std::chrono::duration_cast<Ms>(Clock::now() - start_).count();
    return DTime(elapded);
  }

  long long TimepointMs() {
    return std::chrono::time_point_cast<Ms>(Clock::now())
        .time_since_epoch()
        .count();
  }

  long long TimepointSec() {
    return std::chrono::time_point_cast<Sec>(Clock::now())
        .time_since_epoch()
        .count();
  }

 private:
  TimePoint start_;
};

}  // namespace hhullen

#endif  // SRC_TIMER_H_
