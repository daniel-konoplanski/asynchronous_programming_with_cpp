#include <stop_token>
#include <thread>
#include <syncstream>
#include <iostream>

#define sync_cout std::osyncstream(std::cout)

using namespace std::chrono_literals;

template <typename T>

void show_stop_props(std::string_view name, const T& stop_item)
{
    sync_cout << std::boolalpha << name << ": stop_possible = " << stop_item.stop_possible()
              << ", stop_requested = " << stop_item.stop_requested() << '\n';
};

void func_with_stop_token(std::stop_token stop_token)
{
    for (int i = 0; i < 10; ++i)
    {
        std::this_thread::sleep_for(300ms);

        if (stop_token.stop_requested())
        {
            sync_cout << "stop_worker: Stopping as requested\n";
            return;
        }

        sync_cout << "stop_worker: Going back to sleep\n";
    }
}

void thread_stopper_example()
{
    auto worker1 = std::jthread(func_with_stop_token);
    auto ssource = worker1.get_stop_source();

    auto stop_worker1 = std::jthread([](std::stop_source ssource) {
        std::this_thread::sleep_for(1.2s);
        ssource.request_stop();
    },
    ssource);
}

void thread_stopper_callback_example()
{
    auto worker1 = std::jthread(func_with_stop_token);
    auto worker1_token = worker1.get_stop_token();

    std::stop_callback callback{worker1_token, [](){
        sync_cout << "Stop token was called\n";
    }};

    std::this_thread::sleep_for(1s);

    worker1.request_stop();

    std::this_thread::sleep_for(1s);
}

int main(int, char*[])
{
    // auto worker1 = std::jthread(func_with_stop_token);
    // std::stop_token stop_token = worker1.get_stop_token();
    // show_stop_props("stop_token", stop_token);

    // std::this_thread::sleep_for(1s);

    // worker1.request_stop();

    // std::this_thread::sleep_for(1s);
    // show_stop_props("stop_token", stop_token);

    // _______________________

    // sync_cout << "________________________\n";

    // auto worker2 = std::jthread(func_with_stop_token);

    // std::stop_source stop_source = worker2.get_stop_source();

    // show_stop_props("stop_source", stop_source);

    // auto stopper = std::thread(
    //     [](std::stop_source source)
    //     {
    //         std::this_thread::sleep_for(500ms);

    //         sync_cout << "Request stop for worker2 via source\n";

    //         source.request_stop();
    //     },
    //     stop_source);

    // stopper.join();

    // std::this_thread::sleep_for(200ms);

    // show_stop_props("stop_source after request", stop_source);

    // _______________________

    // auto worker3 = std::jthread(func_with_stop_token);
    // // auto worker2 = std::jthread(func_with_stop_token);

    // std::stop_callback callback(
    //     worker3.get_stop_token(),
    //     []()
    //     {
    //         sync_cout << "stop_callback for worker1 "
    //                   << "executed by thread " << std::this_thread::get_id() << '\n';
    //     });

    // sync_cout << "main_thread: " << std::this_thread::get_id() << '\n';

    // std::this_thread::sleep_for(1s);

    // worker3.request_stop();

    // std::this_thread::sleep_for(2s);

    // thread_stopper_example();
    thread_stopper_callback_example();

    return 0;
}