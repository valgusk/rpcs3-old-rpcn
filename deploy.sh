mv build "build$(stat -c '%w' build)" || echo "no build"

DEPLOY_APPIMAGE="true" \
BUILD_SOURCEVERSION="pew_rpcs3" \
NAME="PEW build" \
COMPILER="clang" \
docker run \
-v $(pwd):/rpcs3 \
-v "/home/pew/dev/.ccache":/root/.ccache \
--env-file .ci/travis.env \
rpcs3/rpcs3-travis-xenial:1.4 \
/bin/bash -ex /rpcs3/.ci/build-linux.sh