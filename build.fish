#!/usr/bin/env fish

set template "keyboard"

make clean
bear -- make TEMPLATE=$template
make run TEMPLATE=$template $argv
