#! /usr/bin/env bash

echo "==== TIME_MEASUREMENT ===="
sudo dmesg -c > /dev/null
sudo ./main < OS_PJ1_Test/TIME_MEASUREMENT.txt
dmesg | grep Project1
echo "=========================="

echo "========= FIFO_1 ========="
sudo dmesg -c > /dev/null
sudo ./main < OS_PJ1_Test/FIFO_1.txt
dmesg | grep Project1
echo "=========================="

echo "========= PSJF_2 ========="
sudo dmesg -c > /dev/null
sudo ./main < OS_PJ1_Test/PSJF_2.txt
dmesg | grep Project1
echo "=========================="

echo "========== RR_3 =========="
sudo dmesg -c > /dev/null
sudo ./main < OS_PJ1_Test/RR_3.txt
dmesg | grep Project1
echo "=========================="

echo "========== SJF_4 ========="
sudo dmesg -c > /dev/null
sudo ./main < OS_PJ1_Test/SJF_4.txt
dmesg | grep Project1

sudo dmesg -c > /dev/null
echo "=========================="