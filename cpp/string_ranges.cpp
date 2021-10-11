#include <string_view>
#include <cstring>

// C++ makes it notoriously hard to split a string or string view into segments.
// This is an example using `strpbrk` for delimiting.
// Can be easily updated to use `strchr`, `strstr`, or `find`, say.
struct split_range
{
    split_range(const std::string_view &s, const char *delims)
        : s_(s), delims_(delims)
    {
    }

    struct sentinel_iterator
    {
    };

    struct iterator
    {
        iterator(const std::string_view &s, const char *delims)
            : delims_{delims}, begin_{s.data()}, end_{strpbrk(begin_, delims_)}, end_of_string_{s.data() + s.size()}
        {
            if (!end_)
            {
                end_ = end_of_string_;
            }
        }
        iterator &operator++()
        {
            begin_ = end_ + 1;
            end_ = strpbrk(end_ + 1, delims_);
            if (!end_)
            {
                end_ = end_of_string_;
            }
            return *this;
        }
        std::string_view operator*() const
        {
            return {begin_, static_cast<size_t>(end_ - begin_)};
        }
        bool operator!=(sentinel_iterator) const
        {
            return begin_ < end_of_string_;
        }

        //private:
        const char *delims_;
        const char *begin_;
        const char *end_;
        const char *end_of_string_;
    };

    iterator begin() const
    {
        return iterator{s_, delims_};
    }
    sentinel_iterator end() const { return {}; }

private:
    std::string_view s_;
    const char *delims_;
};

// Another ugly recurring problem is iterating over the regex matches.
// (std::regex may be deprecated anyway (?), so maybe not such a big deal)
#include <regex>

struct regex_range
{
    regex_range(const std::regex &regex, const std::string_view &s)
        : begin_{s.data(), s.data() + s.size(), regex}, end_{}
    {
    }
    // preventing call with a regex as a temporary value
    regex_range(std::regex &&regex, const std::string_view &s) = delete;

    auto begin() const { return begin_; }
    auto end() const { return end_; }

private:
    std::cregex_iterator begin_;
    std::cregex_iterator end_;
};

#include <iostream>

// use:
int main()
{
    // splitting
    {
        for (const auto &sv : split_range{"   1:2, 3:10, 11:20", ",:"})
        {
            std::cout << sv << "\n";
        }
    }
    // matching
    {
        std::regex rx_pair{"\\d+:\\d+"};
        for (const auto &m : regex_range{rx_pair, "   1:2, 3:10, 11:20"})
        {
            std::cout << m.str() << "\n";
        }
    }
}