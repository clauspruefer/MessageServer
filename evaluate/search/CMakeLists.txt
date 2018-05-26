# set cflags
set(CMAKE_CXX_FLAGS "-g -fPIC -Wall -O0 -std=c++11")

# add executables
add_executable(test_search TestSearch.cpp)
