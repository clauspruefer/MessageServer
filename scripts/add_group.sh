#!/bin/sh

apache_user="www-data"

groupadd -g 2000 msgserver
usermod -a -G msgserver $apache_user


