
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda PŽrez
//  iOS Game Engine
//
//  Rendering Engine (OpenGL ES)
//
//  --- GERenderingShadersES20.h ---
//
//////////////////////////////////////////////////////////////////

#pragma once

//
//  GEShaderProgram
//
struct GEShaderProgram
{
   GLuint ID;
   GLint Status;
};


//
//  GEShader
//
class GEShader
{
protected:
   GLuint iID;
   GLint iStatus;
   
   void load(NSString* sFilename, NSString* sExt);
   
public:
   ~GEShader();
   
   bool check();
   GLuint getID();
};


class GEVertexShader : public GEShader
{
public:
   GEVertexShader(NSString* Filename);   
};


class GEFragmentShader : public GEShader
{
public:
   GEFragmentShader(NSString* Filename);
};
