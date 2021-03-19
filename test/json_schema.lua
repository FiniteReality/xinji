local xinji = require("xinji")
local json = require("json")

local file = assert(..., "file expected")
local types = assert(xinji.evaluate(file))

local getKind do
    local known = {
        "tuple",
        "union",
        "class",
        "primitive",
        "reference"
    }
    for i = 1, #known do
        known[known[i]] = true
    end

    function getKind(type)
        local kinds = type.kind
        for i = #kinds, 1, -1 do
            local kind = kinds[i]

            if known[kind] then
                return kind
            end
        end
    end
end

local getPrimitiveType do
    local known = {
        -- json schema only supports the json types, so there's information
        -- loss here
        ["^int%d+"] = "number"
    }

    function getPrimitiveType(name)
        for i, v in pairs(known) do
            if name:match(i) then
                return v
            end
        end
    end
end

local function schemaify(type, knownSchemas)
    local schemaType = getKind(type)

    if schemaType == "tuple" then
        local members = { }
        for _, member in ipairs(type.members) do
            members[#members+1] = schemaify(member, knownSchemas)
        end

        return {
            type = "array",
            items = members,
            minItems = #members,
            additionalItems = false
        }
    elseif schemaType == "union" then
        local members = { }
        for _, member in ipairs(type.members) do
            members[#members+1] = schemaify(member, knownSchemas)
        end

        return {
            oneOf = members
        }
    elseif schemaType == "class" then
        local members = { }
        local required = { }
        for _, member in ipairs(type.members) do
            members[member.name] = schemaify(member, knownSchemas)
            required[#required+1] = member.name
        end

        return {
            type = "object",
            properties = members,
            required = required,
            additionalProperties = false
        }
    elseif schemaType == "primitive" then
        return {
            type = getPrimitiveType(type.name)
        }
    elseif schemaType == "reference" then
        return {
            ["$ref"] = ("#/definitions/%s"):format(type.type)
        }
    end
end

local schemas = { }
local lastSchema
for i, v in pairs(types) do
    schemas[i] = schemaify(v, schemas)
    lastSchema = i
end

print(json.encode{
    ["$schema"] = "http://json-schema.org/draft-06/schema#",
    ["$ref"] = ("#/definitions/%s"):format(lastSchema),
    definitions = schemas
})
