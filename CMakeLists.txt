cmake_minimum_required(VERSION 3.16)
project(BeatBlast CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Look for the game sources relative to this file (repo layout:
#   <root>/CMakeLists.txt
#   <root>/Beat-Blast/src/*.cpp      (game sources)
#   <root>/Beat-Blast/Assets/        (runtime data))
set(BB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/Beat-Blast/src)
set(BB_ASSETS ${CMAKE_CURRENT_SOURCE_DIR}/Beat-Blast/Assets)

set(BB_SOURCES
    ${BB_SRC}/ammo.cpp
    ${BB_SRC}/big_monster.cpp
    ${BB_SRC}/button.cpp
    ${BB_SRC}/crystal.cpp
    ${BB_SRC}/game_window.cpp
    ${BB_SRC}/item.cpp
    ${BB_SRC}/main.cpp
    ${BB_SRC}/map.cpp
    ${BB_SRC}/menu.cpp
    ${BB_SRC}/normal_stage.cpp
    ${BB_SRC}/player.cpp
    ${BB_SRC}/shared.cpp
    ${BB_SRC}/small_monster.cpp
    ${BB_SRC}/utility.cpp
    ${BB_SRC}/win.cpp
)

add_executable(BeatBlast ${BB_SOURCES})

target_include_directories(BeatBlast PRIVATE ${BB_SRC})

target_compile_definitions(BeatBlast PRIVATE
    _CRT_SECURE_NO_WARNINGS
    _CRT_SECURE_NO_DEPRECATE
    ALLEGRO_NO_MAGIC_MAIN
)

if(MSVC)
    target_compile_options(BeatBlast PRIVATE /utf-8 /permissive- /W3)
else()
    target_compile_options(BeatBlast PRIVATE -Wall -Wno-unknown-pragmas -Wno-unused-parameter)
endif()

# Linux: tell the linker to find bundled .so files in lib/ next to the binary.
if(UNIX AND NOT APPLE)
    set(CMAKE_BUILD_RPATH "$ORIGIN/lib")
    set(CMAKE_INSTALL_RPATH "$ORIGIN/lib")
endif()

# Pick up Allegro either from an installed AllegroConfig.cmake (vcpkg, custom build)
# or via pkg-config (apt / system package manager).
find_package(Allegro 5.2 CONFIG QUIET)

if(Allegro_FOUND)
    message(STATUS "Found Allegro via cmake config (Allegro_DIR=${Allegro_DIR})")
    set(_bb_allegro_targets
        allegro
        allegro_image
        allegro_font
        allegro_ttf
        allegro_primitives
        allegro_audio
        allegro_acodec
        allegro_native_dialog
    )
    set(_bb_link_libs "")
    foreach(_t IN LISTS _bb_allegro_targets)
        if(TARGET ${_t})
            list(APPEND _bb_link_libs ${_t})
        endif()
    endforeach()
    target_link_libraries(BeatBlast PRIVATE ${_bb_link_libs})
else()
    message(STATUS "AllegroConfig not found; falling back to pkg-config.")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(ALLEGRO REQUIRED IMPORTED_TARGET
        allegro
        allegro_image
        allegro_font
        allegro_ttf
        allegro_primitives
        allegro_audio
        allegro_acodec
        allegro_native_dialog
    )
    target_link_libraries(BeatBlast PRIVATE PkgConfig::ALLEGRO)
endif()

# Distribution layout:
#   install_prefix/BeatBlast          <- the executable
#   install_prefix/Assets/
#   install_prefix/lib/               <- bundled .so files (Linux)
install(TARGETS BeatBlast RUNTIME DESTINATION .)
install(DIRECTORY ${BB_ASSETS} DESTINATION .)