cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(BUILD_TESTS OFF CACHE BOOL "Build tests")


set(CONAN_PROFILE ${CMAKE_SOURCE_DIR}/conanProfile)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/generators/conan_toolchain.cmake)
set(UTILS_DIR ${CMAKE_SOURCE_DIR}/utils)

find_program(CONAN conan)
if(NOT EXISTS ${CONAN})
    message(FATAL_ERROR "Conan is missing.")
else()
    message("Find conan: ${CONAN}")
    execute_process(COMMAND ${CONAN} install 
                    --profile=${CONAN_PROFILE} 
                    --profile:build=${CONAN_PROFILE}
                    --build=missing
                    ${CMAKE_SOURCE_DIR}
                    RESULT_VARIABLE result
    )
   
    if(NOT ${resut} EQUAL 0)
        message(FATAL_ERROR "conan install command failed.")
    endif()
endif()



project(Apps)

add_subdirectory(apps/app)
