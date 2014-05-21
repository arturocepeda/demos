
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Rendering Engine (OpenGL ES)
//
//  --- GERendering.cpp ---
//
//////////////////////////////////////////////////////////////////

#include "GERenderingES20.h"
#include "GEDevice.h"
#include "stblib/stb_image.h"
#include <android/log.h>

GERenderingES20::GERenderingES20(CTContext* Context)
   : GERendering(Context, false, GEDevice::getScreenWidth(), GEDevice::getScreenHeight())
{
   glContext = Context;   
   cBackgroundColor.set(1.0f, 1.0f, 1.0f);
    
   float fAspectRatio = (float)iScreenHeight / iScreenWidth;
   cPixelToScreenX = new GELine(0.0f, -1.0f, iScreenWidth, 1.0f);
   cPixelToScreenY = new GELine(0.0f, fAspectRatio, iScreenHeight, -fAspectRatio);
   
   // lighting
   iNumberOfActiveLights = 0;   
   setAmbientLightColor(GEColor(1.0f, 1.0f, 1.0f));
   setAmbientLightIntensity(0.2f);
   
   // create programs
   for(int i = 0; i < GEShaderPrograms::Count; i++)
   {
      sPrograms[i].ID = glCreateProgram();
      sPrograms[i].Status = 0;
   }
   
	// enable alpha blending
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_BLEND);
   
   // generate and attach buffers
   glGenBuffers(1, &iVertexBuffer);
   glGenBuffers(1, &iIndexBuffer);
   
   glEnableVertexAttribArray(GEVertexAttributes.Position);
   glEnableVertexAttribArray(GEVertexAttributes.Normal);
   glEnableVertexAttribArray(GEVertexAttributes.TextureCoord0);
   
   // generate textures
   glGenTextures(TEXTURES, tTextures);
   memset(tTextureSize, 0, sizeof(GETextureSize) * TEXTURES);
   
   // load shaders
   loadShaders();
   iActiveProgram = 0;
}

GERenderingES20::~GERenderingES20()
{
   // release programs
   for(unsigned int i = 0; i < GEShaderPrograms::Count; i++)
      glDeleteProgram(sPrograms[i].ID);
   
   // release buffers
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   
   glDeleteBuffers(1, &iVertexBuffer);
   glDeleteBuffers(1, &iIndexBuffer);
   
   // release textures
   glDeleteTextures(TEXTURES, tTextures);
}

void GERenderingES20::setupProjectionMatrix()
{
}

void GERenderingES20::clearBuffers()
{    
}

void GERenderingES20::loadTexture(unsigned int TextureIndex, const char* Name)
{
   if(TextureIndex >= TEXTURES)
      return;
   
   char sFileName[256];
   unsigned char* pFileData;
   unsigned int iFileLenght;

   sprintf(sFileName, "Textures/%s", Name);
   iFileLenght = GEDevice::getFileLength(sFileName);
   pFileData = new unsigned char[iFileLenght];
   GEDevice::readFile(sFileName, pFileData, iFileLenght);

	int iWidth = 0;
	int iHeight = 0;
	int iBytesPerPixel = 0;
   unsigned char* pImageData = nullptr;

	pImageData = stbi_load_from_memory(pFileData, iFileLenght, &iWidth, &iHeight, &iBytesPerPixel, 0);

	if(!pImageData)
   {
      delete[] pFileData;
		return;
   }
        
   glBindTexture(GL_TEXTURE_2D, tTextures[TextureIndex]);
	
   // setup texture parameters
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   
   GLenum glFormat = iBytesPerPixel == 4 ? GL_RGBA : GL_RGB;
   glTexImage2D(GL_TEXTURE_2D, 0, glFormat, iWidth, iHeight, 0, glFormat, GL_UNSIGNED_BYTE, pImageData);

   stbi_image_free(pImageData);
   delete[] pFileData;
}

void GERenderingES20::loadTextureCompressed(unsigned int TextureIndex, const char* Name,
                                            unsigned int Size, unsigned int BPP, bool Alpha)
{
   if(TextureIndex >= TEXTURES)
      return;
   
   //TODO
}

