# VRLivestreamServer

This project runs the server side application. Note that the server is only 
compatible with Windows at this time. In the future, the video-encoding code
may be migrated away from using FFMpeg.Autogen so that it is cross-platform

## Setup

### NuGet Packages

1. Navigate to the [Nuget Packet Manager Github Page](https://github.com/GlitchEnzo/NuGetForUnity)
1. Go to the releases page.
1. Download the _.unitypackage_ of the latest release, which is 2.0.0 at the time of writing.
1. To install in Unity, Go to Assets->ImportPackets->CustomPackage... and then find the downloaded UnityPackage
1. Note: You may have to restart Unity after the installation to use the package.
1. Click on "NuGet->Manage Nuget Packages" from the top bar and check your installed packages.
   Assuming you have just installed NuGet, you should have no installed packages.
   If you have already installed some packages, take note of what packages you have installed
   Then, install the following packages:
  1. AsyncIO
  1. NetMQ
  1. FFMpeg.Autogen
1. Go back to installed packages. The NetMQ package sometimes installs other uneeded
   packages along side it that can cause build errors. Uninstall these so that 
   the three installed packages are left. (Keep any packages that you had prior to installed
   prior as well)

### FFMpeg Setup

The FFMpeg.Autgen library requires the ffmpeg Dlls for windows. 
Follow these steps:

1. Get the ffmpeg dlls for windows
    - Download the dll files from [here](https://ffmpeg.zeranoe.com/builds/)
    - Use the latest release build, your architecture, and the shared-linking version
    - Unzip the download, and copy the dlls in the "bin" folder into a new folder in the project called Assets\FFmpeg\bin\
    - The folder __must__ be named exactly this for now

## Other Notes:

Make sure to disable Windows Firewall before running the server