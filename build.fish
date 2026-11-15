#!/usr/bin/env fish

set template text

make clean
bear -- make TEMPLATE=$template
make run TEMPLATE=$template $argv
