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

# Pick up Allegro via one of two routes:
#   1. pkg-config (system Allegro on Linux distros). Preferred on Linux
#      because Ubuntu/Debian's AllegroConfig.cmake exports target names that
#      differ from upstream and would silently produce an empty link list.
#   2. AllegroConfig.cmake (vcpkg on Windows). Used when pkg-config is not
#      available or finds nothing.
#
# After this block, _BB_LINK_TARGETS holds the CMake targets / imported
# targets we will pass to target_link_libraries. We FATAL_ERROR if it ends
# up empty so the link step can never silently lose Allegro.

set(_BB_LINK_TARGETS "")

# Route 1: pkg-config. Ubuntu/Debian ships these module names with the -5
# suffix (allegro-5.pc, allegro_image-5.pc, ...).
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(_bb_allegro QUIET
        allegro-5
        allegro_image-5
        allegro_font-5
        allegro_ttf-5
        allegro_primitives-5
        allegro_audio-5
        allegro_acodec-5
        allegro_dialog-5
    )
    if(_bb_allegro_FOUND)
        message(STATUS "Found Allegro via pkg-config (modules: ${_bb_allegro_MODULES})")
        list(APPEND _BB_LINK_TARGETS PkgConfig::_bb_allegro)
    endif()
endif()

# Route 2: AllegroConfig.cmake (vcpkg, custom install, etc.)
if(NOT _BB_LINK_TARGETS)
    find_package(Allegro 5.2 CONFIG QUIET)
    if(Allegro_FOUND)
        message(STATUS "Found Allegro via cmake config (Allegro_DIR=${Allegro_DIR})")
        # Try every plausible target naming convention Allegro has shipped
        # (un-namespaced lowercase, Allegro:: namespaced, allegro:: namespaced).
        set(_bb_cmake_candidates
            allegro allegro_image allegro_font allegro_ttf
            allegro_primitives allegro_audio allegro_acodec allegro_native_dialog
            Allegro::Allegro Allegro::Image Allegro::Font Allegro::TTF
            Allegro::Primitives Allegro::Audio Allegro::Acodec Allegro::NativeDialog
            allegro::allegro allegro::image allegro::font allegro::ttf
            allegro::primitives allegro::audio allegro::acodec allegro::native_dialog
        )
        foreach(_t IN LISTS _bb_cmake_candidates)
            if(TARGET ${_t})
                list(APPEND _BB_LINK_TARGETS ${_t})
            endif()
        endforeach()
    endif()
endif()

if(NOT _BB_LINK_TARGETS)
    message(FATAL_ERROR
        "Allegro 5 was not found via either pkg-config or AllegroConfig.cmake.\n"
        "On Ubuntu install: liballegro5-dev liballegro-image5-dev liballegro-ttf5-dev "
        "liballegro-audio5-dev liballegro-acodec5-dev liballegro-dialog5-dev\n"
        "On Windows use vcpkg with the allegro5 port.")
endif()

message(STATUS "Linking BeatBlast against: ${_BB_LINK_TARGETS}")
target_link_libraries(BeatBlast PRIVATE ${_BB_LINK_TARGETS})

# Distribution layout:
#   install_prefix/BeatBlast          <- the executable
#   install_prefix/Assets/
#   install_prefix/lib/               <- bundled .so files (Linux)
install(TARGETS BeatBlast RUNTIME DESTINATION .)
install(DIRECTORY ${BB_ASSETS} DESTINATION .)