#!/usr/bin/env fish

set template config

make clean
bear -- make TEMPLATE=$template
make run TEMPLATE=$template $argv
