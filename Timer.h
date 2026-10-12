#ifndef TIMER_H
#define TIMER_H

#include <chrono>

// Simple stopwatch based on the monotonic clock.
class Timer {
    using Clock = std::chrono::steady_clock;

public:
    Timer() : start_(Clock::now()) {}
    void restart() { start_ = Clock::now(); }
    // Seconds since construction or the last restart().
    double elapsed() const {
        return std::chrono::duration<double>(Clock::now() - start_).count();
    }

private:
    Clock::time_point start_;
};

#endif
