#!/bin/bash

echo "**** directory state before clean****"
ls -al
echo && echo

echo "**** directory initialize ****"
make clean
rm -rf *_dst *_dst1 *_dst2 *.out *.t
echo && echo

echo "**** directory state after clean ****"
ls -al
