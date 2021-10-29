:: # Wrapping other scripts in a .CMD script
:: It's often beneficial to wrap a script in a "real language" in a cmd (easier to use, you can specify particular interpreter, etc.).
goto :eof

:: ## Python
:: Python has special support for that through `-x` parameter that skips 1st line.
@SETLOCAL ENABLEDELAYEDEXPANSION & @python -x %~f0 %* && exit /B !ERRORLEVEL!
print('hello from python')

:: ## Ruby has similar parameter, but also requires a shebang as the 2nd line
@SETLOCAL ENABLEDELAYEDEXPANSION & @ruby -x %~f0 %* & exit /B !ERRORLEVEL!
#!ruby
print "hello"

:: ## Perl has it too (probably was the first to implement it)
@SETLOCAL ENABLEDELAYEDEXPANSION & @perl -w -x %0 %* & exit /B !ERRORLEVEL!
#!perl -w
use strict;
print "Hello";

:: Node.js doesn't have it, so the wrapper relies on a hack;
:: it's suboptimal: first, `rem` is printed (can be circumvented with `cmd /q`, but it's ugly)
rem='';/*
@SETLOCAL ENABLEDELAYEDEXPANSION & node %0 & exit/b !ERRORLEVEL! ::*/
console.log('hello');

:: Powershell version is also a hack:
@(set ".=" ^)#) & powershell -NoLogo -nop -c "iex (Get-Content -Raw %~f0)" %* & exit/b
write-host "hello"

