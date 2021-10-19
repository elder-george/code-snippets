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

- using `_com_error` to format message from `HRESULT`, and `HRESULT_FROM_WIN32` to get `HRESULT` from Win32 error code
;
- (last but not least) using new standard feature - `std::system_category().message(...)`;