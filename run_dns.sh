# these are ok to fail
docker network create --subnet=172.18.0.0/16 mynet123

docker run \
-v /home/pew/dev/rpcs3/dnsmasq.conf:/etc/dnsmasq.conf \
--network mynet123 --ip "${IP:=172.18.0.7}" \
 --restart always \
-ti \
jpillora/dnsmasq
# --security-opt apparmor:unconfined \
# --cap-add SYS_ADMIN --device /dev/fuse \
# --device /dev/uhid \
# --device /dev/uinput \
# /home/pew/run.sh
# $(for dev in /dev/input/*; do echo -n "--device $dev "; done) \
# $(for dev in /dev/usb/hiddev*; do echo -n "--device $dev "; done) \

# --privileged \