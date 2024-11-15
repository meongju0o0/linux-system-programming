#!/bin/bash

echo "**** directory initialize ****"
make clean
rm -rf *_dst *_dst1 *_dst2 *.out *.t
echo && echo

echo "**** directory state before run ****"
ls -al
echo && echo

echo "**** make c programs ****"
make
echo && echo

echo "**** run ./mycp mycp_src mycp_dst ****"
./mycp mycp_src mycp_dst
echo "mcyp_src: "
cat mycp_src
echo "mycp_dst: "
cat mycp_dst
echo && echo

echo "**** run ./hole ****"
./hole
echo "file.hole: "
cat file.hole
echo && echo

echo "**** run ./list list.c ****"
./list list.c
echo && echo

echo "**** run ./tcp1 tcp1_src tcp1_dst ****"
./tcp1 tcp1_src tcp1_dst
echo "tcp1_src: "
cat tcp1_src
echo "tcp1_dst: "
cat tcp1_dst
echo && echo

echo "**** run ./tcp2 tcp2_src tcp2_dst ****"
./tcp2 tcp2_src tcp2_dst
echo "tcp2_src: "
cat tcp2_src
echo "tcp2_dst: "
cat tcp2_dst
echo && echo

echo "**** run ./mycp2 mycp2_src mycp2_dst ****"
./mycp2 mycp2_src mycp2_dst
echo "mycp2_src: "
cat mycp2_src
echo "mycp2_dst: "
cat mycp2_dst
echo && echo

echo "**** run ./split split_src split_dst1 split_dst2 ****"
./split split_src split_dst1 split_dst2
echo "split_src: "
cat split_src
echo "split_dst1: "
cat split_dst1
echo
echo "split_sdt2: "
cat split_dst2
echo && echo

echo "**** run ./merge merge_src1 merge_src2 merge_dst ****"
./merge merge_src1 merge_src2 merge_dst
echo "merge_src1: "
cat merge_src1
echo "merge_src2: "
cat merge_src2
echo "merge_dst: "
cat merge_dst
echo && echo

echo "**** run ./conv test.in test.out ****"
./conv test.in test.out
echo "test.in: "
cat test.in
echo
echo "test.out: "
cat test.out
echo && echo

echo "**** run ./access test.out ****"
./access test.out
echo && echo

echo "**** run ./tab tab.c"
./tab tab.c
echo "tab.c: "
cat tab.c
echo
echo "tab.c.t: "
cat tab.c.t
echo && echo

echo "**** run ./upper upper_src ****"
./upper upper_src
echo && echo

echo "**** directory state after run ****"
ls -al
