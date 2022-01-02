// Producing stacktrace:
// Grab [this single-header library](https://github.com/bombela/backward-cpp)
#include "backward.hpp"

// If you want to use asserts or just handle good old signals, additionally add
namespace backward
{
    backward::SignalHandling sh;
}
// to set up all the hooks.

