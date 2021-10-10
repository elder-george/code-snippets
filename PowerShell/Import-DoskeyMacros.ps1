$ErrorActionPreference = "Stop" 

# This script needs to be .-sourced, not run directly.
# Otherwise the functions will be created in a new scope and lost when that scope ends.
if ($MyInvocation.InvocationName -ne '.') {
    Write-Error -Message "This script needs to be sourced, not run directly.`nExecute ``. $($MyInvocation.MyCommand)``"
}

# import the definition of a helper function
. .\Convert-WithRegex.ps1

# This is an oversimplified version of the conversion.
# Real one would need to care about numbered arguments, multiple commands in the macro etc
function private:New-FunctionBody($cmd) {
    return "cmd.exe /c `"$($cmd -replace "\$\*",'$args')`""
}

# The creation of a "Function: item can't be done in a function, because that'll introduce new scope, too.
doskey /macros  | Convert-WithRegex '^(?<name>[^=]+)=(?<cmd>.+)$' `
                | ForEach-Object { Set-Item -Path "Function:$($_.name)" -Value (New-FunctionBody $_.cmd) }
