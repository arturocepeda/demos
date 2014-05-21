
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Rendering Engine (OpenGL ES)
//
//  --- GERenderingShadersES20.cpp ---
//
//////////////////////////////////////////////////////////////////


#include "GERenderingShadersES20.h"
#include "GEDevice.h"

#include <GLES2/gl2.h>
#include <android/log.h>
#include <stdio.h>


//
//  GEShader
//
GEShader::~GEShader()
{
   glDeleteShader(iID);
}

void GEShader::load(const char* sFilename, const char* sExt)
{
   // read source file   
   char sPath[128];
   sprintf(sPath, "Shaders/%s.%s", sFilename, sExt);
   unsigned int iFileLength = GEDevice::getFileLength(sPath);
   
   unsigned char* sShaderSource[1];
   sShaderSource[0] = new unsigned char[iFileLength + 1];

   GEDevice::readFile(sPath, sShaderSource[0], iFileLength);
   sShaderSource[0][iFileLength] = '\0';
   char** sShaderSourceAsDoublePtr = reinterpret_cast<char**>(sShaderSource);
   glShaderSource(iID, 1, const_cast<const char**>(sShaderSourceAsDoublePtr), 0);
   
   // compile shader
   glCompileShader(iID);
   glGetShaderiv(iID, GL_COMPILE_STATUS, &iStatus);

   delete[] sShaderSource[0];
   
   if(!check())
   {
      // get info log length
      GLint iLogLength;
      glGetShaderiv(iID, GL_INFO_LOG_LENGTH, &iLogLength);
      
      // get info log contents
      GLchar* sLog = new GLchar[iLogLength];            
      glGetShaderInfoLog(iID, iLogLength, NULL, sLog);
      
      // show info log
      __android_log_print(ANDROID_LOG_VERBOSE, "GameEngine", "Shader '%s.%s': compiling error\n%s", sFilename, sExt, sLog);
      
      delete[] sLog;
   }
}

GLuint GEShader::getID()
{
   return iID;
}

bool GEShader::check()
{
   return (iStatus != 0);
}


//
//  GEVertexShader
//
GEVertexShader::GEVertexShader(const char* Filename)
{
   // get shader ID
	iID = glCreateShader(GL_VERTEX_SHADER);
   iStatus = 0;
   
   // load shader
   load(Filename, "vsh");
}


//
//  GEFragmentShader
//
GEFragmentShader::GEFragmentShader(const char* Filename)
{
   // get shader ID
   iID = glCreateShader(GL_FRAGMENT_SHADER);
   iStatus = 0;
   
   // load shader
   load(Filename, "fsh");
}
