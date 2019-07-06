#!/bin/bash

hz="$(cat /proc/cpuinfo | grep --max-count=1 GHz | awk '{print $9}')"
echo $hz > hz.txt