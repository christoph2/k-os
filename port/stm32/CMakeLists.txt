# CMakeLists.txt for STM32 port
cmake_minimum_required(VERSION 3.16)

# Set STM32 family (can be overridden)
if(NOT DEFINED STM32_FAMILY)
    set(STM32_FAMILY STM32F4xx)
endif()

project(k_os_stm32 C ASM)

if(NOT DEFINED ENV{KOS_SRC_PATH})
    message(FATAL_ERROR "KOS_SRC_PATH is not set. Please point it to the k_os source root (e.g. C:/projects/k-os).")
endif()
get_filename_component(KOS_ROOT "$ENV{KOS_SRC_PATH}" ABSOLUTE)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# STM32 specific compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections -Wall -Wextra")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

# Add STM32 HAL path
if(DEFINED STM32_HAL_PATH)
    include_directories(${STM32_HAL_PATH}/Inc)
    include_directories(${STM32_HAL_PATH}/Inc/Legacy)
endif()

# Define STM32 family
add_definitions(-D${STM32_FAMILY})

include_directories(${KOS_ROOT}/inc)
include_directories(${KOS_ROOT}/port/stm32/gcc)

# STM32 port specific sources
set(STM32_PORT_SOURCES
        port/stm32/gcc/Os_Port_stm32.c
        port/stm32/gcc/startup_stm32f4xx.s  # Add appropriate startup file
)

add_library(k_os_stm32_port STATIC ${STM32_PORT_SOURCES})
