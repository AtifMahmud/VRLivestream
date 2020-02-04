# FFMPEG C++ 

This directory contains a CMake project for building the decoding shared libary
for Android. Note that this project will also build a shared object for an encoding 
library, which is not currently being used. In the future, we can add build support 
for Windows and Linux (currently just Android) so that we can switch the Server and the 
Desktop client to use this library instead of the FFMpeg.Autogen NuGet package.

## Build Instructions

To build this project for Android, you will need to install the Android Native
Development Kit (NDK)

1. The NDK can be downloaded from [here](https://developer.android.com/ndk/downloads)

To build the project (on Linux, Windows instructions in progress):

1. Open a terminal 

