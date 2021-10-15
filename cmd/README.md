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
