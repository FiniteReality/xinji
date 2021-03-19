-- a very weak sandbox to allow xinji to mask globals safely

local ipairs = ipairs
local tinsert = table.insert
local type = type

local declare_mt = {
    __call = function(self, arg)
        if type(arg) == "string" then
            self.name = arg
        elseif type(arg) == "table" then
            for k, v in pairs(arg) do
                if type(k) == "number" then
                    tinsert(self.members, v)
                else
                    self[k] = v
                end
            end
        end

        return self
    end
}

return function()
    local types = { }
    local reverse = { }

    local function declare(data, skipDeclare)

        if not skipDeclare then
            if reverse[data] then
                types[reverse[data]] = nil
            end

            reverse[data] = data.name
            types[data.name] = data
        end

        return setmetatable(data, declare_mt)
    end

    local function primitive(name, skipDeclare)
        return declare(
            { kind = { "primitive" }, name = name },
            skipDeclare)
    end

    local function class(name, skipDeclare)
        return declare(
            { kind = { "class" }, name = name, members = { } },
            skipDeclare)
    end

    local function union(name, skipDeclare)
        return declare(
            { kind = { "union" }, name = name, members = { } },
            skipDeclare)
    end

    local environment = {
        declare = declare,
        primitive = primitive,
        class = class,
        union = union,
        std = _ENV or getfenv(1)
    }

    local metatable = {
        __index = function(_, key)
            if not types[key] then
                error(("Unknown type %q"):format(key), 2)
            else
                return declare({ kind = { "reference" }, type = key }, true)
            end
        end
    }

    return {
        types = types,
        fenv = setmetatable(environment, metatable)
    }
end
