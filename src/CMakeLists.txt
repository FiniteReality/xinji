include(GenerateExportHeader)
find_package(Lua 5.2 REQUIRED)

add_library(lua::Lua UNKNOWN IMPORTED)
set_target_properties(lua::Lua
    PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${LUA_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${LUA_MATH_LIBRARY}"
        IMPORTED_LOCATION "${LUA_LIBRARY}"
)

set(SOURCES
    xinji.c
)

add_library(xinji ${SOURCES})
generate_export_header(xinji)
target_link_libraries(xinji
    PUBLIC
        lua::Lua
)
target_include_directories(xinji
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include>
)
set_target_properties(xinji PROPERTIES
    PUBLIC_HEADER "include/xinji.h")

install(
    TARGETS xinji
    LIBRARY DESTINATION lib
)
