#pragma once

#include "Timer.hpp"

#include <functional>
#include <iostream>
#include <stop_token>
#include <syncstream>
#include <thread>

namespace timer
{

template <typename F, typename... Args>
    requires std::invocable<F, Args...>
Timer<F, Args...>::Timer(F&& callback, Args... args)
    : callback_{ std::bind_front(callback, args...) }
{
}

template <typename F, typename... Args>
    requires std::invocable<F, Args...>
void Timer<F, Args...>::start(std::chrono::microseconds interval)
{
    std::osyncstream(std::cout) << "Interval set to " << interval << '\n';

    worker_ = std::jthread(
        [this, interval](std::stop_token stop_token)
        {
            std::osyncstream(std::cout) << "Starting timer\n";

            while (!stop_token.stop_requested())
            {
                std::this_thread::sleep_for(interval);
                std::osyncstream(std::cout) << "Calling callback\n";
                callback_();
            }
        });
}

template <typename F, typename... Args>
    requires std::invocable<F, Args...>
void Timer<F, Args...>::stop()
{
    std::osyncstream(std::cout) << "Stopping timer\n";
    worker_.request_stop();
}

} // namespace timer
