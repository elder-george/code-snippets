@echo off
:: Windows' terminal got better support for ANSI escape-sequences, but `cmd` isn't good with them
:: At the very least, it's not trivial to output the `ESC` (ASCII 27 or 0x1b) character.
:: Here's a hack found on `ss64.com`
:: (the same hack can be used for other things expanded by the `PROMPT` - backspace, cr/lf etc.)
for /f %%l in ('echo PROMPT $E ^| cmd') do (set "ESC=%%l")

:: the fun ensues!
:: switching between block graphics mode and text mode
set "LINE_MODE=%ESC%(0"
set "TEXT_MODE=%ESC%(B"
:: set frame color to bright yellow
call :echo_no_newline %ESC%[93m
:: in the block graphics mode, letters are replaced with 
echo %LINE_MODE%lqqqqqqqqqqqqqqqqqqqqqqqqqqqk%TEXT_MODE%
echo %LINE_MODE%x                           x%TEXT_MODE%
echo %LINE_MODE%mqqqqqqqqqqqqqqqqqqqqqqqqqqqj%TEXT_MODE%
:: move cursor 2 lines up and 4 lines right, then draw bright blue text on dark red background
:: see the table here for the color codes https://en.wikipedia.org/wiki/ANSI_escape_code#3-bit_and_4-bit
echo %ESC%[2A%ESC%[4G%ESC%[94;41mHello from ANSI colors!%ESC%[37;40m

exit/b

:: This is a non-obvious way to print a string without a new line
:: alternatively, I could just move cursor up, of course
:echo_no_newline
<nul set/p "_any_variable=%1"
exit /b