# 心计 - A scheming schema definition language #

心计 (Xīnjì, pronounced /ɕīntɕìː/, or if you can't read IPA, "shinji" is close
enough) is a schema definition language designed to be used as the
single-source-of-truth of types used in a microservices application involving
multiple applications.

## How does it work? ##

心计 lets you define types using Lua code, which means you're free to use all of
the Lua facilities to augment your schema, as a form of metaprogramming. As an
example:

```lua
primitive "bool"
primitive "float"
primitive "string"

primitive "uint8"
primitive "uint64"

-- an optional value
function optional(valueType)
    local result = union {
        kind = { "union", "optional" }
        valueType,
        null
    }
    return result
end

class "item" {
    uint64 "id",
    string "name",
    enum(uint8) "category" {
        "armor",
        "weapon",
        "ammo",
        "consumable",
    }
}

class "player" {
    uint64 "id",
    string "name",
    tuple(float, float, float) "position",
    array(3 * 9, optional(item)) "inventory"
}
```
