# include dirs
include_directories(/usr/include/apache2 /usr/include/apr-1.0)

# set g++ compiler flags
set(CMAKE_CXX_FLAGS "-fPIC -Wall -O2 -fstack-protector -std=c++11")

# add source dir
aux_source_directory(. SRC_LIST_LIB)

# add library
add_library(mod_msg SHARED ${SRC_LIST_LIB})

# set libs
target_link_libraries(mod_msg msgserver)

# install
install(TARGETS mod_msg DESTINATION lib)
install(CODE "execute_process(COMMAND ldconfig)")
install(CODE "execute_process(COMMAND ./apache/install_apache_mod.sh)")
