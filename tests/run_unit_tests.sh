#!/bin/bash

set -e

clang -Wall unit_tests.c -o unit_tests
./unit_tests

g++ -Wall tests_against_stl.cpp -o tests_against_stl
./tests_against_stl
