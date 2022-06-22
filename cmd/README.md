# .BAT/.CMD snippets and scripts

## uses.cmd

Validates that all the command names passed as args can be resolved using `PATH`.

Kinda like `require`/`import` statement in some languages, but actually doesn't import anything, only fails if a command can not be found. 

Usage:

    @call uses grep cut :: at the top of the script.

Note: The line above is handy for debugging problems, but also makes scripts somewhat slower.

### Illustrates

- `for /d` syntax;

- `where` command;

- redirecting stderr and stdout to NUL;

## for_loop_flavors.cmd

Examples of `for` loop use

### Illustrates

- arithmetic `for` (`for/l`)

- use of `for` for traversing folders (`for`, `for/d`, `for/r`)

- "swiss-army knife" `for/f` - iterating, processing paths, using with strings

## vars.cmd

Using variables

### Illustrates

- setting variables

- string operations (including hack for "dictionary lookup")

- indirect access (access by name)

- functions, returning value and defining scope

## wrapping_scripts.cmd

How to wrap "real" languages into a .cmd file

### Illustrates

- implementing it for Python, Perl, Ruby, Node.js (badly), and Powershell

## ansi_term.cmd

Using ANSI terminal commands from `cmd`

### Illustrates

- a trick to save the `ESC` character to a variable (will also work with some other strings);

- using ANSI terminal commands to move cursor, change text and background color, switch between text and block graphics mode;

- outputting text without newline printed;

## dotenv.cmd

Executing a command with environment read from a file

### Illustrates

- starting a command in a new shell;

- reading a list of key-value pairs from a file;

- using `/s` key of `cmd` to avoid problems with quoting;
