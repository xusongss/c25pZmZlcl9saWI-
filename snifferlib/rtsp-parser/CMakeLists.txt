project(rtsp-parser)
cmake_minimum_required(VERSION 2.8)

find_program(RAGEL "ragel")

function(ragel_gen in_rl)
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${in_rl}.cpp
        COMMAND ${RAGEL} -G2
            -o ${CMAKE_CURRENT_BINARY_DIR}/${in_rl}.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/${in_rl}.rl
            -I ${CMAKE_CURRENT_SOURCE_DIR}
            ${ARGN}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${in_rl}.rl)

    add_custom_target(ragel_${in_rl} DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${in_rl}.cpp)
endfunction(ragel_gen)

if(RAGEL)
    message(STATUS "ragel found at: ${RAGEL}")
else(RAGEL)
    message(FATAL_ERROR "ragel not found")
endif(RAGEL)

ragel_gen(http11_parser)

set_source_files_properties(
    ${CMAKE_CURRENT_BINARY_DIR}/http11_parser.cpp
    PROPERTIES GENERATED 1)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(rtsptest
    main.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/http11_parser.cpp
)
add_dependencies(rtsptest ragel_http11_parser)
