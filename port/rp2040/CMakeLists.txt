# CMakeLists.txt for RP2040 (Raspberry Pi Pico) port
cmake_minimum_required(VERSION 3.20)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(FAMILY rp2040)
set(BOARD pico_sdk)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)


project(k_os_rp2040 C CXX ASM)

if(NOT DEFINED ENV{KOS_SRC_PATH})
    message(FATAL_ERROR "KOS_SRC_PATH is not set. Please point it to the k_os source root (e.g. C:/csprojects/k-os).")
endif()
get_filename_component(KOS_ROOT "$ENV{KOS_SRC_PATH}" ABSOLUTE)

pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(KOS_RP_TARGET "rp2040" CACHE STRING "RP target (rp2040|rp2350)")
set(KOS_RP_SYSCLK_HZ "125000000" CACHE STRING "System clock frequency in Hz for SysTick programming")

if(KOS_RP_TARGET STREQUAL "rp2350")
    # Default to softfp until FPU context save/restore is added (TODO).
    set(_KOS_CPU_FLAGS "-mcpu=cortex-m33 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=softfp")
    add_compile_definitions(PICO_RP2350=1)
else()
    set(_KOS_CPU_FLAGS "-mcpu=cortex-m0plus -mthumb")
    add_compile_definitions(PICO_RP2040=1)
endif()

# RP2040/2350 specific compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${_KOS_CPU_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections -Wall -Wextra")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")
add_compile_definitions(OSPORT_SYSCLK_HZ=${KOS_RP_SYSCLK_HZ})

set(INCLUDES
        ${CMAKE_CURRENT_LIST_DIR}
        ${KOS_ROOT}
        ${KOS_ROOT}/inc
        ${pico_mcp2515_SOURCE_DIR}/include
)

# RP2040 port specific sources
set(RP2040_PORT_SOURCES
        Os_Port_RP2040.c
)

set(KOS_CORE_SOURCES
        ${KOS_ROOT}/src/Com_Flt.c
        ${KOS_ROOT}/src/Com_If.c
        ${KOS_ROOT}/src/Com_Int.c
        ${KOS_ROOT}/src/Os_Alm.c
        ${KOS_ROOT}/src/Os_Ctr.c
        ${KOS_ROOT}/src/Os_Error.c
        ${KOS_ROOT}/src/Os_Evt.c
        ${KOS_ROOT}/src/Os_Exec.c
        ${KOS_ROOT}/src/Os_Intr.c
        ${KOS_ROOT}/src/Os_Mlq.c
        ${KOS_ROOT}/src/Os_Orti.c
        ${KOS_ROOT}/src/Os_ParamAccess.c
        ${KOS_ROOT}/src/Os_Res.c
        ${KOS_ROOT}/src/Os_SchT.c
        ${KOS_ROOT}/src/Os_Task.c
        ${KOS_ROOT}/src/Os_Vars.c
        ${KOS_ROOT}/src/Utl.c
)

if(EXISTS ${KOS_ROOT}/src/Com_Ext.c)
    list(APPEND KOS_CORE_SOURCES ${KOS_ROOT}/src/Com_Ext.c)
endif()

if (MSVC)
    # Use case-insensitive match to catch Debug/Release values
    if (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug$")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Od /Zi /D_DEBUG")
    else ()
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /O2 /Oy /DNDEBUG")
    endif ()
else ()
    #
    # clang or gcc
    #
    add_definitions(-D_GNU_SOURCE)

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable  -ffunction-sections")

    if ("${PROFILE}" STREQUAL "ON")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg -fprofile-arcs -ftest-coverage --coverage")
    endif ()

    if (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug$")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb -D_DEBUG -Og -fno-omit-frame-pointer -fno-optimize-sibling-calls")
        if (NOT CMAKE_HOST_WIN32)
            #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DFORTIFY_SOURCE=2 -fstack-protector-all --param=ssp-buffer-size=4 -fstack-check -fsanitize=address -fsanitize=undefined ")
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DFORTIFY_SOURCE=2 -fstack-protector-all --param=ssp-buffer-size=4 -fstack-check -fsanitize=undefined ")
            if (CMAKE_C_COMPILER_ID MATCHES "Clang")
                set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wthread-safety")
            endif ()
        endif ()
    else ()
        # Treat non-Debug as Release here
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNDEBUG -O2")
    endif ()
