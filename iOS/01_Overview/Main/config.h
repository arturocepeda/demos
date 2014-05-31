
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  iOS Game Engine
//
//  Sample application
//
//  --- config.h ---
//
//////////////////////////////////////////////////////////////////


#ifndef _CONFIG_H_
#define _CONFIG_H_

//
// set frames per second
//
#define FPS 60


//
// set audio update frames
//
#define AUDIO_UPDATE_FRAMES 10


//
// set number of scenes
//
#define NUM_SCENES 1


//
// set supported orientations
//
#define PORTRAIT_UP
//#define PORTRAIT_DOWN
//#define LANDSCAPE_HOME_LEFT
//#define LANDSCAPE_HOME_RIGHT


//
// set number of fingers
//
#define MAX_FINGERS 5


//
// accelerometer settings
//
#define USE_ACCELEROMETER
#define ACCELEROMETER_UPDATE 0.05


//
// set shaders to use
//
#define USE_SHADER_HUD
#define USE_SHADER_TEXT
#define USE_SHADER_MESH_COLOR
#define USE_SHADER_MESH_TEXTURE

#endif