void GERenderingES20::useCamera(GECamera* Camera)
{
   vCameraPosition = Camera->getPosition();
   GEMatrix4MakeTranslation(vCameraPosition, &matView);
    
   vCameraRotation = Camera->getRotation();
   GEMatrix4RotateX(&matView, vCameraRotation.X);
   GEMatrix4RotateY(&matView, vCameraRotation.Y);
   GEMatrix4RotateZ(&matView, vCameraRotation.Z);
}

void GERenderingES20::loadShaders()
{
   GEVertexShader cVertexShaderHUD("hud");
   GEFragmentShader cFragmentShaderHUD("hud");
   attachShaders(GEShaderPrograms::HUD, cVertexShaderHUD, cFragmentShaderHUD);

   GEVertexShader cVertexShaderText("text");
   GEFragmentShader cFragmentShaderText("text");
   attachShaders(GEShaderPrograms::Text, cVertexShaderText, cFragmentShaderText);

   GEVertexShader cVertexShaderMeshColor("mesh_color");
   GEFragmentShader cFragmentShaderMeshColor("mesh_color");
   attachShaders(GEShaderPrograms::MeshColor, cVertexShaderMeshColor, cFragmentShaderMeshColor);

   GEVertexShader cVertexShaderMeshTexture("mesh_texture");
   GEFragmentShader cFragmentShaderMeshTexture("mesh_texture");
   attachShaders(GEShaderPrograms::MeshTexture, cVertexShaderMeshTexture, cFragmentShaderMeshTexture);
}

void GERenderingES20::attachShaders(unsigned int ProgramIndex, GEVertexShader& VertexShader, GEFragmentShader& FragmentShader)
{
   // attach shaders to the program
   glAttachShader(sPrograms[ProgramIndex].ID, VertexShader.getID());
   glAttachShader(sPrograms[ProgramIndex].ID, FragmentShader.getID());
   
   // bind attributes
   glBindAttribLocation(sPrograms[ProgramIndex].ID, GEVertexAttributes.Position, "aPosition");
   glBindAttribLocation(sPrograms[ProgramIndex].ID, GEVertexAttributes.Normal, "aNormal");
   glBindAttribLocation(sPrograms[ProgramIndex].ID, GEVertexAttributes.TextureCoord0, "aTextCoord0");
   
   // link program
   linkProgram(ProgramIndex);
   
   if(!checkProgram(ProgramIndex))
   {
      // get info log length
      GLint iLogLength;
      glGetProgramiv(sPrograms[ProgramIndex].ID, GL_INFO_LOG_LENGTH, &iLogLength);
      
      // get info log contents
      GLchar* sLog = new GLchar[iLogLength];
      glGetProgramInfoLog(sPrograms[ProgramIndex].ID, iLogLength, NULL, sLog);
      
      // show info log
      __android_log_print(ANDROID_LOG_VERBOSE, "GameEngine", "Program %d: linking error\n%s", ProgramIndex, sLog);
      
      delete[] sLog;
      exit(1);
   }
   
   // get uniforms location
   getUniformsLocation(ProgramIndex);
}

void GERenderingES20::linkProgram(unsigned int iProgramIndex)
{
   glLinkProgram(sPrograms[iProgramIndex].ID);
   glGetProgramiv(sPrograms[iProgramIndex].ID, GL_LINK_STATUS, &(sPrograms[iProgramIndex].Status));
}

bool GERenderingES20::checkProgram(unsigned int iProgramIndex)
{
   return (sPrograms[iProgramIndex].Status != 0);
}

