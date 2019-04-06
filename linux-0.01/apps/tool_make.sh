#!/bin/bash
if gcc -m32 -o $1 $1.c -nostdlib -nostdinc -e main -Ttext=100 -static -fno-builtin ../lib/lib.a -I ../include; then
cd ..
mkdir tmp_hd
sudo mount -o loop,offset=10240 hd_oldlinux.img tmp_hd
sudo cp apps/$1 tmp_hd/bin
sleep .5
sudo umount tmp_hd
rmdir tmp_hd
rm apps/$1
else
echo 'GCC Compile failed!'
fi
