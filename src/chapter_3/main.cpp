#include <iostream>
#include <print>
#include <ranges>
#include <sstream>
#include <syncstream>
#include <thread>

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

    return 0;
}