void GERenderingES20::getUniformsLocation(unsigned int iProgramIndex)
{
   // matrices
   iUniforms[iProgramIndex][GEUniforms::ModelViewProjection] = glGetUniformLocation(sPrograms[iProgramIndex].ID, "uModelViewProjectionMatrix");
   iUniforms[iProgramIndex][GEUniforms::ModelView] = glGetUniformLocation(sPrograms[iProgramIndex].ID, "uModelViewMatrix");
   iUniforms[iProgramIndex][GEUniforms::Normal] = glGetUniformLocation(sPrograms[iProgramIndex].ID, "uNormalMatrix");
   
   // object color properties
   iUniforms[iProgramIndex][GEUniforms::ObjectColor] = glGetUniformLocation(sPrograms[iProgramIndex].ID, "uObjectColor");
   iUniforms[iProgramIndex][GEUniforms::Texture0] = glGetUniformLocation(sPrograms[iProgramIndex].ID, "uTexture0");
   
   // lighting
   iUniforms[iProgramIndex][GEUniforms::AmbientLightColor] = glGetUniformLocation(sPrograms[iProgramIndex].ID, "uAmbientLightColor");
   iUniforms[iProgramIndex][GEUniforms::AmbientLightIntensity] = glGetUniformLocation(sPrograms[iProgramIndex].ID, "uAmbientLightIntensity");
   iUniforms[iProgramIndex][GEUniforms::PointLight1Position] = glGetUniformLocation(sPrograms[iProgramIndex].ID, "uPointLight1Position");
   iUniforms[iProgramIndex][GEUniforms::PointLight1Color] = glGetUniformLocation(sPrograms[iProgramIndex].ID, "uPointLight1Color");
   iUniforms[iProgramIndex][GEUniforms::PointLight1Intensity] = glGetUniformLocation(sPrograms[iProgramIndex].ID, "uPointLight1Intensity");
}

void GERenderingES20::useShaderProgram(unsigned int iProgramIndex)
{
   glUseProgram(sPrograms[iProgramIndex].ID);
   iActiveProgram = iProgramIndex;
}

void GERenderingES20::createMesh(GEMesh** Mesh)
{
   *Mesh = new GEMeshES20();
}

void GERenderingES20::createSprite(GESprite** Sprite)
{
   *Sprite = new GESpriteES20();
}

void GERenderingES20::createLabel(GELabel** Label, unsigned int Font, GEAlignment Alignment,
                                  unsigned int Width, unsigned int Height, const char* Text)
{
   *Label = new GELabelES20(fFonts[Font], Text, Alignment, Width, Height);
}

void GERenderingES20::createCamera(GECamera** Camera)
{
   *Camera = new GECamera();
}

void GERenderingES20::defineFont(unsigned int Font, const char* FontName, float Size, bool Bold, bool Italic)
{
   //TODO
}

void GERenderingES20::releaseFont(unsigned int Font)
{
}

void GERenderingES20::renderBegin()
{
   glClearColor(cBackgroundColor.R, cBackgroundColor.G, cBackgroundColor.B, 1.0f);
   glClearDepthf(1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   glBindBuffer(GL_ARRAY_BUFFER, iVertexBuffer);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iIndexBuffer);
}

void GERenderingES20::renderMesh(GEMesh* Mesh)
{
   // get model matrix from the mesh
   Mesh->getModelMatrix(&matModel);
   
   // calculate model-view matrix and transform matrix
   GEMatrix4Multiply(matView, matModel, &matModelView);
   GEMatrix4Multiply(matProjection, matModelView, &matModelViewProjection);
   
   // calculate normal matrix
   GEMatrix4GetMatrix3(matModelView, &matNormal);
   GEMatrix3Invert(&matNormal);
   GEMatrix3Transpose(&matNormal);

   // set uniform values for the shaders
   cColor = Mesh->getColor();
   
   glUniformMatrix4fv(iUniforms[iActiveProgram][GEUniforms::ModelViewProjection], 1, 0, matModelViewProjection.m);
   glUniformMatrix4fv(iUniforms[iActiveProgram][GEUniforms::ModelView], 1, 0, matModelView.m);
   glUniformMatrix3fv(iUniforms[iActiveProgram][GEUniforms::Normal], 1, 0, matNormal.m);
   glUniform4f(iUniforms[iActiveProgram][GEUniforms::ObjectColor], cColor.R, cColor.G, cColor.B, Mesh->getOpacity());
   
   glUniform3f(iUniforms[iActiveProgram][GEUniforms::AmbientLightColor], sAmbientLight.Color.R, sAmbientLight.Color.G, sAmbientLight.Color.B);
   glUniform1f(iUniforms[iActiveProgram][GEUniforms::AmbientLightIntensity], sAmbientLight.Intensity);

   glUniform3f(iUniforms[iActiveProgram][GEUniforms::PointLight1Position], sLights[0].Position.X, sLights[0].Position.Y, sLights[0].Position.Z);
   glUniform3f(iUniforms[iActiveProgram][GEUniforms::PointLight1Color], sLights[0].Color.R, sLights[0].Color.G, sLights[0].Color.B);
   glUniform1f(iUniforms[iActiveProgram][GEUniforms::PointLight1Intensity], sLights[0].Intensity);

   glBindTexture(GL_TEXTURE_2D, Mesh->getTexture());
   glUniform1i(iUniforms[iActiveProgram][GEUniforms::Texture0], 0);
   
   // draw
   Mesh->render();
}

