
  Air Hockey demo (Arturo Cepeda)
 =================================

Since I wanted to create a demo completely on my own (even the graphic content and sounds), 
I chose the option to develop something simple where I should work on all the fields which 
take part in the video games programming process (gameplay, UI, rendering, sound, physics, 
AI and network) instead of focusing my effort on one only discipline. Therefore I have 
developed an Air Hockey game: it is extremely funny and its rules are very simple to 
understand. 

I have paid particular attention to the modularity as I built this demo, because I know it 
is key to work in group. The game is made up of the following modules:

 - Game engine
   - Rendering engine (based on Direct3D)
   - Audio engine (based on FMOD Ex)
   - Network engine (based on WinSock2)
   - Scene manager
   - Timer

 - Air Hockey library
   - Gameplay module
   - AI module


In order to run the included executable files, you will need the following components to 
be installed:

 [Air Hockey.exe]

   - DirectX: http://www.microsoft.com/en-us/download/details.aspx?id=35


 [Air Hockey (Kinect OpenNI).exe]

   - DirectX: http://www.microsoft.com/en-us/download/details.aspx?id=35
   - Kinect sensor (hardware)
   - OpenNI: http://www.openni.org/Downloads/OpenNIModules.aspx
   - SensorKinect: https://github.com/avin2/SensorKinect/tree/unstable/Bin
   - PrimeSense NITE: http://www.openni.org/Downloads/OpenNIModules.aspx


 [Air Hockey (Kinect SDK).exe]

   - Only for Windows 7/8
   - DirectX: http://www.microsoft.com/en-us/download/details.aspx?id=35
   - Kinect sensor (hardware)
   - Kinect SDK: http://www.microsoft.com/en-us/kinectforwindows/develop/overview.aspx


Feel free to contact me if you have any problem: acepeda@usal.es

