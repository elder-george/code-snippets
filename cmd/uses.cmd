:: Validates that all the command names passed as args can be resolved using PATH.
@for /d %%a in (%*) do @( (where %%a >NUL 2<&1)  || (echo COMMAND %%a NOT FOUND & exit 1) )
