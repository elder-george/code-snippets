# PowerShell scripts and functions

## Convert-WithRegex

Takes regex and parses every line in input. If matches, creates an object with names corresponding to the groups in the regex.

### Illustrates

- Simplified syntax for `filter` functions;

- Use of regular expressions;

- Comment-Based Help.

## Import-DoskeyMacros.ps1

Converts macros created with `doskey.exe` into PowerShell functions.

Does only very rudimentary conversion so far.

### Illustrates

- PowerShell scopes

- Difference between executing and sourcing a script (and how to distinguish them in runtime)

- Creating functions on-the-fly
