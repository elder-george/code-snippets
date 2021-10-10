Set-StrictMode -Version 6.0
filter Convert-WithRegex([regex]$Regex, [switch]$Strict) {
    <#
    .SYNOPSIS
        Tries to parse a line in input into an object using regular expression.

    .DESCRIPTION
        Convert-WithRegex is a filter that applies a regular expression to every line in input.
        If it matches, it creates an object with a property for each group in the regex.
        Doesn't try to convert properties from string to any other type. Use Select-Object or Add-Member for that.
    
    .PARAMETER Regex
        Regular expression that is going to be used for parsing.
        Each matched group will be converted into a field of the resulting object.
    
    .PARAMETER Strict
        If set, causes Error to be reported.

    .EXAMPLE
        # Parse hosts file
        Get-Content $env:WinDir\System32\drivers\etc\hosts | Convert-WithRegex '^\s*(?<ip>[\d\.]+)\s+(?<host>[^\s]+)\s*(?<comment>#.*)?$'

    .EXAMPLE
        # Parse hosts file and make 'ip' a typed property instead of `string`
        Get-Content $env:WinDir\System32\drivers\etc\hosts | Convert-WithRegex '^\s*(?<ip>[\d\.]+)\s+(?<host>[^\s]+)\s*(?<comment>#.*)?$' `
            | Select-Object -ExcludeProperty ip -Property *,@{l='ip';e={[ipaddress]$_.ip}}
    #>
    if ($_ -match $Regex) {
        $Matches.Remove(0);
        New-Object -TypeName PSObject -Property $Matches
    } elseif ($Strict) {
        Write-Error "'$_' doesn't match /$Regex/";
    }
}