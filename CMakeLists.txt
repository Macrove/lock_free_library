cmake_minimum_required(VERSION 3.25)
project(Lock_Free_Library)
set(CMAKE_CXX_STANDARD                  20)
set(CMAKE_CXX_STANDARD_REQUIRED         YES)
set(CMAKE_CXX_EXTENSION                 OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Generate compile_commands.json for clangd" FORCE)

#add_executable(${PROJECT_NAME} main.cpp)

#find_package(fmt)

#target_link_libraries(${PROJECT_NAME} mylib fmt::fmt)

add_subdirectory(utils)
add_subdirectory(lock_free)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    lockfree
    utils
)
