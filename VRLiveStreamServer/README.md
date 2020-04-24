# VRLivestreamServer

This project runs the a test scene as the server-side applicatoin.
Note that the server is only compatible with Windows at this time.

It Accepts TCP requests from clients to connect to an RTP stream.

## System Requirements

The minimum specs for the machine running the the server project is of course
dependent on the complexity of the Unity application that it is integrated with.
However, the extraction of the 360 video frames is quite intensive on the GPU.
A machine with a high quaility GPU is therefore recommended.

## Setup

There should be no required setup to run this test app.
However, the server-side code does depend on the dll that is created using
the FFMpegCpp project. The dll from this project is already placed
under \Plugins\x86_64\librtpserver.dll.

However, if you change the FFMpeg server implementation, you will need
to navigate to and delete the current dll and replace it with the new dll. Note
that the dll must be named exactly librtpserver.dll for Unity to find it.
If the Unity project is running while you do this, you will likely have to restart
the application for it to take effect.

## Creating a unitypackage

By creating a unitypackage, the streaming functionality can be easily integrated into other projects:

1. Select Assets->Export Package
1. Select plugins/x64_64
1. Select Scripts/LiveStream
1. Click the "export" button and choose a location to export to

## Using a unitypackage in a new project

To use the unitypackage and start livestreaming in a new project:

1. Select Assets->Import Package
1. Select the unitypackage created in the previous step
1. Attach the scripts ClientCamera.cs and ManualUIRenderer.cs to the camera you want to stream from
   Note that the ManualUIRenderer script is only necessary if your project contains Unity UI elements
   that should be part of the livestream

## Other Notes:

If the server does not appear to be running, you might want to disable your computer's firewall
and try again
