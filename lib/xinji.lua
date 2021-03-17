local loadfile = loadfile

if _VERSION < "Lua 5.2" then
    local polyfills = require("xinji.internal.polyfills")

    loadfile = polyfills.loadfile
end

local createEnv = require("xinji.internal.environment")

-- Evaluates the given Xinji file
local function evaluate(file)
    local env = createEnv()
    local func, err = loadfile(file, "t", env.fenv)

    if not func then
        return false, err
    end

    func()

    return env.types
end

return {
    evaluate = evaluate
}
