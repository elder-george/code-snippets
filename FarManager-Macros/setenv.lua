local pipe = io.popen("\"".._params.."\">NUL && set");
local lines = pipe:lines();
for l in lines do
    local k,v = string.match(l, "([^=]+)=(.*)")
    mf.env(k, 1, v);
end
pipe:close();
