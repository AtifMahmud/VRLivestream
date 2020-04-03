# VRLiveStreamClient

This Project is for building the client side application for Android.

## Setup

This project depends on NetMQ and AsyncIO which are NuGet packages. See the 
VRLivestreamServer READMe for instructions for how to install NuGet and these
pacakges. DO _NOT_ install FFmpeg.Autogen in this project - it is not needed
MAKE SURE TO UNINSTALL ALL OTHER WEIRD PACKAGES THAT GET INSTALLED.
You should only have NetMQ and AsyncIO, nothing else.

## Build Instructions

You will need to have the Android SDK installed. There are two options for this:

1. Install through UnityHub. I had troubles getting Unity to see this version
   so I used the other option
1. Install the Android SDK manually. You then have to point the Unity Editor
   to this version. (Found in Edit->Preferences->Android)

You will need to install Google Cardboard SDK for Unity.

1. Download the UnityPackage from [here](https://github.com/googlevr/gvr-unity-sdk/releases)
1. In the UnityEditor, go to Assets->ImportPacket->CustomPackage... and find you downloaded package

VR then needs to be enabled in the project:

1. Go to ProjectSettings->Player(AndroidIcon)->XRSettings
1. Enbale VR for the project (check the "Virtual Reality Supported" box)
1. Add two Virtual Reality SDKs: "None" and "Cardboard"
   - Note: Make sure "None" is at the top of the list so that VR is not turned on
    by default (The main page is a static 2D scene. VR is manually enabled in a script
    after the ip address of the server is entered)
1. Switch from XRSettings to OtherSettings and change the MinimumAPISupported to at least
   version 19

One more setting needs to be changed:

1. Go to ProjectSettings->Player(AndroidIcon)->OtherSettings
1. Make sure the API Compatibility Level is 4.0 or NetMQ will not build

To Build:

1. Go to File->BuildSettings and Switch to Android
1. Add the ClientMain and ClientCardboard scenes to the build
   - Note: The ClientMain must be above the ClientCardboard scene so that it is loaded
    first.
1. You should then be able to build the APK file.

The APK file can be moved onto your phone and installed. You may have to enable developer
options in order to install a raw apk file outside of the play store. Also, if you make 
a new APK file and want to reinstall, you will have to uninstall the original first
or the new version will not install.

## Other Notes

The decoding of the video is using ffmpeg shared-object files that are in the 
Assets/Plugins/Android folder. These shared-objects are build using the FFMpegCPP 
project. If you need to update the Decoding code, you will have to rebuild the shared-objects
and replace the ones in the Assets/Plugins/Android folder. See the FFMpegCPP project
for instructions.
