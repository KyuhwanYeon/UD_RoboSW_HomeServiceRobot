#!/bin/sh


xterm  -e "source devel/setup.bash; export TURTLEBOT_GAZEBO_WORLD_FILE="$(pwd)/src/world/SecondWorld.world"; roslaunch turtlebot_gazebo turtlebot_world.launch" &
sleep 6
xterm  -e "source devel/setup.bash; export TURTLEBOT_GAZEBO_MAP_FILE="$(pwd)/src/map/map.yaml"; roslaunch turtlebot_gazebo amcl_demo.launch " &
sleep 7
xterm  -e "source devel/setup.bash; roslaunch turtlebot_rviz_launchers view_navigation.launch " 
