# project settings
project(wcdx-msgserver)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

# set g++ compiler flags
set(CMAKE_CXX_FLAGS "-fPIC -Wall -pthread -O2 -fstack-protector -std=c++11")

# add executable
add_executable(${PROJECT_NAME} ${SRC_LIST})

# add unittest executable
add_executable(${PROJECT_NAME}.unittest ${SRC_LIST})
set_target_properties(${PROJECT_NAME}.unittest PROPERTIES COMPILE_DEFINITIONS "UNITTEST_MSGS")

# add custom targets
add_custom_target(ErrorHeader SOURCES ./Error.hpp)
add_custom_target(HelperHeader SOURCES ./Helper.hpp)
add_custom_target(GlobalHeader SOURCES ./Global.hpp)
add_custom_target(MemoryManagerHeader SOURCES ./MemoryManager.hpp)
add_custom_target(Constant SOURCES ./Constant.hpp)
add_custom_target(Vector SOURCES ./Vector.hpp)
add_custom_target(Debug SOURCES ./Debug.cpp)

# add subdirs
add_subdirectory(evaluate)
add_subdirectory(lib)
add_subdirectory(apache)
add_subdirectory(test)

# install
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
