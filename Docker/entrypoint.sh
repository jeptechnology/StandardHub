#!/bin/bash
mkdir -p /home/fio
chown fio /home/fio
chmod 755 /home/fio

echo "fio:fio" | chpasswd $chpasswdOptions

if [ -d "/home/fio/.ssh/keys" ]; then
cat /home/fio/.ssh/keys/* >> /home/fio/.ssh/authorized_keys
chown fio /home/fio/.ssh/authorized_keys
chmod 600 /home/fio/.ssh/authorized_keys
fi

# generate host keys if not present
ssh-keygen -A

# log to stderr (-e),
# If DOCKER_SSH_PORT specified in environent the use that, otherwise default to 2222 so as not to clash with host ssh port
/usr/sbin/sshd -e -p ${DOCKER_SSH_PORT:=2222} &

# keep launching a gdbserver on port 2159 until we are killed
while true; do gdbserver --multi localhost:2159; sleep 1; done