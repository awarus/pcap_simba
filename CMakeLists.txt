cmake_minimum_required(VERSION 3.0)
project(ParserTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    parser.cpp
    ./util/utils.cpp
    ./net/net.cpp
    ./net/net_utils.cpp
    ./decode/net_decoder.cpp
    ./decode/simba_decoder.cpp
)

add_executable(parser ${SOURCES})
target_compile_options(parser PRIVATE -O2)
