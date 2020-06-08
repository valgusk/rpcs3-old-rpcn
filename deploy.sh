
DEPLOY_APPIMAGE="true" \
BUILD_SOURCEVERSION="pew_rpcs3" \
NAME="PEW build" \
COMPILER="clang" \
docker run \
-v $(pwd):/rpcs3 \
-v "/home/pew/dev/.ccache":/root/.ccache \
--env-file .travis/travis.env \
rpcs3/rpcs3-travis-xenial:1.2 \
/bin/bash -ex /rpcs3/.travis/build-linux.bash