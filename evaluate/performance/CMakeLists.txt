# add source dir
aux_source_directory(. SRC_LIST_PERFORMANCE)

# add executable
add_executable(PerformanceTestSet ${SRC_LIST_PERFORMANCE})

# set libs
target_link_libraries(PerformanceTestSet msgserver)
