// See [explanation](https://en.wikipedia.org/wiki/X_Macro).
// There're two approaches to X-Macros:

// a) define data itself as a macro accepting "functor" and expand it with different "functors".

// clang-format off
#define TOKEN_TYPES(val)\
    val(' ', Whitespace,"%255[ \t\r\n]%n")\
    val('(', ParenLeft, "%1[(]%n")\
    val(')', ParenRight,"%1[)]%n")\
    val(',', Comma,     "%1[,]%n")\
    val('=', Assign,    "%1[=]%n")\
    val('+', Plus,      "%[+]%n")\
    val('-', Minus,     "%1[-]%n")\
    val('*', Asterisk,  "%1[*]%n")\
    val('/', Slash,     "%1[/]%n")\
    val('^', Caret,     "%1[\\^]%n")\
    val('~', Tilde,     "%1[~]%n")\
    val('!', Bang,      "%1[!]%n")\
    val('?', Question,  "%1[?]%n")\
    val(':', Colon,     "%1[:]%n")\
    val('\0', Name,     "%255[a-zA-Z_]%n")\
    val('\0', Number,   "%255[0-9]%n")\
    val('\0', Eof,      nullptr)
// clang-format on

// Generate `enum` itself
#define ENUM_VAL(_, V, ...) V,
enum class TokType : int
{
    TOKEN_TYPES(ENUM_VAL)
        _Count,
};
constexpr size_t TokType_Count = (size_t)TokType::_Count;
#undef ENUM_VAL

// generate strings (e.g. for debug output)
#define ENUM_STR(S, _, ...) S,
constexpr char TokType_Chars[TokType_Count] = {
    TOKEN_TYPES(ENUM_STR)};
#undef ENUM_STR

// generate an array of patterns to be used by tokenizer
#define TOKEN_PATTERN(N, V, P) P,
constexpr const char *const TokPatterns[TokType_Count] = {
    TOKEN_TYPES(TOKEN_PATTERN)};
#undef TOKEN_PATTERN

/* b) put data in separate file and include it multiple times
 Something like:
 file: tokens.inc
 ```
    // note lack of include-guard
    // with a separate file one doesn't need to put `\` at the end of line
    val(' ', Whitespace, %255[ \t\r\n]%n")
    val('(', ParenLeft, "%1[(]%n")
    val(')', ParenRight,"%1[)]%n")
    val(',', Comma,     "%1[,]%n")
    val('=', Assign,    "%1[=]%n")
    val('+', Plus,      "%[+]%n")
    val('-', Minus,     "%1[-]%n")
    val('*', Asterisk,  "%1[*]%n")
    val('/', Slash,     "%1[/]%n")
    val('^', Caret,     "%1[\\^]%n")
    val('~', Tilde,     "%1[~]%n")
    val('!', Bang,      "%1[!]%n")
    val('?', Question,  "%1[?]%n")
    val(':', Colon,     "%1[:]%n")
    val('\0', Name,     "%255[a-zA-Z_]%n")
    val('\0', Number,   "%255[0-9]%n")
    val('\0', Eof,      NULL)
```
file: tokens.cpp
```
// Generate `enum` itself
#define ENUM_VAL(_, V, ...) V,
#include "tokens.inc"
#undef ENUM_VAL
// etc.
```
*/

// Example of use:
#include <string>

struct Token
{
    TokType type;
    std::string text;
};

struct Lexer
{
    Lexer(std::string &&s)
        : contents{s}, it{s.begin()}
    {
    }

    Token next()
    {
        for (auto i = 0u; it != contents.end() && i < TokType_Count; ++i)
        {

            int bytes_read = 0;
            char buf[256] = {0};
            auto pattern = TokPatterns[i];
            if (!pattern)
                continue;
            // using sscanf as a "poor man's regex engine"
            if (std::sscanf(&*it, pattern, buf, &bytes_read))
            {
                it += bytes_read;
                if ((TokType)i == TokType::Whitespace)
                {
                    continue;
                }
                auto token = Token{(TokType)i, buf};
                return token;
            }
        }
        return Token{TokType::Eof, ""};
    }

private:
    std::string contents;
    std::string::iterator it;
};