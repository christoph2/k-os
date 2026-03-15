cmake_minimum_required(VERSION 3.16)
project(k_os C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(KOS_CROSS_ARM OFF)
if(CMAKE_C_COMPILER MATCHES "arm-none-eabi")
    set(KOS_CROSS_ARM ON)
endif()

# Source root (defaults to this tree, can be overridden)
set(KOS_SRC_PATH "${CMAKE_CURRENT_SOURCE_DIR}" CACHE PATH "Path to k_os source root")
set(KOS_INCLUDE_DIRS "${KOS_SRC_PATH}" "${KOS_SRC_PATH}/inc")

# Shared kernel sources
set(KOS_KERNEL_SOURCES
    ${KOS_SRC_PATH}/src/Com_Flt.c
    ${KOS_SRC_PATH}/src/Com_If.c
    ${KOS_SRC_PATH}/src/Com_Int.c
    ${KOS_SRC_PATH}/src/Os_Alm.c
    ${KOS_SRC_PATH}/src/Os_Ctr.c
    ${KOS_SRC_PATH}/src/Os_Error.c
    ${KOS_SRC_PATH}/src/Os_Evt.c
    ${KOS_SRC_PATH}/src/Os_Exec.c
    ${KOS_SRC_PATH}/src/Os_Intr.c
    ${KOS_SRC_PATH}/src/Os_Mlq.c
    ${KOS_SRC_PATH}/src/Os_Orti.c
    ${KOS_SRC_PATH}/src/Os_ParamAccess.c
    ${KOS_SRC_PATH}/src/Os_Res.c
    ${KOS_SRC_PATH}/src/Os_SchT.c
    ${KOS_SRC_PATH}/src/Os_Task.c
    ${KOS_SRC_PATH}/src/Os_Vars.c
    ${KOS_SRC_PATH}/src/Utl.c
)
if(EXISTS ${KOS_SRC_PATH}/src/Com_Ext.c)
    list(APPEND KOS_KERNEL_SOURCES ${KOS_SRC_PATH}/src/Com_Ext.c)
endif()

# C++ OIL generator / parser
if(NOT KOS_CROSS_ARM)
    add_subdirectory(tools/oilcpp)

    # OIL parser tool (C++17)
    add_executable(oilparse
        tools/oil/OilLexer.cpp
        tools/oil/OilParser.cpp
        tools/oil/oilparse.cpp
    )
    target_include_directories(oilparse PRIVATE tools/oil)

    # Install generator + parser
    install(TARGETS kosgen_cpp oilparse RUNTIME DESTINATION bin)

    if(MSVC)
        set_property(TARGET kosgen_cpp PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
        set_property(TARGET oilparse   PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    elseif(MINGW)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc")
    endif()
else()
    if(NOT KOSGEN_CPP_EXECUTABLE)
        # Default to in-tree host build if present
        set(KOSGEN_CPP_EXECUTABLE "${CMAKE_SOURCE_DIR}/tools/oilcpp/kosgen_cpp.exe" CACHE FILEPATH "Host-built kosgen_cpp for cross builds")
    endif()
    if(NOT EXISTS "${KOSGEN_CPP_EXECUTABLE}")
        message(FATAL_ERROR "Cross-compiling with arm-none-eabi requires a host-built kosgen_cpp. Set KOSGEN_CPP_EXECUTABLE to the host binary path.")
    endif()
    add_executable(kosgen_cpp IMPORTED GLOBAL)
    set_target_properties(kosgen_cpp PROPERTIES IMPORTED_LOCATION "${KOSGEN_CPP_EXECUTABLE}")
    message(STATUS "Using prebuilt kosgen_cpp at ${KOSGEN_CPP_EXECUTABLE} for cross build.")
    # oilparse is not needed for target builds; skip in cross mode.
endif()

# Helper to collect port specifics
function(kos_port_config PLATFORM OUT_SRCS OUT_LIBS OUT_INCS)
    set(srclist "")
    set(liblist "")
    set(inclist "")
    if(PLATFORM STREQUAL "windows")
        list(APPEND srclist ${KOS_SRC_PATH}/port/windows/Os_Port_Win32.c)
        list(APPEND inclist ${KOS_SRC_PATH}/port/windows)
        list(APPEND liblist kernel32 user32 winmm)
        if(MINGW)
            list(APPEND liblist pthread)
        endif()
    elseif(PLATFORM STREQUAL "pthreads" OR PLATFORM STREQUAL "posix")
        list(APPEND srclist ${KOS_SRC_PATH}/port/pthreads/Os_Port_Unix.c)
        list(APPEND inclist ${KOS_SRC_PATH}/port/pthreads)
        list(APPEND liblist pthread rt)
    elseif(PLATFORM STREQUAL "rp2040")
        list(APPEND srclist ${KOS_SRC_PATH}/port/rp2040/Os_Port_RP2040.c)
        list(APPEND inclist ${KOS_SRC_PATH}/port/rp2040)
        # Toolchain/SDK linking handled in port/rp2040 CMake if enabled.
    elseif(PLATFORM STREQUAL "stm32")
        list(APPEND srclist ${KOS_SRC_PATH}/port/stm32/Os_Port_STM32.c)
        list(APPEND inclist ${KOS_SRC_PATH}/port/stm32)
    endif()
    set(${OUT_SRCS} "${srclist}" PARENT_SCOPE)
    set(${OUT_LIBS} "${liblist}" PARENT_SCOPE)
    set(${OUT_INCS} "${inclist}" PARENT_SCOPE)
endfunction()

# Helper to build a generated example for a platform
function(kos_add_generated_example NAME PLATFORM OIL_FILE)
    kos_port_config(${PLATFORM} PORT_SRCS PORT_LIBS PORT_INCS)
    if(PORT_SRCS STREQUAL "")
        message(STATUS "Skipping ${NAME}: no port sources for platform ${PLATFORM}")
        return()
    endif()

    set(gen_dir "${CMAKE_BINARY_DIR}/generated/${NAME}")
    file(MAKE_DIRECTORY "${gen_dir}")
    set(gen_prefix "${gen_dir}/Os_Cfg")

    set(gen_stub "${gen_prefix}_stubs.c")

    add_custom_command(
        OUTPUT "${gen_prefix}.c" "${gen_prefix}.h" "${gen_prefix}.info.txt" "${gen_prefix}.ort" "${gen_stub}" "${gen_dir}/main.c" "${gen_dir}/kosgen.cmake" "${gen_dir}/CMakeLists.txt"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${gen_dir}"
        COMMAND $<TARGET_FILE:kosgen_cpp> --include "${KOS_SRC_PATH}/config/oil" --target ${PLATFORM} --orti -o "${gen_prefix}" "${OIL_FILE}"
        DEPENDS kosgen_cpp "${OIL_FILE}"
        VERBATIM
    )
    add_custom_target(${NAME}_cfg DEPENDS "${gen_prefix}.c" "${gen_prefix}.h" "${gen_prefix}.info.txt" "${gen_prefix}.ort" "${gen_stub}")

    add_executable(${NAME}
        ${KOS_KERNEL_SOURCES}
        ${PORT_SRCS}
        "${gen_prefix}.c"
        "${gen_stub}"
        "${gen_dir}/main.c"
    )
    add_dependencies(${NAME} ${NAME}_cfg)
    target_include_directories(${NAME} PRIVATE ${KOS_INCLUDE_DIRS} ${PORT_INCS} "${gen_dir}")
    target_link_libraries(${NAME} PRIVATE ${PORT_LIBS})

    set(output_subdir "")
    if(PLATFORM STREQUAL "windows")
        set(output_subdir "windows")
    elseif(PLATFORM STREQUAL "pthreads" OR PLATFORM STREQUAL "posix")
        set(output_subdir "posix")
    elseif(PLATFORM STREQUAL "stm32")
        set(output_subdir "stm32")
    endif()
    if(NOT output_subdir STREQUAL "")
        set(outdir "${KOS_SRC_PATH}/samples/${output_subdir}/build")
        file(MAKE_DIRECTORY "${outdir}")
        set_target_properties(${NAME} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${outdir}"
            OUTPUT_NAME "${NAME}")
    endif()

    if(WIN32 AND PLATFORM STREQUAL "windows")
        target_compile_definitions(${NAME} PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX _WIN32_WINNT=0x0601 WINVER=0x0601)
    endif()
endfunction()

# Platform-specific sample builds
if(WIN32)
    kos_add_generated_example(kos_sample_windows "windows" "${KOS_SRC_PATH}/samples/windows/first.oil")
elseif(UNIX)
    kos_add_generated_example(kos_sample_posix "pthreads" "${KOS_SRC_PATH}/samples/posix/first.oil")
endif()

set(KOS_RP2040_AVAILABLE OFF)
if(DEFINED ENV{PICO_SDK_PATH})
    find_program(ARM_NONE_EABI_GCC NAMES arm-none-eabi-gcc
        HINTS
            "$ENV{PICO_TOOLCHAIN_PATH}/bin"
            "$ENV{ARM_GCC_TOOLCHAIN}/bin"
            "$ENV{GNU_ARM_TOOLCHAIN}/bin"
    )
    if(ARM_NONE_EABI_GCC AND CMAKE_C_COMPILER MATCHES "arm-none-eabi")
        set(KOS_RP2040_AVAILABLE ON)
        message(STATUS "RP2040 build enabled (arm-none-eabi toolchain active: ${CMAKE_C_COMPILER}).")
    elseif(ARM_NONE_EABI_GCC)
        message(STATUS "PICO_SDK_PATH set and arm-none-eabi-gcc found at ${ARM_NONE_EABI_GCC}, but current compiler is ${CMAKE_C_COMPILER}; reconfigure with arm-none-eabi-gcc to build RP2040 sample (skipping for now).")
    else()
        message(STATUS "PICO_SDK_PATH set but arm-none-eabi-gcc not found; skipping RP2040 sample build.")
    endif()
else()
    message(STATUS "PICO_SDK_PATH not set; skipping RP2040 sample build.")
endif()

if(KOS_RP2040_AVAILABLE)
    set(KOS_RP2040_SAMPLE_OIL "${KOS_SRC_PATH}/samples/pico/first.oil" CACHE FILEPATH "Sample OIL file for RP2040 config generation" FORCE)
    add_subdirectory(port/rp2040)
endif()
# STM32 availability detection (cross toolchain required)
set(KOS_STM32_TOOLCHAIN "")
if(CMAKE_C_COMPILER MATCHES "arm-none-eabi")
    set(KOS_STM32_TOOLCHAIN "${CMAKE_C_COMPILER}")
elseif(DEFINED ENV{STM32_TOOLCHAIN_PATH})
    find_program(ARM_NONE_EABI_GCC NAMES arm-none-eabi-gcc HINTS "$ENV{STM32_TOOLCHAIN_PATH}/bin")
    if(ARM_NONE_EABI_GCC)
        set(KOS_STM32_TOOLCHAIN "${ARM_NONE_EABI_GCC}")
    endif()
else()
    find_program(ARM_NONE_EABI_GCC NAMES arm-none-eabi-gcc)
    if(ARM_NONE_EABI_GCC)
        set(KOS_STM32_TOOLCHAIN "${ARM_NONE_EABI_GCC}")
    endif()
endif()

set(KOS_STM32_FAMILY "STM32F4xx" CACHE STRING "STM32 HAL family macro (e.g., STM32F4xx)")
set(KOS_STM32_AVAILABLE OFF)
if(KOS_STM32_TOOLCHAIN)
    if(CMAKE_C_COMPILER MATCHES "arm-none-eabi")
        set(KOS_STM32_AVAILABLE ON)
        message(STATUS "STM32 cross toolchain active (${CMAKE_C_COMPILER}); enabling STM32 sample build.")
    else()
        message(STATUS "STM32 toolchain found at ${KOS_STM32_TOOLCHAIN} but host compiler is ${CMAKE_C_COMPILER}; configure with the STM32 toolchain to build the STM32 sample.")
    endif()
endif()

if(KOS_STM32_AVAILABLE)
    find_path(STM32_HAL_INCLUDE_DIR stm32f4xx_hal.h
        HINTS
            "${STM32_HAL_PATH}"
            "$ENV{STM32_HAL_PATH}"
    )
    if(NOT STM32_HAL_INCLUDE_DIR)
        message(STATUS "STM32 HAL headers not found (stm32f4xx_hal.h); skipping STM32 sample build.")
    else()
        include_directories(${STM32_HAL_INCLUDE_DIR})
        kos_add_generated_example(kos_sample_stm32 "stm32" "${KOS_SRC_PATH}/samples/stm32/first.oil")
        target_compile_definitions(kos_sample_stm32 PRIVATE ${KOS_STM32_FAMILY})
    endif()
endif()
