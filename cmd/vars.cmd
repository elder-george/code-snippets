@echo off
:: This is about regular variables, not the one created with `setx` command

:: setting variable with constant
set greeting=hello world
echo %greeting%

:: ensuring the variable is terminated
set "farewell=good bye" :: we don't want comment to end up in the variable
echo %farewell%

:: concatenation
set text=%greeting%, %username%
echo %text%

:: substring
echo %greeting:~5%  & :: starting with 5
echo %greeting:~3,2%& :: starting with 3, take 2 chars
echo %greeting:~-5% & :: starting with 5th from the end

:: string substitution
echo %greeting:o=O%  &  :: change 'o'->'O'

:: drop extra spaces
set "str=    a    word     "
set "s1=%str:  =%" &:: s1="a word "
echo %s1:~0,-1% &:: drops the last space, leaving "a word"

:: map-like structure (found [here](https://www.dostips.com/DtTipsStringManipulation.php))
:: note: it doesn't work in the command prompt, only in script
SET k=May

SET map=Jan-01;Feb-02;Mar-03;Apr-04;May-05;Jun-06;Jul-07;Aug-08;Sep-09;Oct-10;Nov-11;Dec-12
:: drop everything after `key-` and quote the result
CALL SET v=%%map:*%k%-=%%
:: drop everything after `;` by inserting a comment
SET v=%v:;=&::%

ECHO.%v%

:: Arithmetics
set /a answer=5*8+2 >NUL &:: for some reason it prints the result in addition to setting it
echo %answer%

goto :skip_prompt
:: prompt from user
set /p name=Introduce yourself: &:: disabling it so that it doesn't block script execution
set /p line=<line.txt &::read line from a file

:skip_prompt

:: indirect access
set I=5
set "A_%i%=10"
echo %A_5%

:: `cmd`'s execution model is very antique. By default it eveluates every variable once
:: To circumvent it, one may enable "delayed expansion" and dereference variables with !..!
setlocal enabledelayedexpansion
set I=0
for /l %%n in (%I%,1,5) do (
    set /a v=!I! * !I!
    set "A_!I!=!v!"
    set /a I+=1
)
set A_ &:: print all virables starting with `A_`
setlocal disabledelayedexpansion

:: indirect access can also be used to return values from functions
goto:square_use
:square
    :: %~1 = %name of output variable
    :: %2 = number to square
    set /a "%~1=%~2 * %~2"
    goto:EOF &:: return

:square_use
call:square "result" 5
echo %result%

:: for more complex functions setting scope is recommended
:: soruce https://www.dostips.com/DtTutoFunctions.php
goto:skip_function
:another_function
    :: %~1 stores name of output variable
    :: ... more args
    setlocal
    :: do stuff
    set localVar=foo
    (endlocal &::return values
        if "%~1" neq "" set "%~1"=%localVar%"
    )
:skip_function
