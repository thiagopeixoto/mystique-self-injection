cmake_minimum_required(VERSION 3.10)
project(mystique LANGUAGES CXX ASM)
enable_language(ASM_MASM)
set(CMAKE_CXX_STANDARD 11)

add_definitions(-DUNICODE -D_UNICODE)

set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/rwxsection.asm
    ${PROJECT_SOURCE_DIR}/src/syscall.cpp
    ${PROJECT_SOURCE_DIR}/src/mystique.cpp
)

set(HEADERS
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

add_executable(mystique ${SOURCES})

target_include_directories(mystique ${HEADERS})
target_link_libraries(mystique Ws2_32.lib)