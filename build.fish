#!/usr/bin/env fish

set template "todo"

make clean
bear -- make TEMPLATE=$template
make run TEMPLATE=$template $argv
