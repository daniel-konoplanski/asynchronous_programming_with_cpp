#include <exception>
#include <memory>
#include <mutex>
#include <thread>
#include <typeinfo>
#include <iostream>
#include <syncstream>

#define sync_cout std::osyncstream(std::cout)

using namespace std::chrono_literals;

std::exception_ptr exception_ptr{};
std::mutex mtx{};

void some_throw()
{
    throw std::bad_typeid();
}

int main()
{
    auto worker1 = std::jthread(
        []()
        {
            try
            {
                some_throw();
            }
            catch (const std::exception& ex)
            {
                sync_cout << "Catched: " << ex.what() << "\n";
                std::lock_guard<std::mutex> lg{mtx};
                exception_ptr = std::current_exception();
            }
        });

    std::this_thread::sleep_for(2s);

    try
    {
        std::rethrow_exception(exception_ptr);
    }
    catch (const std::exception& ex)
    {
        sync_cout << "Catched in main thread: " << ex.what() << "\n";
    }

    return 0;
}