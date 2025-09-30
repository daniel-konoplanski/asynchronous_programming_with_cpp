#pragma once

#include <chrono>
#include <concepts>
#include <functional>
#include <thread>

namespace timer
{

template <typename F, typename... Args>
requires std::invocable<F, Args...>
class Timer
{
public:
    Timer(F&& callback, Args&&... args);

public:
    void start(std::chrono::microseconds interval);
    void stop();

private:
    using R = std::invoke_result_t<F, Args...>;
    std::function<R(Args...)> callback_;
    std::jthread worker_;
};

} // namespace timer

#include "Timer.inl"
