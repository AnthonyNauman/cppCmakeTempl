cmake_minimum_required(VERSION 3.16)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Test coverage
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -fprofile-arcs -ftest-coverage")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -Wall -W -fprofile-arcs -ftest-coverage")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_BUILD_TYPE Release)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

project(tests LANGUAGES CXX)

find_package(GTest REQUIRED)

set(TESTED_HEADER_FOLDER 
	${CMAKE_SOURCE_DIR}/apps/app/include
)
set(TESTED_SOURCE_FOLDER 
	${CMAKE_SOURCE_DIR}/apps/app/src
)

set(TESTED_SOURCES 
	${TESTED_HEADER_FOLDER}/someClass.hpp
	${TESTED_SOURCE_FOLDER}/someClass.cpp
)

file( GLOB_RECURSE SOURCES FOLLOW_SYMLINKS
	${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/*/include/*.h
   	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
   	# ${CMAKE_CURRENT_SOURCE_DIR}/*/src/*.c
)

add_executable(${PROJECT_NAME} ${SOURCES} ${TESTED_SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
	${GTEST_INCLUDE_DIRS}
    ${TESTED_HEADER_FOLDER}
    ${TESTED_SOURCE_FOLDER}
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
    
    GTest::gtest_main
    GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})

# add_custom_command(
#     TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#             ${CMAKE_CURRENT_SOURCE_DIR}/data 
#             ${CMAKE_CURRENT_BINARY_DIR}/data)

# Create the gcov target. Run coverage tests with 'make gcov'


set(OBJECT_DIR ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${PROJECT_NAME}.dir/__/src)
# add_subdirectory(tests)

find_program(GCOV gcov REQUIRED)
find_program(LCOV lcov REQUIRED)
find_program(GENHTML_PATH genhtml REQUIRED)

add_custom_target(gcov
    COMMAND mkdir -p gcoverage
    COMMAND ${CMAKE_MAKE_PROGRAM} tests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

message("CURR BIN DIR: " ${CMAKE_CURRENT_BINARY_DIR})
message("OBJECT DIR: " ${OBJECT_DIR})

# TODO: fix this
add_custom_command(TARGET gcov
    COMMAND echo "=================== GCOV ===================="
    COMMAND ${GCOV} -b ${TESTED_SOURCES} -o ${OBJECT_DIR} 
    COMMAND echo "-- Source diretorie: ${CMAKE_SOURCE_DIR}/src/"
    COMMAND echo "-- Coverage files have been output to ${CMAKE_BINARY_DIR}/gcoverage"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/gcoverage
)

add_dependencies(gcov ${PROJECT_NAME})

add_custom_target(lcov
    COMMAND mkdir -p lcoverage
    COMMAND ${CMAKE_MAKE_PROGRAM} tests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_command(TARGET lcov
    COMMAND echo "=================== LCOV ===================="
    COMMAND echo "-- Passing lcov tool under code coverage"
    COMMAND lcov --capture --directory ${OBJECT_DIR} --output-file lcoverage/main_coverage.info -b .
    COMMAND echo "-- Remove undesired files from main_coverage"
    COMMAND lcov --remove lcoverage/main_coverage.info '*/usr/include/*' '*/gtest/*' -o lcoverage/filtered_coverage.info
    COMMAND echo "-- Generating HTML output files"
    COMMAND genhtml lcoverage/filtered_coverage.info --output-directory lcoverage
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Make sure to clean up the coverage folder
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES gcoverage)

# Create the gcov-clean target. This cleans the build as well as generated 
# .gcda and .gcno files. ${CMAKE_MAKE_PROGRAM} is /usr/bin/make
add_custom_target(refresh
    COMMAND ${CMAKE_MAKE_PROGRAM} clean
    COMMAND rm CMakeCache.txt
    COMMAND rm -f ${OBJECT_DIR}/*.gcno
    COMMAND rm -f ${OBJECT_DIR}/*.gcda
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)