# these are ok to fail
docker network create --subnet=172.18.0.0/16 mynet123
xhost +local:docker


docker run \
-v $(pwd):/home/pew/rpcs3 \
-v "${CACHE:=/home/pew/dev/rpcs3/.cache}":/home/pew/.cache \
-v "${CONFIG:=/home/pew/dev/rpcs3/.config}":/home/pew/.config \
--network mynet123 --ip "${IP:=172.18.0.3}" \
-ti \
--env-file .travis/travis.env \
--gpus all,capabilities=graphics \
-v /tmp/.X11-unix:/tmp/.X11-unix -e DISPLAY=$DISPLAY \
-e XAUTHORITY=/tmp/.Xauthority \
-v ${XAUTHORITY}:/tmp/.Xauthority \
-v /home/pew/drives/bigdisk/home/pew/Downloads/:/downloads \
--env PULSE_SERVER=unix:/tmp/pulseaudio.socket \
--env PULSE_COOKIE=/tmp/pulseaudio.cookie \
--volume /var/run/pulse/native:/tmp/pulseaudio.socket \
--volume /var/run/pulse/native:/tmp/pulseaudio.socket \
--volume /home/pew/dev/rpcs3-docker/pulseaudio.conf:/etc/pulse/client.conf \
--privileged \
$(for dev in /dev/hidraw*; do echo -n "--device $dev "; done) \
rpewcs3:1.0 \
bash
# --security-opt apparmor:unconfined \
# --cap-add SYS_ADMIN --device /dev/fuse \
# --device /dev/uhid \
# --device /dev/uinput \
# /home/pew/run.sh
# $(for dev in /dev/input/*; do echo -n "--device $dev "; done) \
# $(for dev in /dev/usb/hiddev*; do echo -n "--device $dev "; done) \

# --privileged \