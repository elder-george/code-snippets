# C++ snippets

## string_conversion.cpp

A quick&dirty conversion between different string and string_view variants, borrowed [here](https://dbj.org/c17-codecvt-deprecated-panic/)

Note: this may not necessarily work well with actual internationalized text - use ICU (or equivalent) instead.

### Illustrates

- conversion

- template specialization

- ridiculous syntax C++ has for functions accepting array literals.

## handle_wrapper.cpp

Ways of wrapping handles/opaque pointers into RAII:

- simple `unique_ptr`;

- somewhat smarter wrapper (which can be inherited too, for better interface!)

- `scope_guard` (but see also `std::experimental::scope_exit`);

### Illustrates

- `unique_ptr` + custom deleter

- non-copyable classes

- RAII;

- deduction guides;

## x_macros.cpp

(Ab)using [X Macros](https://en.wikipedia.org/wiki/X_Macro) to avoid code repetition etc.

### Illustrates

- X Macros (two approaches);

- `scanf`-as-a-regex-engine;

- simple tokenizer based on these two.

## string_ranges.cpp

Defining custom ranges for splitting or regex-matching strings

### Illustrates

- Custom ranges with sentinels;

- use of CRT `strpbrk` function (can be replaced with `string_view::find_first_of`);

- `std::regex` and `std::regex_iterator`;

- deleting a constructor accepting a temporary value;

## format_win_error.h

Different ways of getting description for Win32 errors.

### Illustrates

- use for `FormatMessage` function to allocated buffer and format message in single call;

- using macros to wrap it;

- wrapping pointer to buffer in `unique_ptr`;

- using `_com_error` to format message from `HRESULT`, and `HRESULT_FROM_WIN32` to get `HRESULT` from Win32 error code;

- (last but not least) using new standard feature - `std::system_category().message(...)`;

## format_to_stream

How to format arbitrary tuples and collections for output into `std::ostream`.

### Illustrates

- implementng `operator<<`

- variadic templates and "mapping" types (with side-effects) using fold-expressions;

- SFINAE and `std::enable_if`

## format_to_string

Concatenating different flavors of strings (`std::string`, `std::string_view`, C-style string, string literals) into a single string with no more than one allocation.

Formatting (similar to `QString::arg`) that replaces `%0`..`%9` markers with corresponding arguments, again, with no more than one allocation.

### Illustrates

- variadic templates and iterating over the types (with side-effects) using recursion;

- SFINAE and making compiler distinguish `const char*` and `const char[N]`;

- using `std::forward`;

- replacing global `new` and `delete` operators;

## debugging

Getting stack traces.

### Illustrates

- use of the `backward` library for producing stack traces;

## generators

Creating generators (~coroutines) with macros (based on [this article](https://www.codeproject.com/Tips/29524/Generators-in-C).

### Illustates

- making state machines using `switch` statement;

- what kind of abominable code macros allow to create;

- template specialization and how to use it to "unpack" function signature;

- custom iterators and ranges (see also `string_ranges`);