#!/bin/bash
if gcc -m32 -o domaci1.bin main.c scan.c -nostdlib -nostdinc -e main -Ttext=100 -static -fno-builtin ../../lib/lib.a -I ../../include; then
cd ../..
mkdir tmp_hd
sudo mount -o loop,offset=10240 hd_oldlinux.img tmp_hd
sudo cp apps/domaci1/domaci1.bin tmp_hd/root
sudo cp apps/domaci1/scancodes.tbl tmp_hd/root
sudo cp apps/domaci1/test1.tst tmp_hd/root
sudo cp apps/domaci1/ctrl.map tmp_hd/root
sleep .5
sudo umount tmp_hd
rmdir tmp_hd
else
echo 'GCC Compile failed!'
fi
