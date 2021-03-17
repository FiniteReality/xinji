local xinji = require("xinji")

local file = assert(..., "file expected")
local types = assert(xinji.evaluate(file))

local function getIndentStr(amount)
    if amount < 1 then
        return ""
    elseif amount == 1 then
        return "- "
    else
        local r = { }
        for i = 1, amount - 1 do
            r[#r+1] = "| "
        end
        r[#r+1] = "- "

        return table.concat(r, '')
    end
end

local function printRecursive(value, indent)
    indent = indent or 1

    local indentStr = getIndentStr(indent)

    local empty = true
    for i, v in pairs(value) do
        empty = false
        io.write(("%s%s = %s\n"):format(
            indentStr,
            tostring(i),
            tostring(v)
        ))

        if type(v) == "table" then
            printRecursive(v, indent + 1)
        end
    end

    if empty then
        io.write(("%s(empty)\n"):format(indentStr))
    end
end

printRecursive(types)
