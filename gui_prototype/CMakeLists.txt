cmake_minimum_required(VERSION 3.20)

project(Halbschatten C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)

add_executable(Halbschatten
    src/main.c

    src/desktop/wallpaper.c
    src/desktop/layer.c

    src/engine/color.c
    src/engine/cell.c
    src/engine/surface.c
    src/engine/draw.c
    src/engine/engine.c

    src/graphics/renderer.c
    src/graphics/font.c
    src/graphics/glyph.c
)

target_include_directories(Halbschatten PRIVATE
    include
)

target_link_libraries(Halbschatten PRIVATE
    SDL2::SDL2
)

if(MSVC)
    target_compile_options(
        Halbschatten
        PRIVATE
        /W4
    )

else()
    target_compile_options(
        Halbschatten
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )

endif()
