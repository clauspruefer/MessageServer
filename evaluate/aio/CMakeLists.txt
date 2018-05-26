# add executables
add_executable(test_posix_aio TestPosixAIO.cpp TestPosixAIO.hpp)

# add dynamic libs
target_link_libraries(test_posix_aio rt)
