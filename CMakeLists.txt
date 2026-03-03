cmake_minimum_required(VERSION 3.16)
project(k_os C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED ENV{KOS_SRC_PATH})
    message(FATAL_ERROR "KOS_SRC_PATH is not set. Please point it to the k_os source root (e.g. C:/csprojects/k-os).")
endif()
get_filename_component(KOS_SRC_PATH "$ENV{KOS_SRC_PATH}" ABSOLUTE)

# Windows-specific compiler definitions
if(WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-DNOMINMAX)
    add_definitions(-D_WIN32_WINNT=0x0601)  # Windows 7
    add_definitions(-DWINVER=0x0601)

    # MinGW specific settings
    if(MINGW)
        add_definitions(-D__USE_MINGW_ANSI_STDIO=1)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unknown-pragmas")
    endif()
endif()

include_directories(${KOS_SRC_PATH} ${KOS_SRC_PATH}/inc ${KOS_SRC_PATH}/samples/OSEKBumser)
include_directories(${KOS_SRC_PATH}/port/windows)

option(BUILD_K_OS "Build k_os kernel" OFF)

set(K_OS_SOURCES
        src/Com_Flt.c
        src/Com_If.c
        src/Com_Int.c
        src/Os_Alm.c
        src/Os_Ctr.c
        src/Os_Error.c
        src/Os_Evt.c
        src/Os_Exec.c
        src/Os_Intr.c
        src/Os_Mlq.c
        src/Os_Orti.c
        src/Os_ParamAccess.c
        src/Os_Res.c
        src/Os_SchT.c
        src/Os_Task.c
        src/Os_Vars.c
        port/windows/Os_Port_Win32.c
        samples/OSEKBumser/Os_Cfg.c
        samples/OSEKBumser/main.c)

# Optionally include COM extensions if present
if(EXISTS ${CMAKE_SOURCE_DIR}/src/Com_Ext.c)
    list(APPEND K_OS_SOURCES src/Com_Ext.c)
endif()

if(BUILD_K_OS)
    add_executable(k_os ${K_OS_SOURCES})
endif()

# Link Windows libraries
if(WIN32)
    if(TARGET k_os)
        target_link_libraries(k_os
                kernel32
                user32
                winmm
        )
    endif()

    # MinGW specific libraries
    if(MINGW)
        if(TARGET k_os)
            target_link_libraries(k_os pthread)
        endif()
    endif()
endif()

# Port directories
add_subdirectory(port/rp2040)

# C++ OIL generator
add_subdirectory(tools/oilcpp)

# OIL parser tool (C++17)
add_executable(oilparse
        tools/oil/OilLexer.cpp
        tools/oil/OilParser.cpp
        tools/oil/oilparse.cpp)

target_include_directories(oilparse PRIVATE tools/oil)

# Compiler specific flags
if(MSVC)
    set_property(TARGET k_os PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set_property(TARGET oilparse PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
elseif(MINGW)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc")
endif()
