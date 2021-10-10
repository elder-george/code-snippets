# Macros and scripts for FarManager

## CommandLine.Editor.lua

Edits the command line in editor.

Adapted from [this script](https://github.com/z0hm/far-scripts/blob/master/CommandLineF4.lua).

### Installation

copy or symlink to `%FARPROFILE%\Macros\Scripts`.

### Illustrates

- Basic macro structure;

- Use of FAR Lua API;

- Use of FFI

## setenv.lua

Executes a scripts that changes environment and copies the new values to the current one.

Circumvents the problem when it's hard to initialize, say, VS Command prompt in FAR because it runs scripts 
in new instances of shell.

### Use:

```>lua:setenv.lua <script name>```

Note: requires a patch to standard FAR installation like this

```
--- luamacro.old.lua
+++ luamacro.lua
@@ -191,6 +191,7 @@
     strParseError = ""
     Env = Env or setmetatable({}, gmeta)
     Env._filename = fname
+    Env._params = params
     setfenv(f1, Env)
     if f2 then setfenv(f2, Env) end
     return f1,f2
```

### Illustrates

- Use of `io.popen`