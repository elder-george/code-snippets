#include <type_traits>
#include <string>
#include <string_view>


namespace details
{
    // iteration over the variadic template arguments.
    // Normal case:
    template<typename FuncT, typename HeadT, typename... ArgsT>
    void template_for_each(FuncT func, HeadT&& head, ArgsT&&... rest)
    {
        func(std::forward<HeadT>(head));
        template_for_each(func, rest...);
    }

    // Terminating case:
    template<typename FuncT>
    void template_for_each(FuncT) {}

    // This overload is optional: if it's not present, `strlen` is always called,
    // which is usually OK, but... Why not avoid that if length is known at compile-time?
    template<size_t N>
    std::string_view to_string_view(const char (&s)[N])
    {
        // C-style arrays have terminating '\0' that we don't need.
        return {s, N-1};
    }

    // the template specification is crafted to force compiler to look at this overload
    // *after* the previous one.
    // (borrowed here https://android.googlesource.com/platform/external/qemu/+/emu-master-dev/android/android-emu-base/android/base/StringView.h#106)
    template<typename CharT, typename = std::enable_if_t<std::is_same_v<CharT, char>>>
    std::string_view to_string_view(const CharT* const &s)
    {
        return s;
    }

    // implementation for string and string_view are trivial:
    template<typename = char>
    std::string_view to_string_view(const std::string& s){
        return s;
    }

    template<typename = char>
    std::string_view to_string_view(const std::string_view& s){
        return s;
    }
}


// concatenate string-like arguments with no more than one allocation:
template<typename... ArgsT>
std::string concat(ArgsT&&... args)
{
    using namespace details;
    // First, compute the total size of all strings combined:
    size_t totalSize = 0;
    template_for_each([&totalSize](auto&& arg) {
            using ArgT = std::remove_reference_t<decltype(arg)>;
            totalSize += to_string_view(std::forward<ArgT>(arg)).size();
        }, std::forward<ArgsT>(args)...);

    std::string result;
    // ... and allocate enough memory (+1 byte for '\0')
    result.reserve(totalSize + 1);

    // then iterate again and append each argument to the accumulator
    template_for_each([&result](auto&& arg) {
        using ArgT = std::remove_reference_t<decltype(arg)>;
        result += to_string_view(std::forward<ArgT>(arg));
    }, std::forward<ArgsT>(args)...);

    return result;
}

// Utilities for formatting
namespace details
{
    struct marker_info {
        std::string_view literal;
        int arg_number;
        std::string_view rest;
    };

    // this can be done a bit clearer with C++20, but I only have C++17 compiler installed right now
    constexpr marker_info find_marker(const std::string_view& format)
    {
        for (auto i = 0; i < format.size(); ++i)
        {
            if (format[i] == '%')
            {
                auto literal_end = i++;
                auto c = format[i++];
                if (c == '%')
                {
                    return { 
                            format.substr(0, literal_end + 1), 
                            -1,
                            format.substr(i)
                        };
                }
                else if ('0' <= c && c <= '9')
                {
                    return {
                            format.substr(0, literal_end), 
                            c - '0', 
                            format.substr(i)
                        };
                }
            }
        }
        return { format, -1, format.substr(0,0) };
    }

    template<typename Func>
    constexpr void foreach_marker(std::string_view fmt, Func func)
    {
        
        while(!fmt.empty())
        {
            auto [ literal, arg_number, rest ] = find_marker(fmt);
            func(literal, arg_number);
            fmt = rest;
        }
    }
    
    template<int N>
    std::string_view nth(int) {
        return "";
    }    

    template<int N = 0, typename ArgT, typename... ArgsT>
    std::string_view nth(int i, ArgT&& arg, ArgsT&&... args)
    {
        if (i == N)
        {
            return to_string_view(std::forward<ArgT>(arg));
        }
        return nth<N+1, ArgsT...>(i, std::forward<ArgsT>(args)...);
    }
}

// Simple formatting facility - replaces markers `%0`..`%9` with corresponding string-ish arguments.
// Doesn't do any advanced validation (but it's not hard to implement one).
// Only allocates memory once.
template<typename... ArgsT>
std::string format(const std::string_view& fmt, ArgsT&&... args)
{
    using namespace details;
    // compute length of all the strings, including substitutions.
    size_t total_length = 0;
    foreach_marker(fmt, [&total_length, &args...](auto&& s, int arg_id) {
        total_length += s.size();
        if (arg_id >= 0)
        {
            total_length += nth(arg_id, std::forward<ArgsT>(args)...).size();
        }
    });

    std::string result;
    result.reserve(total_length);

    // concatenate
    foreach_marker(fmt, [&result, &args...](auto&& s, int arg_id) {
        result.append(s);
        if (arg_id >= 0)
        {
            result.append(nth(arg_id, std::forward<ArgsT>(args)...));
        }
    });

    return result;
}

// overriding new/delete to verify no more than one allocation per `concat`/`format` is done
#include <cstdlib>

static int alloc_count = 0;
static int free_count = 0;

void* operator new(size_t sz)
{
    alloc_count++;
    return std::malloc(sz);
}
void* operator new[](size_t sz)
{
    alloc_count++;
    return std::malloc(sz);
}

void operator delete(void* p)
{
    free_count++;
    std::free(p);
}
void operator delete[](void* p)
{
    free_count++;
    std::free(p);
}

// simple test program
#include <cstdio>
#include <cassert>

int main()
{
    using namespace std::literals;
    {
        // using different kinds of literals here.
        // We want to get a string long enough for SSO not to apply.
        const char* bang = "!!!!";
        const std::string hello = "Hello"s;
        auto s = concat(hello," dear ", "world"sv, bang);

        std::fprintf(stderr, "%s\n", s.c_str());
    }

    assert(alloc_count == free_count);
    assert(alloc_count == 1);
    alloc_count = free_count = 0;

    {
        // using different kinds of literals here.
        // We want to get a string long enough for SSO not to apply.
        const char* bang = "!!!!";
        const std::string hello = "Hello"s;
        auto s = format("- %0, %1 %3%2...", hello, "dear", bang, "world"sv);

        std::fprintf(stderr, "%s\n", s.c_str());
    }
    assert(alloc_count == free_count);
    assert(alloc_count == 1);
}