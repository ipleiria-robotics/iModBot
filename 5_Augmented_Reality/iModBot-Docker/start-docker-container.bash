#!/bin/bash

# get parameter from system
user=`id -un`
dir=`pwd`
# start sharing xhost
xhost +local:root

# run docker
docker start \
docker-imodbot