/*
Implementing generators (a variant of coroutine) with macros
(based on [this article](https://www.codeproject.com/Tips/29524/Generators-in-C),
in turn, based on [this article](http://www.chiark.greenend.org.uk/~sgtatham/coroutines.html).

tl;dr: we are abusing the fact that `switch` allows to jump to a `case` label in the middle of the loop.
(cf. Duff device)
*/

#include <iterator>
#include <tuple>

namespace details {
    // dummy tag type to use for "are we done there yet?" checks
    struct sentinel{};

    // To be able to construct iterators with arguments, we are using template
    // specialization.
    template <typename SignatureT> struct iterator_base;

    // Note that iterator_base is incomplete:
    // it doesn't have `operator++` that is going to be implemented by
    // our macros in a subclass.
    template<typename ReturnT, typename... ArgsT>
    struct iterator_base<ReturnT(ArgsT...)> {
        using ArgsTuple = std::tuple<ArgsT...>;
        
        iterator_base(const ArgsTuple& args)
        : args_{args}
        {}

        const ReturnT& operator*() const & {
            return value_;
        }
        ReturnT&& operator*() && {
            return std::move(value_);
        }

        bool operator==(sentinel) const {
            return finished_;
        }
        bool operator!=(sentinel) const {
            return !finished_;
        }
        
    protected:
        // a helper function to access arguments
        template<size_t N>
        auto&& arg() const { return std::get<N>(args_); }

        const ArgsTuple args_;
        ReturnT value_{};
        int line_{};
        bool finished_{};
    };

    // this is a simple range implementation.
    // all the magic is done in the iterator implementation.
    template <typename IterT, typename TSignature>
    struct generator_base;

    // using template specialization to "unpack" the function signature
    template<typename IterT, typename ReturnT, typename... ArgsT>
    struct generator_base<IterT, ReturnT(ArgsT...)> {
        auto end() const {
            return sentinel{};
        }

        auto begin() const { 
            IterT it{args_}; // creating the iterator using the polymorphic staatic method.
            // first iteration of the generator will go to the first $yield
            return ++it;
        }

        generator_base(ArgsT&&... args)
        : args_{std::forward<ArgsT>(args)...}
        {}
    private:
        const std::tuple<ArgsT...> args_;
    };
}

// Declare a generator type corresponding to the `SIGNATURE`
// Since we really need nothing from it, it can be an alias to `generator_base`
// Note that `iterator_##NAME` doesn't have body.
// It is intentional: the following block will be used as the class body.
// This allows to "convert" the "variables" at the top into the iterator fields (i.e. state).
// However this also limits us in what we can put inside the class - we can only add things
// in the beginning of `$start` or end of `$stop`
#define $generator(NAME, SIGNATURE) using NAME = details::generator_base<struct iterator_##NAME, SIGNATURE>;\
\
class iterator_##NAME final: public details::iterator_base<SIGNATURE>

// Actual code starts here. Initial value of `line_` is 0, so this is the starting point.
// Note that we also import the base class constuctors here, to simplify the parameter passing.
#define $start public:                  \
    using iterator_base::iterator_base; \
    auto& operator++() {                \
        switch(line_) { case 0:;

// `$yield` statement pauses the execution by "saving" the position into `line_`
// and creating a label to return to.
// Normally, we would use the `do{}while(0)` trick to create a new scope.
// But we don't really have anything deserving a new scope.
// Note that we use `return` and not `break` because there's a *good* chance that
// `$yield` will be called inside a loop.
#define $yield(V)               \
            line_ = __LINE__;   \
            value_ = (V);       \
            return *this;       \
        case __LINE__:;

// finish execution by setting the flag
// We also provide the final label here, but there're other ways.
#define $stop                   \
        default: line_ = 0;     \
            finished_ = true;   \
            return *this;       \
        } /*end of switch*/     \
    }                           \
private:

// Demo time!
#include <cstdio>

// a generator emitting N first Fibonacci numbers
$generator(fib, int(int)) {
    int a = 1, b = 1;   // all these variable are going to be fields of the iterator
    int i;              // because of this, they need to be declared before $start (or after `$stop`)
    $start;
    for (i = 0; i < arg<0>(); ++i) {    // I wish I could figure a better way to access the arguments
        $yield(a);      // here the execution is stopped to be resumed on the next iteration.
        auto n = a + b;
        a = b; b = n;
    }
    $stop;
};  // note that we need a `;` here - otherwise the class declaration won't be finished and compiler will be sad.

int main()
{
    for (const auto& n : fib{10}) {
        std::fprintf(stderr, "%d\n", n);
    }
}
