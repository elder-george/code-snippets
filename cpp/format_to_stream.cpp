#include <iostream>

// format arbitrary `std::tuple`
// (obviously, elements need to be formattable too)
#include <tuple>

template<typename... TArgs>
std::ostream& operator<<(std::ostream& os, const std::tuple<TArgs...>& t) {
    std::apply(
        [&os](TArgs const&... tupleArgs)
        {
            os << '(';
            std::size_t n{0};
            ((os << (n++ ? ", " : "") << tupleArgs), ...);
            os << ')';
        }, t);
    return os;
}

// Can also be used for pair
template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& t) {
    return (os << std::tuple(t));
}

// formatting any range (i.e. "thing supported by `std::begin()` and `std::end()`")
// ...except strings (`std::string`, string literals or C-strings),
// otherwise compiler will have a conflict.
#include <iterator>     // for `std::begin`, `std::end`
#include <type_traits>  // for `std::enable_if`
#include <utility>      // for `std::declval`

// disabling for types that
// (a) aren't supported by `std::begin` or
// (b) have iterator accessing `char`
// (`const char&` is only needed by MSVC, others are happy with just `char`)
template<typename T>
std::enable_if_t<
    !std::is_same_v<const char&, 
                    decltype(*std::begin(std::declval<T>()))>,
    std::ostream&> 
operator<<(std::ostream& os, const T& container) {
    bool isFirst = true;
    os << '[';
    for (const auto& item : container) {
        os << (isFirst ? "" : ", ") << item;
        isFirst = false;
    }
    os << ']';

    return os;
}
