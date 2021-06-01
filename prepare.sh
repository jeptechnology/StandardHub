#!/bin/sh

mkdir -p /home/fio/.docker
cd  /home/fio/.docker
echo "{}" > config.json
mount -o bind config.json /usr/lib/docker/config.json