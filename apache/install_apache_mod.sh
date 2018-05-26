#!/bin/bash

# find installed library
lib=$(find -P /lib /usr/local/lib -name "libmod_msg.so" 2>/dev/null)
lib_path=$(dirname "$lib")
lib_file=$(basename "$lib")

# copy for apache naming convention
newlib="${lib_path}/mod_msg.so"
cp -a ${lib} ${newlib}

# install apache module and add apache module configuration
apxs -n msg -A -i ${newlib}

