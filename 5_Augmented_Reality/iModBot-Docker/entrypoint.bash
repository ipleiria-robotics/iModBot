#! /bin/bash

echo 'export PATH="/usr/local/microxrcedds_agent-1.3.0/bin/:$PATH"' >> ~/.bashrc
echo 'export LD_LIBRARY_PATH=/usr/local/fastrtps-1.8.3/lib:/usr/local/microxrcedds_agent-1.3.0/lib:/usr/local/fastcdr-1.0.10/lib:/usr/local/microcdr-1.2.0/lib:/usr/local/microxrcedds_client-1.2.1/lib' >> ~/.bashrc
echo 'source /opt/ros/foxy/setup.bash' >> ~/.bashrc 
exec $@
