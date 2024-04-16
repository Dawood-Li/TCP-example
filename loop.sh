#!/bin/bash

# Usage:
# nohup ./loop.sh echo "hello" &

# 检查是否提供了可执行文件名
if [ -z "$1" ]; then
    echo "Usage: "
    echo "    $0 executable [args...]"
    echo "or: "
    echo "    nohup $0 executable [args...] &"
    exit 1
fi

executable="$1"
shift
program_args="$@"

for ((times=1; times <= 10; times++)); do
    echo "----------------------------------------------------------------"
    echo "$executable $program_args"
    echo "----------------------------------------------------------------"
    echo "Start $((times)) times at $(date +'%Y-%m-%d %H:%M:%S')"
    echo "----------------------------------------------------------------"
    $executable $program_args &
    wait $!
    echo "----------------------------------------------------------------"
    echo "Stop $((times)) times at $(date +'%Y-%m-%d %H:%M:%S')"
    echo "----------------------------------------------------------------"
    echo
    sleep 5
done
