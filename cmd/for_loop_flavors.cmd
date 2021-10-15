:: `FOR` loops have many tricky variants and use

:: arithmetic loop
for /L %%v IN (0,1,10) do @(echo %%v)

:: iterate over files
for %%f in (*.cmd) do @( echo %%f )

:: iterate over directories (using wildcard)
for /D %%d in (*.cmd) do @( echo %%d )

:: search for files recursively
for /r C:\Windows\System32 %%f in (*.cmd) do @( echo %%f )

:: Iterate over the command output
for /f %%f in ('dir /b c:\windows\System32\drivers\etc') do @( echo %%f )
:: `usebackq` option changes the quotes role, so this would also work
for /f "usebackq" %%f in (`dir /b c:\windows\System32\drivers\etc`) do @( echo %%f )

:: process columnar data (a la UNIX `cut`)
:: tokens are stored in %(var), %(var+1), so better start with %a
:: for example
for /f "delims=\\ tokens=1,5,6 usebackq" %a in (`dir /b /s c:\windows\System32\drivers\etc\*`) do @echo %a %b %c
:: prints: 
:: c: etc hosts
:: c: etc hosts.ics
:: c: etc lmhosts.sam
:: c: etc networks
:: c: etc protocol
:: c: etc services 

:: This is also the only way (I know) to set a variable from the command output:
for /f %%f in ('dir /b c:\windows\System32\drivers\etc') do @( SET MY_VAR=%%f )

:: ... or to do file path processing
for /f %%F in ('dir /b c:\windows\System32\drivers\etc') do @( 
    (set _PATH=%%~fF) ::full path
    (set _DRIVE=%%~dF) :: drive (c:, d: etc)
    (set _DIR=%%~pF) :: path to directory
    (set _FILENAME_ONLY=%%~nF) :: filename (without extension)
    (set _EXT_ONLY=%%~xF) :: extension only`
    (set _SZ=%%~zF) ::file size
    (set _TIME=%%~aF) ::access time
    (set _WHERE=%%~$PATH:F) :: equivalent of `where.exe`
    :: etc. see `FOR /?` or `HELP FOR` for more uses
)

:: `for /f` also works with strings
for /f %%F in ("myfile.txt") do ( echo %%~nF )
