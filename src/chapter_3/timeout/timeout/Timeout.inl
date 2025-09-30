#pragma once

#include "Timeout.hpp"

#include <functional>
#include <iostream>
#include <mutex>
#include <stop_token>
#include <syncstream>
#include <thread>
#include <utility>

namespace timeout
{

template <typename F, typename... Args>
    requires std::invocable<F, Args...>
Timeout<F, Args...>::Timeout(F&& callback, Args&&... args)
    : callback_{ std::bind_front(callback, std::forward(args)...) }
{
}

template <typename F, typename... Args>
    requires std::invocable<F, Args...>
void Timeout<F, Args...>::start(std::chrono::microseconds interval)
{
    std::osyncstream(std::cout) << "Event interval set to " << interval << '\n';

    worker_ = std::jthread(
        [this, interval](std::stop_token stop_token)
        {
            std::osyncstream(std::cout) << "Starting timer\n";

            while (!stop_token.stop_requested())
            {
                std::this_thread::sleep_for(interval);
                std::lock_guard<std::mutex> lg{ mtx_ };

                if (!event_received)
                {
                    std::osyncstream(std::cout) << "No event received - calling callback\n";
                    callback_();
                }

                event_received = false;
            }
        });
}

template <typename F, typename... Args>
    requires std::invocable<F, Args...>
void Timeout<F, Args...>::handle()
{
    std::osyncstream(std::cout) << "Received event to handle\n";
    std::lock_guard<std::mutex> lg{mtx_};
    event_received = true;
}


template <typename F, typename... Args>
    requires std::invocable<F, Args...>
void Timeout<F, Args...>::stop()
{
    std::osyncstream(std::cout) << "Stopping timer\n";
    worker_.request_stop();
}

} // namespace timer
