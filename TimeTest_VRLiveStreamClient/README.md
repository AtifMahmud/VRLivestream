# VRLiveStreamClient

This Project is the client-side application for the project.

When you run the project, you input the ip address of the computer running
the server-side, and then this project will attempt to connect to the RTP stream
of the server.

## Build Instructions For Android

You will need to have the Android SDK installed. There are two options for this:

1. Install through UnityHub (suggested).
1. Install the Android SDK manually. You then have to point the Unity Editor
   to this version. (Found in Edit->Preferences->Android)

You will need to install Google Cardboard SDK for Unity.

1. Download the UnityPackage from [here](https://github.com/googlevr/gvr-unity-sdk/releases)
1. In the UnityEditor, go to Assets->ImportPacket->CustomPackage... and find you downloaded package

Additionally, some project settings need to be changed. Note that most of these
settings should already be set.

VR then needs to be enabled in the project:

1. Go to ProjectSettings->Player(AndroidIcon)->XRSettings
1. Enbale VR for the project (check the "Virtual Reality Supported" box)
1. Add two Virtual Reality SDKs: "None" and "Cardboard"
   - Note: Make sure "None" is at the top of the list so that VR is not turned on
    by default (The main page is a static 2D scene. VR is manually enabled in a script
    after the ip address of the server is entered)
1. Switch from XRSettings to OtherSettings and change the MinimumAPISupported to at least
   version 19

Note that if Virtual Reality Supported was already checked, you should now uncheck it
and then reenable it. There seems to be a Unity bug where the Google Cardboard
SDK doesn't load properly if its added while VR is already enabled and the cardboard
SDK already in the list

Some more settings need to be changed in order to build 64-bit apks:

1. Go to ProjectSettings->Player(AndroidIcon)->OtherSettings
1. Change scripting backend to IL2CPP
1. Change Target Architecture to ARM64


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

## Build instructions for Windows

There are no extra build steps for Windows. You should be able to build an
executable without changing any setings.

## Other Notes

The rtp streaming of the video is using ffmpeg shared-object files that are in the
Assets/Plugins/ folders. These shared-objects are build using the FFMpegCPP
project. If you need to update the streaming code, you will have to rebuild the shared-objects
and replace the ones in the Assets/Plugins/ folders. See the FFMpegCPP project
for instructions.

## Troubleshooting

If running the project on Windows and you encounter errors, you may want to try disabling your firewall.
