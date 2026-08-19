#!/bin/bash

source=(
)
flags=(
# here some flags I use
    "-std=c++20"
    "-Wall"
    "-Wextra"
    "-Wpedantic"
    "-Werror"
    "-g"
)
libs=(
    -lftxui-component
    -lftxui-dom
    -lftxui-screen
)
oname='proj_0.0.1'

#source filling
mapfile -d $'\0' source < <(find "$(pwd)/src" -type f -name "*.cpp" -print0)

#compiling project
g++ "${flags[@]}" -o "$oname" "${source[@]}" "${libs[@]}" || exit 1

#run or no
run=''
read -r -p "run? (Y/n)" run
case "$run" in
    ""|[Yy])
        clear
        "./$oname"
        ;;
    *)
        echo "cancelled"
        ;;
esac
