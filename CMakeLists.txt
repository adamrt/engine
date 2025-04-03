cmake_minimum_required(VERSION 3.5)
project(
  Engine
  VERSION     0.1
  DESCRIPTION "Engine for PSX"
  LANGUAGES   C)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g3 -ggdb")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Linux -pthread shenanigans
if (CMAKE_SYSTEM_NAME STREQUAL Linux)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
endif()

# Use left-handed coordinate system
add_compile_definitions(CGLM_FORCE_LEFT_HANDED)
option(CGLM_SHARED "Shared build" OFF)
option(CGLM_STATIC "Static build"  ON)
option(CGLM_USE_C99 "" ON)
add_subdirectory(lib/cglm/ EXCLUDE_FROM_ALL)


set(SHARED_INCLUDE_DIRS lib/sokol lib/sokol/util)
set(SHARED_MAC_COMPILE_OPTIONS -x objective-c)
set(SHARED_MAC_FRAMEWORKS "-framework QuartzCore" "-framework Cocoa" "-framework MetalKit" "-framework Metal" "-framework OpenGL" "-framework AudioToolbox")
set(SHARED_LINUX_LIBS X11 Xi Xcursor GL asound dl m Threads::Threads)

function(add_shared_library lib_name source_file)
    add_library(${lib_name} STATIC ${source_file})
    target_include_directories(${lib_name} PUBLIC ${SHARED_INCLUDE_DIRS})

    if(CMAKE_SYSTEM_NAME STREQUAL Darwin)
        target_compile_options(${lib_name} PRIVATE ${SHARED_MAC_COMPILE_OPTIONS})
        target_link_libraries(${lib_name} ${SHARED_MAC_FRAMEWORKS})
    elseif(CMAKE_SYSTEM_NAME STREQUAL Linux)
        target_link_libraries(${lib_name} INTERFACE ${SHARED_LINUX_LIBS})
    endif()
endfunction()

# Add libraries using the helper function
add_shared_library(sokol lib/sokol.c)

# Add engine executable
add_executable(engine
    src/main.c
    src/common.c
    #src/gpu_impl.c
    src/cpu_impl.c
)

target_link_libraries(engine sokol cglm_headers)

# Emscripten specific settings
if (CMAKE_SYSTEM_NAME STREQUAL Emscripten)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    target_link_options(engine PUBLIC --shell-file ../src/shell.html)
    target_link_options(engine PUBLIC
        -sUSE_WEBGL2=1
        -sNO_FILESYSTEM=0
        -sASSERTIONS=1
        -sMALLOC=emmalloc
        -sTOTAL_STACK=768mb
        -sTOTAL_MEMORY=1024mb
        -sEXPORTED_FUNCTIONS=_main
        -sEXPORTED_RUNTIME_METHODS=ccall
        --closure=1)
endif()

if (NOT CMAKE_SYSTEM_NAME STREQUAL Emscripten)
target_compile_options(engine PUBLIC
    -Wall
    -g
    -O0
    -Wextra
    -Wpedantic
    -Werror
    -Wno-unused-function
    -Wshadow
    -Wunreachable-code
    -Wnull-dereference
    -Wformat=2
    -Wstrict-aliasing
    -Wimplicit-fallthrough
    -fsanitize=address
    -fsanitize=undefined
    -fno-omit-frame-pointer)
endif()

if (CMAKE_C_COMPILER_ID MATCHES "Clang" AND NOT CMAKE_SYSTEM_NAME STREQUAL Emscripten)
    target_link_options(engine PRIVATE LINKER:-dead_strip -fsanitize=address)

elseif (CMAKE_C_COMPILER_ID MATCHES "GNU" AND NOT CMAKE_SYSTEM_NAME STREQUAL Emscripten)
    target_link_options(engine PRIVATE -Wl, --gc-sections)
endif()