void GERenderingES20::renderSprite(GESprite* Sprite)
{
   // get model matrix from the sprite
   Sprite->getModelMatrix(&matModel);
   
   // calculate transform matrix
   GEMatrix4Multiply(matView, matModel, &matModelView);   
   GEMatrix4Multiply(matProjection, matModelView, &matModelViewProjection);      

   // set uniform values for the shaders
   glUniformMatrix4fv(iUniforms[iActiveProgram][GEUniforms::ModelViewProjection], 1, 0, matModelViewProjection.m);
   glUniform4f(iUniforms[iActiveProgram][GEUniforms::ObjectColor], 1.0f, 1.0f, 1.0f, Sprite->getOpacity());
   
   glBindTexture(GL_TEXTURE_2D, Sprite->getTexture());
   glUniform1i(iUniforms[iActiveProgram][GEUniforms::Texture0], 0);
   
   // draw
   Sprite->render();
}

void GERenderingES20::renderLabel(GELabel* Label)
{
   // get model matrix from the label
   Label->getModelMatrix(&matModel);
   
   // calculate transform matrix
   GEMatrix4Multiply(matView, matModel, &matModelView);
   GEMatrix4Multiply(matProjection, matModelView, &matModelViewProjection);

   // set uniform values for the shaders
   cColor = Label->getColor();
   
   glUniformMatrix4fv(iUniforms[iActiveProgram][GEUniforms::ModelViewProjection], 1, 0, matModelViewProjection.m);
   glUniform4f(iUniforms[iActiveProgram][GEUniforms::ObjectColor], cColor.R, cColor.G, cColor.B, Label->getOpacity());
   
   glBindTexture(GL_TEXTURE_2D, Label->getTexture());
   glUniform1i(iUniforms[iActiveProgram][GEUniforms::Texture0], 0);
   
   // draw
   Label->render(); 
}

void GERenderingES20::renderEnd()
{
}

void GERenderingES20::set2D(bool Portrait)
{
   glDisable(GL_DEPTH_TEST);
   
   float fAspect = (float)GEDevice::getScreenHeight() / GEDevice::getScreenWidth();

   if(Portrait)
      GEMatrix4MakeOrtho(-1.0f, 1.0f, -fAspect, fAspect, 0.1f, 100.0f, &matProjection);
   else
      GEMatrix4MakeOrtho(-fAspect, fAspect, -1.0f, 1.0f, 0.1f, 100.0f, &matProjection);

   GEMatrix4MakeLookAt(GEVector3(0.0f, 0.0f, 1.0f), GEVector3(0.0f, 0.0f, 0.0f), GEVector3(0.0f, 1.0f, 0.0f), &matView);
}

void GERenderingES20::set3D(bool Portrait)
{
	glEnable(GL_DEPTH_TEST);

   float fAspect = (Portrait)? (float)GEDevice::getScreenWidth() / GEDevice::getScreenHeight():
                               (float)GEDevice::getScreenHeight() / GEDevice::getScreenWidth();

   GEMatrix4MakePerspective(65.0f / DEG2RAD, fAspect, 0.1f, 100.0f, &matProjection);
}
