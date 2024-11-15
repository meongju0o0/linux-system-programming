#!/bin/bash

echo "**** directory initialize ****"
make clean
./autoclean.sh
echo && echo

echo "**** directory state before run ****"
ls -al
echo && echo

echo "**** make c programs ****"
make
echo && echo

echo "**** run ./stat stat.c ****"
./stat stat.c
echo && echo

echo "**** run ./umask ****"
./umask
ls -al | grep foo
ls -al | grep bar
echo && echo

echo "**** run ./chmod ****"
./chmod
ls -al | grep foo
ls -al | grep bar
echo && echo

echo "**** run ./myln myln.c myln.c.link"
./myln myln.c myln.c.link
echo "cat myln.c:"
cat myln.c
echo
echo "cat myln.c.link:"
cat myln.c.link
echo && echo

echo "**** run ./mysln mysln.c mysln.c.slink ****"
./mysln mysln.c mysln.c.slink
echo "cat mysln.c:"
cat mysln.c
echo
echo "cat mysln.c.slink:"
cat mysln.c.slink
echo && echo

echo "**** run ./myrm myrm.c.tmp ****"
cp myrm.c myrm.c.tmp
ls -al | grep myrm.c
./myrm myrm.c.tmp
ls -al | grep myrm.c
echo && echo

echo "**** run ./mymv mymv.c.old mymv.c.new"
cp mymv.c mymv.c.old
ls -al | grep mymv.c
./mymv mymv.c.old mymv.c.new
ls -al | grep mymv.c
echo && echo

echo "**** run ./mymd tmp ****"
./mymd tmp
ls -al | grep tmp
echo && echo

echo "**** run ./myrd tmp ****"
./myrd tmp
ls -al | grep tmp
echo && echo

echo "**** run ./myls ****"
./myls
echo && echo

echo "**** run ./mylsr ****"
./mylsr
echo && echo

echo "**** run ./mycd ****"
./mycd tmp
echo && echo

echo "**** run ./mypwd ****"
./mypwd
echo && echo

echo "**** run ./mylsl ****"
./mylsl
echo && echo

echo "**** directory state after run ****"
ls -al
