# VRLivestream

This project contains a method to serve 360 degree video from an arbitrary Unity project.
The livestream is an RTP stream that is only accessible on the local network of the server.

Three subprojects make up this project. See their corresponding READMEs for more detail.

1. VRLivestreamServer: This project contains a Unity project that extracts
   and streams a 360 video of a sample scene. However, its can be integrated into
   any arbitrary Unity scene - see its README for more detail.
1. VRLiveStreamClient: This project is a Unity application that can be run
   on Windows or Android that connects to and displays the 3D livestream from
   the server.
1. FFMpegCpp: This is a C++ Cmake project that contains the underyling streaming
   code. It is used by both the server and client projects.
