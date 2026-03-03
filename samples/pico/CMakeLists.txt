cmake_minimum_required(VERSION 3.16)
project(kos_pico_sample LANGUAGES C CXX)

if (NOT DEFINED KOS_ROOT)
    get_filename_component(KOS_ROOT "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
endif()

set(KOSGEN_CPP_EXECUTABLE "${KOSGEN_CPP_EXECUTABLE}" CACHE FILEPATH "Path to kosgen_cpp executable")
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

set(SAMPLE_OIL "${CMAKE_CURRENT_LIST_DIR}/first.oil")
set(GEN_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(GEN_PREFIX "${GEN_DIR}/Os_Cfg")

add_custom_command(
    OUTPUT ${GEN_PREFIX}.c ${GEN_PREFIX}.h ${GEN_PREFIX}.info.txt
    COMMAND ${CMAKE_COMMAND} -E make_directory ${GEN_DIR}
    COMMAND ${KOSGEN_CPP_EXECUTABLE} --include ${KOS_ROOT}/config/oil -o ${GEN_PREFIX} ${SAMPLE_OIL}
    DEPENDS ${SAMPLE_OIL} ${KOSGEN_CPP_EXECUTABLE}
    COMMENT "Generating sample OS config from ${SAMPLE_OIL}"
    VERBATIM
)

add_custom_target(pico_oil_config ALL
    DEPENDS ${GEN_PREFIX}.c ${GEN_PREFIX}.h ${GEN_PREFIX}.info.txt
)
