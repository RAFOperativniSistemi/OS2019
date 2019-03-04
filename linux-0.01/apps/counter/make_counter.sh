#!/bin/bash
if gcc -m32 -o counter.bin counter.c counter_test.c -nostdlib -nostdinc -e main -Ttext=100 -static -fno-builtin ../../lib/lib.a -I ../../include; then
cd ../..
mkdir tmp_hd
sudo mount -o loop,offset=10240 hd_oldlinux.img tmp_hd
sudo cp apps/counter/counter.bin tmp_hd/root
sleep .5
sudo umount tmp_hd
rmdir tmp_hd
rm apps/counter/counter.bin
else
echo 'GCC Compile failed!'
fi
