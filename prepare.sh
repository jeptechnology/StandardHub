#!/bin/sh

mkdir -p /home/fio/.docker
echo "{}" > /home/fio/.docker/config.json
mount -o bind config.json /usr/lib/docker/config.json