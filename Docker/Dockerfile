############################################################
# Alpine linux Docker App development container
############################################################

FROM alpine:3.13

RUN apk --update add \
      bash \
      openssh \
      alpine-sdk \
      cmake \
      gdb \
   && echo "#include <unistd.h>" > /usr/include/sys/unistd.h \    
   && rm -rf /var/cache/apk/*

COPY entrypoint.sh .

# Create fio user:
# - add this user to the "dialout" group - this enables access to the UART devices
# - add this user to the "input" group of lmp (this is id:13 which is "cdrom" on alpine!) - this enables access to the button devices
RUN  addgroup -g 1000 1000 \
  && adduser -D -u 1000 -G 1000 fio \
  && adduser fio dialout \
  && adduser fio cdrom 

# create a space for the persistent storage
RUN mkdir -p /data/shared && chown fio /data/shared \
 && mkdir -p /data/private && chown fio /data/private

RUN chmod +x /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]
