cmake_minimum_required(VERSION 3.10)

project(TransportationSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

set(SOURCES
    src/main.cpp
    src/transport_system.cpp
)

add_executable(app ${SOURCES})

file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
