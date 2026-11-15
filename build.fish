#!/usr/bin/env fish

set template circle

make clean
bear -- make TEMPLATE=$template
make run TEMPLATE=$template $argv
