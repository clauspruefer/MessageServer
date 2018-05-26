# add source dir
aux_source_directory(. SRC_LIST_LIB)

# add library
add_library(msgserver SHARED ${SRC_LIST_LIB})

# install
install(TARGETS msgserver DESTINATION lib)
