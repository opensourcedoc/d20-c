#!/usr/bin/env bats

PROGRAM=d20
DIST_DIR=dist

@test "Test 1d6" {
    run ./$DIST_DIR/$PROGRAM
    [ "$output" -ge "1" -a "$output" -le "6" ]
}

@test "Test 1d8" {
    run ./$DIST_DIR/$PROGRAM -r 1 -d 8
    [ "$output" -ge "1" -a "$output" -le "8" ]
}

@test "Test 2d6+1" {
    run ./$DIST_DIR/$PROGRAM -r 2 -d 6 -m 1
    [ "$output" -ge "3" -a "$output" -le "13" ]
}

@test "Test 1d6 by string" {
    run ./$DIST_DIR/$PROGRAM "1d6"
    [ "$output" -ge "1" -a "$output" -le "6" ]
}

@test "Test 2d8+1 by string" {
    run ./$DIST_DIR/$PROGRAM "2d8+1"
    [ "$output" -ge "3" -a "$output" -le "17" ]
}
