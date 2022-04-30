// `defer` macro (https://youtu.be/QpAhX-gsHMs?t=1075)

#define macro_var(name) name##__LINE__
#define defer(start, end) for (      \
    int macro_var(_i_) = (start, 0); \
    !macro_var(_i_);                 \
    (++macro_var(_i_), (end)))

// Usage:
// defer(begin(), end())
// {
//    ...
// }
//
// e.g.
// #define profle defer(profile_begin(), profile_end())
// profile
// {
//    ...
// }

#include <stdio.h>

#define with_file(name, ...) \
    FILE *name;              \
    defer(name = fopen(__VA_ARGS__), fclose(name))

void use_with_file()
{
    with_file(txt, "macros.cpp", "r")
    {
        char buf[256];
        while (fgets(buf, sizeof(buf), txt))
        {
            printf("%s", buf);
        }
    }
}

int main()
{
    use_with_file();
}