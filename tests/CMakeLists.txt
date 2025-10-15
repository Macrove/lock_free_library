#set(CMAKE_CXX_STANDARD                  20)
#set(CMAKE_CXX_STANDARD_REQUIRED         YES)
#set(CMAKE_CXX_EXTENSION                 OFF)
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Generate compile_commands.json for clangd" FORCE)

#find_package(fmt)

#target_link_libraries(${PROJECT_NAME} mylib fmt::fmt)

#add_subdirectory(utils)
#add_subdirectory(lock_free)
#add_subdirectory(locked)

add_executable(spsc_test spsc_test.cpp)

target_link_libraries(spsc_test
    PRIVATE
    lockfree
    locked
    utils
)
