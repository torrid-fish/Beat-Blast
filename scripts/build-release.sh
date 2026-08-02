#!/usr/bin/env bash
# Build standalone Beat Blast bundles.
#
# Prerequisites are intentionally external to this repository:
#
# Linux:
#   sudo apt install cmake g++ pkg-config liballegro5-dev
#   Set LINUX_PREFIX if Allegro was installed outside /usr.
#
# Windows x86 from Linux:
#   Install i686-w64-mingw32-g++ and provide a prebuilt Allegro 5 x86 bundle.
#   Set ALLEGRO_WIN32_DIR to the directory containing include/, lib/, and bin/.
#
# Examples:
#   ./scripts/build-release.sh linux
#   ALLEGRO_WIN32_DIR=$HOME/allegro-win32/allegro/allegro \
#     ./scripts/build-release.sh windows
#   ./scripts/build-release.sh all

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
OUT_DIR="${OUT_DIR:-$ROOT_DIR/dist}"
LINUX_PREFIX="${LINUX_PREFIX:-/usr}"
LINUX_LIB_DIR="${LINUX_LIB_DIR:-$LINUX_PREFIX/lib/x86_64-linux-gnu}"
ALLEGRO_WIN32_DIR="${ALLEGRO_WIN32_DIR:-$HOME/allegro-win32/allegro/allegro}"
MINGW_PREFIX="${MINGW_PREFIX:-/usr}"

die() {
    printf 'error: %s\n' "$1" >&2
    exit 1
}

require_command() {
    command -v "$1" >/dev/null 2>&1 || die "missing command: $1"
}

build_linux() {
    require_command cmake
    require_command pkg-config
    require_command zip

    local build_dir="$ROOT_DIR/build-release-linux"
    local bundle_dir="$OUT_DIR/Beat-Blast-linux-x86_64"
    local lib_dir="$LINUX_LIB_DIR"

    PKG_CONFIG_PATH="$lib_dir/pkgconfig:${PKG_CONFIG_PATH:-}" \
        cmake -S "$ROOT_DIR" -B "$build_dir" \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_PREFIX_PATH="$LINUX_PREFIX"
    cmake --build "$build_dir" --parallel

    [[ -d "$lib_dir" ]] || die "Linux library directory not found: $lib_dir"
    rm -rf "$bundle_dir"
    mkdir -p "$bundle_dir/lib"
    cp "$build_dir/BeatBlast" "$bundle_dir/"
    cp -a "$ROOT_DIR/Beat-Blast/Assets" "$bundle_dir/"

    # These are the Allegro and codec libraries used by the standalone release.
    local library
    for library in \
        liballegro.so.5.2 liballegro_audio.so.5.2 \
        liballegro_acodec.so.5.2 liballegro_font.so.5.2 \
        liballegro_ttf.so.5.2 liballegro_image.so.5.2 \
        liballegro_primitives.so.5.2 libvorbis.so.0 libvorbisfile.so.3 \
        libvorbisenc.so.2 libogg.so.0 libopus.so.0 libopusfile.so.0 \
        libdumb.so.1 libopenal.so.1 libasound.so.2 \
        libpulse.so.0 libpulse-simple.so.0 libpulsecommon-16.1.so; do
        [[ -e "$lib_dir/$library" ]] || die "missing Linux library: $lib_dir/$library"
        cp -L "$lib_dir/$library" "$bundle_dir/lib/"
    done

    (cd "$bundle_dir" && zip -qr "$OUT_DIR/Beat-Blast-linux-x86_64.zip" BeatBlast Assets lib)
    printf 'created %s\n' "$OUT_DIR/Beat-Blast-linux-x86_64.zip"
}

build_windows() {
    export PATH="$MINGW_PREFIX/bin:$PATH"
    require_command i686-w64-mingw32-g++
    require_command zip

    local compiler="$(command -v i686-w64-mingw32-g++)"
    local source_dir="$ROOT_DIR/Beat-Blast/src"
    local bundle_dir="$OUT_DIR/Beat-Blast-windows-x86"
    local object_dir="$bundle_dir/obj"
    local allegro_inc="$ALLEGRO_WIN32_DIR/include"
    local allegro_lib="$ALLEGRO_WIN32_DIR/lib"
    local allegro_bin="$ALLEGRO_WIN32_DIR/bin"

    [[ -d "$allegro_inc" && -d "$allegro_lib" && -d "$allegro_bin" ]] \
        || die "ALLEGRO_WIN32_DIR must contain include/, lib/, and bin/"

    rm -rf "$bundle_dir"
    mkdir -p "$object_dir"
    local sources=(
        ammo big_monster button crystal game_window item main map menu
        normal_stage player shared small_monster utility win
    )
    local source
    for source in "${sources[@]}"; do
        "$compiler" -std=c++17 -O2 -Wall -Wno-unknown-pragmas \
            -Wno-unused-parameter -Wno-unused-variable -Wno-maybe-uninitialized \
            -Wno-return-type -D_CRT_SECURE_NO_WARNINGS \
            -D_CRT_SECURE_NO_DEPRECATE -DALLEGRO_NO_MAGIC_MAIN \
            -I "$source_dir" -I "$allegro_inc" -c "$source_dir/$source.cpp" \
            -o "$object_dir/$source.o"
    done

    "$compiler" -O2 -static-libgcc -static-libstdc++ "$object_dir"/*.o \
        -L "$allegro_lib" -lallegro_monolith -lwinpthread -lopengl32 \
        -lgdi32 -lcomdlg32 -lole32 -lwinmm -ldsound -ladvapi32 \
        -lshell32 -lshlwapi -luser32 -lkernel32 -lmsvcrt \
        -o "$bundle_dir/BeatBlast.exe"

    cp "$allegro_bin/allegro_monolith-5.2.dll" "$bundle_dir/"
    cp "$MINGW_PREFIX/i686-w64-mingw32/lib/libwinpthread-1.dll" "$bundle_dir/"
    cp "$MINGW_PREFIX/lib/gcc/i686-w64-mingw32/13-posix/libgcc_s_dw2-1.dll" "$bundle_dir/"
    cp "$MINGW_PREFIX/lib/gcc/i686-w64-mingw32/13-posix/libstdc++-6.dll" "$bundle_dir/"
    cp -a "$ROOT_DIR/Beat-Blast/Assets" "$bundle_dir/"

    (cd "$bundle_dir" && zip -qr "$OUT_DIR/Beat-Blast-windows-x86.zip" \
        BeatBlast.exe allegro_monolith-5.2.dll libwinpthread-1.dll \
        libgcc_s_dw2-1.dll libstdc++-6.dll Assets)
    rm -rf "$object_dir"
    printf 'created %s\n' "$OUT_DIR/Beat-Blast-windows-x86.zip"
}

mkdir -p "$OUT_DIR"
case "${1:-all}" in
    linux) build_linux ;;
    windows) build_windows ;;
    all) build_linux; build_windows ;;
    *) die "usage: $0 [linux|windows|all]" ;;
esac
