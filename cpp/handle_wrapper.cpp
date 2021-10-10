#include <memory>
#include <cstdio> // for examples to run
#include <string> // ditto

// A lot of libraries expose C-style API operating on opaque pointers (AKA handles).
// Such code is leak-prone unless RAII is enforced.
// Here're utilities providing slightly different ways for that

// Quick&Dirty way: use std::unique_ptr:
template <typename T, typename TDeleter>
std::unique_ptr<T, TDeleter> wrap(T *pointer, TDeleter deleter)
{
    return {pointer, deleter};
}

// More thorough way:
// (the main difference is that users don't need to call .get())
template <typename THandle, typename Deleter>
struct Handle
{
    std::unique_ptr<THandle, Deleter> _ptr;
    Handle(THandle *h, Deleter d)
        : _ptr{h, d}
    {
        // we may also want to validate the pointer before assigning it.
    }

    operator THandle *()
    {
        return _ptr.get();
    }
};
// Before C++14 this function helps to avoid specifying types.
// With C++17 or later it's not needed thanks to the Template Argument Deduction
template <typename THandle, typename Deleter>
auto wrapHandle(THandle *h, Deleter d)
{
    return Handle<THandle, Deleter>{h, d};
}

// It's also much easier to make better wrappers later
#include <type_traits>
struct FileHandle : public Handle<FILE, std::add_pointer<decltype(std::fclose)>::type>
{
    static constexpr char *READONLY = "r";
    static constexpr char *WRITEONLY = "w";
    // etc

    FileHandle(const std::string &path, char *access)
        : Handle{std::fopen(path.data(), access), &std::fclose}
    {
    }

    template <size_t N>
    void get_string(char (&buf)[N])
    {
        std::fgets(buf, N, *this);
    }
};

// Semi-related: scope quard aka `defer`
// std::experimental::scope_exit (in <experimental/scope>) is a better option though.
template <typename TFunc>
struct scope_guard
{
    TFunc f;
    scope_guard() = delete;
    scope_guard(const scope_guard &) = delete;
    scope_guard &operator=(const scope_guard &) = delete;

    ~scope_guard()
    {
        f();
    }
};
// deduction guide for C++17 and later
template <class TFunc>
scope_guard(TFunc) -> scope_guard<TFunc>;
// a function for C+14
template <typename TFunc>
scope_guard<TFunc> on_scope(TFunc &&f)
{
    return scope_guard<TFunc>{f};
}

int main()
{
    using namespace std;
    // option 1: unique_ptr
    {
        auto file = wrap(fopen("README.md", "r"), fclose);
        char buf[1024];
        fgets(buf, sizeof(buf), file.get()); // notice ugly `.get()` here
        fprintf(stderr, "%s\n", buf);
    } // file is closed on exit

    // option 2: actual wrapper
    {
        auto file = Handle(fopen("README.md", "r"), fclose);
        char buf[100];
        fgets(buf, size(buf), file); // no `.get()`!
        fprintf(stderr, "%s\n", buf);
    }
    // And now we can build a semi-decent class on top of the wrapper.
    {
        auto file = FileHandle("README.md", FileHandle::READONLY);
        char buf[100];
        file.get_string(buf);
        fprintf(stderr, "%s\n", buf);
    }
    // option 3: scope_guard
    {
        auto file = fopen("README.md", "r");
        auto close_file = scope_guard{[=] { std::fclose(file); }};
        char buf[100];
        fgets(buf, size(buf), file);
        fprintf(stderr, "%s\n", buf);
    }
}