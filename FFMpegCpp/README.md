# FFMPEG C++

This directory contains a CMake project for building the rtp streaming libraries
needed for both the server-side and client-side application.

The output of building this project is two shared-object files:

1. librtpclient.so (Android) or rtpclient.dll (Windows)
1. librtpserver.so (Android) or rtpserver.dll (Windows)

Note that on windows you will need to rename the files to librtpclient.dll
and librtpserver.dll in order to use them.

## Build Instructions

This project has already been built and the resulting librtpclient.so
file is already in VRLiveStreamClient and librtpserver.so is already in
VRLivestreamServer. You only need to rebuild this if you have edited this CMake
project.

This project can either be built for Android or Windows. Linux and Mac operating
systems are not currently supported. If you wish to add build support for either
Linux or Mac, you will have to build or download FFMpeg shared objects for that
platform.

### Buid For Windows

You can build this project either using the Windows Command Prompt,
or alternatively you may use Visual Studio

#### PreRequisites

1. CMake
    - Cmake is a C/C++ tool for managing and building C/C++ projects
    - You can download an installer from [here](https://cmake.org/download/)
    - Note: When installing, make sure to add CMake to your PATH variable
1. A build toolchain, such as MinGW or Visual Studios. I recommend downloading
   and installing the free version of Visual Studios. I have not tested building
   this project with MinGW

#### Build using Windows Cmd Prompt

1. Open a windows command prompt in the FFMpegCpp directory

1. Make a build directory and enter it

    ```
    mkdir windows_build
    cd windows_build
    ```

1. Run Cmake

    ``` cmake .. ```

1. Build the project

    ``` cmake --build . --config Release ```

    - The output dll files should be in a created Release\ folder
    - Note that there will likely be warning related to the FFMpeg library.
      These can be safely ignored

#### Build in Visual Studio

Note that these instructions were created using Visual Studio 2019.
Exact steps may differ if using a different version

1. Open Visual Studio
1. Select on "Continue without code" in the "Get started" menu
1. Click on File->Open->Cmake...
1. Navigate to the FFMpegCpp folder and select the CMakeLists.txt file to open
1. Change the build type from Debug to Release
1. Click Build-BuildAll
1. There will likely be warnings for the FFMpeg library. These can be safely ignored
1. The built dll files should be under out\build\x64-Release\

### Build for Android (From Windows)

#### Prerequisites

Before you can target Android from your windows machine, you must have the following installed

 - CMake
    - Cmake is a C/C++ tool for managing and building c/C++ projects
    - You can download an installer from [here](https://cmake.org/download/)
    - Note: When installing, make sure to add CMake to your PATH variable
 - Android NDK
    - The Android NDK is the toolset that allows building C/C++ projects for Android
    - You can download the Android NDK from [here](https://developer.android.com/ndk/downloads)
    - Extract the downloaded zip file. Note that there is no "installation". Simply having the NDK
      downloaded and extracted is sufficient.
 - MinGW
    - Minimalist development environment for Windows.
    - Couldn't get this to build properly without it
    - It is likely already installed on your machine. If not, you can follow instructions [here](http://mingw.org/wiki/Getting_Started)

#### Steps

1. Open the windows CMD prompt
1. Navigate to the FFMpeg directory:

   ``` cd \Path\To\UnityProject\FFMpegCpp ```

1. Create a build directory:

   ``` mkdir build ```

1. Enter the build directory

   ``` cd build ```

1. Run CMake

   ``` cmake -G "MinGW Makefiles" -DTARGET_ANDROID=ON -DCMAKE_TOOLCHAIN_FILE=<TOOLCHAIN_PATH> -DANDROID_ABI=<TARGET_ABI> -DCMAKE_MAKE_PROGRAM=<PATH_TO_MAKE> ..\```

    - Replace <TOOLCHAIN_PATH> with the path to the android ndk toolchain. This file should be as in <android_ndk>\build\cmake\android.toolchain.cmake where <android_ndk> is the path to where you extracted the android ndk
    - Replace <TARGET_ABI> with the ABI you are targeting. Eg. one of arm64-v8a armeabi-v7a x86 or x86_64
    - Replace <PATH_TO_MAKE> with <android_ndk>\prebuilt\windows-x86_64\bin\make.exe

   A full example using my system directory structure would be:

   ``` cmake -G "MinGW Makefiles" -DTARGET_ANDROID=ON -DCMAKE_TOOLCHAIN_FILE=C:\Users\Kaleb\Documents\android-ndk-r21\build\cmake\android.toolchain.cmake -DANDROID_ABI=arm64-v8a -DCMAKE_MAKE_PROGRAM=C:\Users\Kaleb\Documents\android-ndk-r21\prebuilt\windows-x86_64\bin\make.exe .. ```

1. Build the project:

   ``` cmake --build . --config Release ```

   - There may be some Warnings related to the FFMpeg library - this is fine.
   - If there are no errors, you should now see .so files in the build directory

### Build For Android (From Linux)

These instructions assume that your machine already has basic build tools istalled such
as Cmake. If not, follow the instructions for your distro for how to install CMake
and other basic C++ build tools.

To build this project for Android, you will need to install the Android Native
Development Kit (NDK). The NDK is necessary to build any C++ based project for Android.

1. The NDK can be downloaded from [here](https://developer.android.com/ndk/downloads)

To build the project (on Linux):

1. Open a terminal
1. Make a build directory under the FFMpegCPP directory.

   ``` mkdir build ```

1. Enter the directory

   ``` cd build ```

1. Run cmake:

    ```
    cmake \
    -DCMAKE_TOOLCHAIN_FILE=<NDK-Toolchain-path> \
    -DANDROID_ABI=<target_abi> \
    ../
    ```

    Note that the NDK toolchain path should point to a toolchain.cmake file.
    The target_abi, indicates which version of android is being targeted. It can
    be one of any of the following:

      - armeabi-v7a  (32-bit arm)
      - arm64-v8a    (64-bit arm)
      - x86          (32-bit x86 based)
      - x86_64       (64-bit x86 based)

    At the time of writing, almost all new Android devices are 64-bit ARM devices

    As an example, to build for arm64-v8a, the command on my machine looks as:

    ```
    cmake \
    -DCMAKE_TOOLCHAIN_FILE=~/android-ndk-r21/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI=arm64-v8a \
    ../
    ```

1. Run make:

   ```
   make
   ```

   To use all cores of your machine to build (which is faster), you can
   add the -j<num-cores> argument:

   ```
   make -j$(nproc)
   ```

### After you build

After you build, you must move the resuling shared-object files to
the VRLivestream Unity projects.

For Android:

 - Move librtpclient.so to VRLivestreamClient\Plugins\Assets\Android

For Windows:

 - Move rtpclient.dll to VRLivestreamClient\Plugins\Assets\x86_64 and rename to librtpclient.dll
 - Move rtpserver.dll to VRLivestreamServer\Plugins\Assets\x86_64 and rename to librtpserver.dll

## Dependencies

This project extensively uses the FFMpeg library. In order to build this project
for a given platform, the corresponding FFMpeg shared libraries for that platform
must be present to link against. Currently, this project houses a build of FFMpeg
for Windows and Android. To build for other platforms, you will need to build
FFMpeg for that platform first, or download a build. The main download page
for FFmpeg is currently [here](http://ffmpeg.org/download.html)

### Lib sources

For Windows, we have downloaded builds from [here](https://ffmpeg.zeranoe.com/builds/).
Note that shared linking option is required. It is also currently possible to download
Mac 64-bit builds from here as well, though this has not been tested by this project

For Android, the FFMpeg library was built manually using a third-party GitHub project
that contains a build script. The Github repo is called ffmpeg-android-maker and
can currently be found [here](https://github.com/Javernaut/ffmpeg-android-maker.git).
Note that once this project was cloned, the scripts/ffmpeg/build.sh file was edited
to remove the line that disabled networking (--disable-network). Without removing
this line before building, the Android libs will not support any networking components
(eg. connecting to the RTP stream).
