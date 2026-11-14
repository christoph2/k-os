cmake_minimum_required(VERSION 3.16)
project(kos_oilcpp LANGUAGES CXX)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	include(CTest)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(oilcpp_objects OBJECT
    src/main.cpp
    src/Preprocessor.cpp
    src/OilParser.cpp
    src/Generator.cpp
)

target_include_directories(oilcpp_objects PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(kosgen_cpp $<TARGET_OBJECTS:oilcpp_objects>)
target_link_libraries(kosgen_cpp PRIVATE)

if(BUILD_TESTING)
	add_test(NAME oilcpp_preprocessing
		COMMAND ${CMAKE_COMMAND}
			-DGENERATOR=$<TARGET_FILE:kosgen_cpp>
			-DTEST_DIR=${CMAKE_CURRENT_BINARY_DIR}/preprocessing-test
			-DROOT_DIR=${CMAKE_CURRENT_SOURCE_DIR}/../..
			-P ${CMAKE_CURRENT_SOURCE_DIR}/tests/preprocessing.cmake)
endif()

if (MSVC)
    target_compile_options(oilcpp_objects PRIVATE /W4 /permissive- /EHsc)
else()
    target_compile_options(oilcpp_objects PRIVATE -Wall -Wextra -Wpedantic)
endif()