endif (MSVC)

# Generate sample OS configuration from OIL using kosgen_cpp
set(KOS_RP2040_SAMPLE_OIL "${KOS_ROOT}/samples/pico/first.oil" CACHE FILEPATH "Sample OIL file for RP2040 config generation")
set(KOS_RP2040_GEN_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated/rp2040_sample")
set(KOS_RP2040_GEN_PREFIX "${KOS_RP2040_GEN_DIR}/Os_Cfg")

if (NOT KOSGEN_CPP_EXECUTABLE)
    find_program(KOSGEN_CPP_EXECUTABLE NAMES kosgen_cpp kosgen_cpp.exe
        HINTS
            ${KOS_ROOT}/tools/oilcpp
            ${KOS_ROOT}
    )
endif()
if (NOT KOSGEN_CPP_EXECUTABLE)
    message(FATAL_ERROR "kosgen_cpp not found. Set KOSGEN_CPP_EXECUTABLE to the built binary (e.g. tools/oilcpp/kosgen_cpp.exe).")
endif()

add_custom_command(
    OUTPUT ${KOS_RP2040_GEN_PREFIX}.c ${KOS_RP2040_GEN_PREFIX}.h ${KOS_RP2040_GEN_PREFIX}.info.txt
    COMMAND ${CMAKE_COMMAND} -E make_directory ${KOS_RP2040_GEN_DIR}
    COMMAND ${KOSGEN_CPP_EXECUTABLE} --include ${KOS_ROOT}/config/oil -o ${KOS_RP2040_GEN_PREFIX} ${KOS_RP2040_SAMPLE_OIL}
    DEPENDS ${KOS_RP2040_SAMPLE_OIL} ${KOSGEN_CPP_EXECUTABLE}
    COMMENT "Generating RP2040 sample OS config from ${KOS_RP2040_SAMPLE_OIL}"
    VERBATIM
)

add_custom_target(rp2040_sample_config ALL
    DEPENDS ${KOS_RP2040_GEN_PREFIX}.c ${KOS_RP2040_GEN_PREFIX}.h ${KOS_RP2040_GEN_PREFIX}.info.txt
)

add_library(k_os_rp2040_port STATIC ${RP2040_PORT_SOURCES})
target_include_directories(k_os_rp2040_port PUBLIC ${INCLUDES} ${KOS_RP2040_GEN_DIR})
target_link_libraries(k_os_rp2040_port pico_stdlib hardware_spi)
add_dependencies(k_os_rp2040_port rp2040_sample_config)

add_library(k_os_rp2040_core STATIC ${KOS_CORE_SOURCES})
target_include_directories(k_os_rp2040_core PUBLIC ${INCLUDES} ${KOS_RP2040_GEN_DIR})
target_link_libraries(k_os_rp2040_core k_os_rp2040_port)
add_dependencies(k_os_rp2040_core rp2040_sample_config)

add_executable(k_os_rp2040_sample
        Os_RP2040_Sample.c
        ${KOS_RP2040_GEN_PREFIX}.c
)
add_dependencies(k_os_rp2040_sample rp2040_sample_config)
target_include_directories(k_os_rp2040_sample PRIVATE ${INCLUDES} ${KOS_RP2040_GEN_DIR})
target_link_libraries(k_os_rp2040_sample
        k_os_rp2040_core
        k_os_rp2040_port
        pico_stdlib
        hardware_spi
)
pico_add_extra_outputs(k_os_rp2040_sample)
