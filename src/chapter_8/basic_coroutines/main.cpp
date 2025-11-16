#include <coroutine>

struct return_type
{
    struct promise_type;

    ~return_type() noexcept = default;

    explicit return_type(promise_type&)
    {
    }

    struct promise_type
    {
        return_type get_return_object() noexcept
        {
            return return_type{ *this };
        }

        void return_void() noexcept
        {
        }

        std::suspend_always initial_suspend() noexcept
        {
            return {};
        }

        std::suspend_always final_suspend() noexcept
        {

            return {};
        }

        void unhandled_exception() noexcept
        {
        }
    };
};

return_type coro_func()
{
    co_return;
}

int main()
{
    coro_func();
}
