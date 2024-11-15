#!/bin/bash

echo "**** directory state before clean****"
ls -al
echo && echo

echo "**** directory initialize ****"
make clean
rm -rf foo bar *.link *.slink *.tmp *.old *.new

echo "**** directory state after clean ****"
ls -al
