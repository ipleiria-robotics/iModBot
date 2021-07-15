#!/bin/bash

# get parameter from system
user=`id -un`
dir=`pwd`
# start sharing xhost
xhost +local:root

# run docker
docker run \
  --net=host \
  --ipc=host \
  --privileged \
  -v ${dir}/ROS2_Workspace:/home/${user}/work/ROS2_Workspace \
  -v ${dir}/Unity_Workspace:/home/${user}/work/Unity_Workspace \
  -v ${dir}/VScode_Workspace:/home/${user}/work/VScode_Workspace \
  -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
  -v $HOME/.Xauthority:$docker/.Xauthority \
  -v $HOME/work:$HOME/work \
  -e XAUTHORITY=$home_folder/.Xauthority \
  -e DISPLAY=$DISPLAY \
  -e QT_X11_NO_MITSHM=1 \
  -it --name "docker-imodbot" ${user}/docker-imodbot