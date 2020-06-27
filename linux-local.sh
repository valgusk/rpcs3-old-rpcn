#!/bin/sh -ex
export QTVER=5.14.2
export QTVERMIN=514
export LLVMVER=10
export GCCVER=9

export CLANG_BINARY=clang
export CLANGXX_BINARY=clang++
export LLD_BINARY=lld
export GCC_BINARY=gcc
export GXX_BINARY=g++

export DEPLOY_APPIMAGE="true"
export BUILD_SOURCEVERSION="pew_rpcs3"
export NAME="PEW build"
export COMPILER="clang"

# Setup Qt variables
export QT_BASE_DIR=/home/pew/drives/Mastocyte/dev/QT-5.14/5.14.2/gcc_64
export PATH=$QT_BASE_DIR/bin:$PATH
export LD_LIBRARY_PATH=$QT_BASE_DIR/lib/x86_64-linux-gnu:$QT_BASE_DIR/lib
export QMAKE=$QT_BASE_DIR/bin/qmake


if [ "$DOBUILD" = "true" ]; then

    # Pull all the submodules except llvm, since it is built separately and we just download that build
    # Note: Tried to use git submodule status, but it takes over 20 seconds
    # shellcheck disable=SC2046
    git submodule -q update --init $(awk '/path/ { print $3 }' .gitmodules)

    # Download pre-compiled llvm libs
    # curl -sLO https://github.com/RPCS3/llvm-mirror/releases/download/custom-build/llvmlibs-linux.tar.gz
    # mkdir llvmlibs
    # tar -xzf ./llvmlibs-linux.tar.gz -C llvmlibs

    mv build "build$(stat -c '%w' build)" || echo "no build"
    # rm -rf build
    mkdir build
    cd build || exit 1

    if [ "$COMPILER" = "gcc" ]; then
        # These are set in the dockerfile
        export CC=${GCC_BINARY}
        export CXX=${GXX_BINARY}
        export LINKER=gold
        # We need to set the following variables for LTO to link properly
        export AR=/usr/bin/gcc-ar-$GCCVER
        export RANLIB=/usr/bin/gcc-ranlib-$GCCVER
        export CFLAGS="-fuse-linker-plugin"
    else
        export CC=${CLANG_BINARY}
        export CXX=${CLANGXX_BINARY}
        export LINKER=lld
        export AR=/usr/bin/llvm-ar
        export RANLIB=/usr/bin/llvm-ranlib
    fi

    export CFLAGS="$CFLAGS -Ofast -fuse-ld=${LINKER}"

    cmake ..                                               \
        -DCMAKE_INSTALL_PREFIX=/usr                        \
        -DUSE_NATIVE_INSTRUCTIONS=ON                      \
        -DUSE_PRECOMPILED_HEADERS=OFF                      \
        -DBUILD_LLVM_SUBMODULE=ON                          \
        -DCMAKE_C_FLAGS="$CFLAGS"                          \
        -DCMAKE_CXX_FLAGS="$CFLAGS"                        \
        -DCMAKE_AR="$AR"                                   \
        -DCMAKE_RANLIB="$RANLIB"                           \
        -G Ninja

    # -DCMAKE_PREFIX_PATH=/usr/                        
    ninja; build_status=$?;

    cd ..

    if [ "$build_status" -eq 0 ]; then
        echo "build successful"
    fi
fi

if [ "$DOBUNDLE" = "true" ]; then
    cd build || exit 1

    # rm -f linuxdeploy*.AppImage

    if [ ! -f "linuxdeploy-x86_64.AppImage" ]; then
        wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
        wget https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage
        chmod +x linuxdeploy*.AppImage
    fi

    if [ -d "appdir" ]; then
        rm -rf appdir
    fi

    DESTDIR=appdir ninja install

    ./linuxdeploy-x86_64.AppImage --appdir appdir --library=/usr/lib/libxcb.so
    ./linuxdeploy-plugin-qt-x86_64.AppImage --appdir appdir --extra-plugin=libxcb.so
    ./linuxdeploy-x86_64.AppImage --appimage-extract

    rm ./appdir/usr/lib/libxcb*
    cp "$(readlink -f /lib64/libnsl.so.1)" ./appdir/usr/lib/libnsl.so.1

    squashfs-root/plugins/linuxdeploy-plugin-appimage/usr/bin/appimagetool appdir --no-appstream

    cd ..
fi


if [ "$DORUN" = "true" ]; then
    cd build

    ./bin/rpcs3
fi