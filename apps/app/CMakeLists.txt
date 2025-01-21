cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (BUILD_TESTS)
    message(STATUS "Test build")
    add_compile_definitions(TEST_BUILD)
endif()

find_package(spdlog REQUIRED)


project(app LANGUAGES CXX)

add_executable(${PROJECT_NAME}
    # ${SRC}
    src/main.cpp
    src/someClass.cpp
    include/someClass.hpp
)
target_include_directories(${PROJECT_NAME} PRIVATE include)


target_link_libraries(${PROJECT_NAME} 
    PRIVATE 

    spdlog::spdlog
)

if (BUILD_TESTS)
    add_subdirectory(tests)
endif()
