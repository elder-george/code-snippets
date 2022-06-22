:: Loading variables from .env file before running a command
:: The trick here is to use `/s` key so that quotes in the `cmd /c` command are not expanded.
:: Example:
:: > echo HOMEDRIVE=D:
:: > dotenv (set HOMEDRIVE)
:: D:
@cmd /s /c "(for /f "tokens=1,2 delims==" %%i in (.\.env) do (set %%i=%%j)) & %1 %2 %3 %4 %5 %6 %7 %8 %9"