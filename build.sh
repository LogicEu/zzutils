#!/bin/bash

cmd() {
    echo "$@" && $@
}

build() {
    [ ! -d $1 ] && echo "$0: could not find directory: $1" && exit
    [ ! -d bin/ ] && cmd mkdir -p bin/
    cmd gcc $1/$1.c -o bin/$1 -std=c89 -O2 -Wall -Wextra -pedantic
}

build_all() {
    for f in *; do [ -d $f ] && build $f; done
}

clean() {
    [ -d bin/ ] && cmd rm -rf bin/
}

(( $# < 1 )) && echo -e "$0: missing argument.\nSee 'help' for more information" && exit

for arg in "$@"
do
    case "$arg" in
        "help")
            echo "$0:"
            echo -e "<arg>\t\t: compile specific utility inside bin/ directory"
            echo -e "all\t\t: compile all utilities inside bin/ directory"
            echo -e "install\t\t: install utilities in system"
            echo -e "uninstall\t: uninstall utilities from system"
            echo -e "clean\t\t: clean local compiled binaries"
            echo -e "-help\t\t: dissplay help message";;
        "all")
            build_all;;
        "install")
            build_all && mv bin/* /usr/local/bin/;;
        "uninstall")
            for f in *; do [ -d $f] && cmd rm -f /usr/local/bin/$f; done;;
        "clean")
            clean;;
        *)
            build $arg;;
    esac
done

