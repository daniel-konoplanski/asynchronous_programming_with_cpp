#include <functional>
#include <iostream>
#include <mutex>
#include <print>
#include <ranges>
#include <sstream>
#include <syncstream>
#include <thread>
#include <unistd.h>
#include <utility>

using namespace std::chrono_literals;

void func()
{
    std::print("Using function pointer\n");
}

int raceCondition()
{
    std::thread t1(
        []()
        {
            for (int i = 0; i < 100; ++i)
            {
                std::ostringstream oss;

                oss << "1 " << "2 " << "3 " << "4 " << std::endl;

                std::cout << oss.str();
            }
        });

    std::thread t2(
        []()
        {
            for (int i = 0; i < 100; ++i)
            {
                std::ostringstream oss;

                oss << "5 " << "6 " << "7 " << "8 " << std::endl;

                std::cout << oss.str();
            }
        });

    t1.join();
    t2.join();

    return 0;
}

#define sync_cout std::osyncstream(std::cout)

void synced()
{
    std::thread t1(
        []()
        {
            for (int i = 0; i < 100; ++i)
            {
                sync_cout << "1 " << "2 " << "3 " << "4 " << std::endl;
            }
        });

    std::thread t2(
        []()
        {
            for (int i = 0; i < 100; ++i)
            {
                sync_cout << "5 " << "6 " << "7 " << "8 " << std::endl;
            }
        });

    t1.join();
    t2.join();
}


namespace {

int result = 0;

};

void setValue(int& result) {

    std::this_thread::sleep_for(1s);
    result = 1 + (rand () % 10);
}

void joinable()
{
    std::thread t1;

    std::cout << "Is t1 joinable? " << t1.joinable()
              << std::endl;

    std::thread t2([](){
        std::this_thread::sleep_for(100ms);
    });

    t1.swap(t2);

    std::cout << "Is t1 joinable? " << t1.joinable()
              << std::endl;

    std::cout << "Is t2 joinable? " << t2.joinable()
              << std::endl;

    t1.join();

    std::cout << "Is t1 joinable? " << t1.joinable()
              << std::endl;
}

void deamonThread()
{
    sync_cout << "starting deamon thread\n";

    std::this_thread::sleep_for(2s);

    sync_cout << "finishing deamon thread\n";
}

template <typename F, typename... Args>
class JThreadWrapper
{
public:
    JThreadWrapper(F&& func, Args&&... args)
        : threadId_{threadCounter}
    {
        sync_cout << "constructed thread with id: " << threadId_ << "\n";
        thread_ = std::jthread(std::forward<F>(func), std::forward<Args>(args)...);
        ++threadCounter;
    }

    ~JThreadWrapper()
    {
        sync_cout << "destoyed thread with id: " << threadId_ << "\n";
    }

private:
    const uint32_t threadId_{};
    std::jthread thread_{};
    static inline uint32_t threadCounter{ 1U };
};

std::mutex mtx{};

void yield_func()
{
    auto work = [&](const std::string& name)
    {
        while (true)
        {
            bool work_to_do = rand() % 2;

            if (work_to_do)
            {
                sync_cout << name << ": working\n";

                std::lock_guard<std::mutex> lock(mtx);

                for (auto start = std::chrono::steady_clock::now(), now = start; now < start + 3s;
                     now = std::chrono::steady_clock::now())
                {
                }
            }
            else
            {
                sync_cout << name << ": yielding\n";
                std::this_thread::yield();
            }
        }
    };

    std::jthread t1(work, "t1");
    std::jthread t2(work, "t2");
}

int main()
{
    std::thread t1(func);
    std::print("t1 id: {}\n", t1.get_id());
    t1.join();

    std::thread t2([]() { std::print("Using a lambda\n"); });
    std::print("t2 id: {}\n", t2.get_id());
    t2.join();

    auto processorCount = std::thread::hardware_concurrency();

    std::print("Current number of processors: {}\n", processorCount);

    // raceCondition();
    // synced();

    auto passBy = [](const std::string& string) { std::print("Hello from thread using string: \"{}\"\n", string); };

    auto t3 = std::thread(passBy, "pass by value");

    t3.join();

    std::string reference{ "pass by ref" };

    auto t4 = std::thread(passBy, std::cref(reference));

    t4.join();

    // Setting a value in a different thread
    std::thread t5(setValue, std::ref(result));

    t5.join();

    sync_cout << "Result: " << result << std::endl;

    auto t6 = std::thread([](){
        for (int i : std::views::iota(1, 100))
        {
            sync_cout << i << "\n";
        }
    });

    auto t7 = std::move(t6);    

    t7.join();

    //Check if threads are joinable
    joinable();

    auto t8 = std::thread(deamonThread);
    t8.detach();

    std::this_thread::sleep_for(3s);


    std::jthread t9([]() {
        sync_cout << "Hello from jthread\n";
    });

    t9.join();

    // Using the JThreadWrapper
    auto jt1 = JThreadWrapper([]() { sync_cout << "Hello from jt1\n"; });
    auto jt2 = JThreadWrapper([]() { sync_cout << "Hello from jt2\n"; });
    auto jt3 = JThreadWrapper([]() { sync_cout << "Hello from jt3\n"; });

    std::cout << "________________\n";
    //Using std::this_thread::yield
    yield_func();

    return 0;
}
