#pragma once

#include <chrono>
#include <concepts>
#include <functional>
#include <mutex>
#include <thread>

/*
 *  As an exercise, you can slightly modify this example to implement a timeout class that calls a callback function if
 *  there is no input event within a given timeout interval. This is a common pattern when dealing with network
 *  communications where a packet replay request is sent if no packets have been received for some time.
 */

namespace timeout
{

template <typename F, typename... Args>
requires std::invocable<F, Args...>
class Timeout
{
public:
    Timeout(F&& callback, Args&&... args);

public:
    void start(std::chrono::microseconds interval);
    void handle();
    void stop();

private:
    mutable std::mutex mtx_{};
    bool event_received{false};
    using R = std::invoke_result_t<F, Args...>;
    std::function<R(Args...)> callback_;
    std::jthread worker_;
};

} // namespace timeout

#include "Timeout.inl"