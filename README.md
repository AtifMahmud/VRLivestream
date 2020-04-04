# VRLivestream

This project contains a method to serve 360 degree video from an arbitrary Unity project.
The livestream is an RTP stream that is only accessible on the local network of the server.

These subprojects make up this project. See their corresponding READMEs for more detail.

1. VRLivestreamServer: This project contains a Unity project that extracts
   and streams a 360 video of a sample scene. However, its can be integrated into
   any arbitrary Unity scene - see its README for more detail.

2. VRLiveStreamClient-Desktop: This project is a Unity application that can be run
   on Windows or Android that connects to and displays the 3D livestream from
   the server.

3. FFMpegCpp: This is a C++ Cmake project that contains the underyling streaming
   code. It is used by both the server and client projects.

4. MainMallDemo: This is a demo project where you can walk around Main Mall. 

5. VRLiveStreamClien-Android: Has an apk to test the client side functionality. 



## Instructions

### Simple Demo

1. Launch the MainMallDemo project
2. Find out the IP address of your computer
3. Make sure that your phone is connected to the same wi-fi as your computer
4. Enter playmode on your computer
5. Open the app on your phone, and connect to the IP address of your computer
6. Try it out!