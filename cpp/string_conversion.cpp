#include <string>
#include <iostream>
#include <string_view>

namespace string_convert
{
    // from https://dbj.org/c17-codecvt-deprecated-panic/
    /* 
    Transform any std string or string view
    into any of the 4 the std string types,
    Apache 2.0 (c) 2018 by DBJ.ORG
    */
    template <typename T, typename F>
    T transform_to(F str) noexcept
    {
        // note: F has to have the empty()method
        if (str.empty())
            return {};
        // note: F must be able to work with std begin and end
        return {std::begin(str), std::end(str)};
        // also the above line requires, T has a constructor
        // that will take begin and end values of type F.
    }

    template <typename T, typename F, size_t N>
    T transform_to(const F (&str)[N]) noexcept
    {
        // there is nothing to transform
        if constexpr (N < 1)
        {
            return {};
        }
        else
        {
            // else transform and return
            return {std::begin(str), std::end(str)};
        }
    }
} // namespace string_convert

// Use:
int main()
{
    using namespace std::literals;
    using namespace string_convert;

    std::wcout << transform_to<std::wstring>("hello world"sv);

    std::cout << transform_to<std::string>(u"hello world");
}