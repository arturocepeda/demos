
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda PŽrez
//  Game Engine
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
   unsigned int ID;
   int Status;
};


//
//  GEShader
//
class GEShader
{
protected:
   unsigned int iID;
   int iStatus;
   
   void load(const char* sFilename, const char* sExt);
   
public:
   ~GEShader();
   
   bool check();
   unsigned int getID();
};


class GEVertexShader : public GEShader
{
public:
   GEVertexShader(const char* Filename);   
};


class GEFragmentShader : public GEShader
{
public:
   GEFragmentShader(const char* Filename);
};
