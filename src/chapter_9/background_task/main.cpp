#include <boost/asio.hpp>
#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/system/detail/error_code.hpp>

#include <functional>
#include <print>
#include <thread>
#include <iostream>

using namespace std::chrono_literals;

void backgroundTask(boost::asio::io_context& ioContext)
{
    std::print("Posting background task\n");

    auto performWork = []() {
        std::print("Doing some work\n");
    };

    boost::asio::post(ioContext, performWork);
}

// int main()
// {
//     boost::asio::io_context ioContext{};

//     auto jt1 = std::jthread(
//         [](boost::asio::io_context& io) {
//             backgroundTask(io);
//         },
//         std::ref(ioContext));

//     auto jt2 = std::jthread(
//         [](boost::asio::io_context& io) {
//             std::print("Adding worker guard\n");
//             auto workGuard = boost::asio::make_work_guard(io);
//             std::this_thread::sleep_for(10s);
//             std::print("Resetting worker guard\n");
//             workGuard.reset();
//         },
//         std::ref(ioContext));

//     ioContext.run();

//     return 0;
// }

int main()
{
    boost::asio::io_context io_context;

    auto work_guard = boost::asio::make_work_guard(
        io_context);

    std::thread io_thread([&io_context]() {
        std::cout << "Running io_context.\n";
        io_context.run();
        std::cout << "io_context stopped.\n";
    });

    std::thread worker(
        backgroundTask,
        std::ref(io_context));

    // Main thread doing some work.
    std::this_thread::sleep_for(5s);
    std::cout << "Removing work_guard." << std::endl;

    work_guard.reset();
    worker.join();
    io_thread.join();

    return 0;
}