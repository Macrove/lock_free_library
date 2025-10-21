# plot chart using csv of something
# understand this difference and U test thing
# lock it on a cpu and figure out other ways to remove variance

find_package(benchmark REQUIRED)

add_executable(l_spsc_bm l_spsc_bm.cpp)
add_executable(lf_spsc_bm lf_spsc_bm.cpp)

target_link_libraries(l_spsc_bm
    PRIVATE
    locked
    utils
    benchmark::benchmark
)
target_link_libraries(lf_spsc_bm
    PRIVATE
    lockfree
    utils
    benchmark::benchmark
)
