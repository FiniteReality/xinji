local function generator(f)
    return function()
        local result = f:read(1024)

        if not result then
            f:close()
        end

        return result
    end
end

local function loadfile(filename, mode, env)
    local file, err = io.open(filename, "r")

    if not file then
        return nil, err
    end

    mode = mode or "bt"

    if file:read(1) == "\27" and mode:sub(1,1) ~= "b" then
        return nil, "binary chunk found"
    end

    file:seek("set")

    local f, err = load(
        generator(file),
        ("@%s"):format(filename))

    if not f then
        return nil, err
    end

    if env then
        f = setfenv(f, env)
    end

    return f
end

return {
    loadfile = loadfile
}
