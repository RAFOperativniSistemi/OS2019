#!/bin/bash
cd ..
mkdir tmp_hd
sudo mount -o loop,offset=10240 hd_oldlinux.img tmp_hd
sudo cp apps/$1 tmp_hd/root/$1
sleep .5
sudo umount tmp_hd
rmdir tmp_hd

