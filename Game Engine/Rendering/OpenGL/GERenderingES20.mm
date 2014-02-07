
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda PŽrez
//  iOS Game Engine
//
//  Rendering Engine (OpenGL ES)
//
//  --- GERendering.mm ---
//
//////////////////////////////////////////////////////////////////

#include "GERenderingES20.h"
#include "GEDevice.h"
#include "config.h"

GERenderingES20::GERenderingES20(EAGLContext* Context)
    : GERendering(Context, false, GEDevice::getTouchPadWidth(), GEDevice::getTouchPadHeight())
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
   
   CGImageRef cgImage = [UIImage imageNamed: [NSString stringWithUTF8String: Name]].CGImage;
   CGContextRef cgContext;
   GLubyte* pData = nil;
   size_t iWidth, iHeight;
	
   if(!cgImage)
      return;
   
   glBindTexture(GL_TEXTURE_2D, tTextures[TextureIndex]);
   
   iWidth = CGImageGetWidth(cgImage);
   iHeight = CGImageGetHeight(cgImage);
   
   tTextureSize[TextureIndex].Width = iWidth;
   tTextureSize[TextureIndex].Height = iHeight;
	
   pData = (GLubyte*)calloc(iWidth * iHeight * 4, sizeof(GLubyte));
   
   // Uses the bitmatp creation function provided by the Core Graphics framework.
   cgContext = CGBitmapContextCreate(pData, iWidth, iHeight, 8, iWidth * 4,
                                     CGImageGetColorSpace(cgImage), kCGImageAlphaPremultipliedLast);
   
   // After you create the context, you can draw the image to the context.
   CGContextDrawImage(cgContext, CGRectMake(0.0f, 0.0f, (CGFloat)iWidth, (CGFloat)iHeight), cgImage);
   
   // You don't need the context at this point, so you need to release it to avoid memory leaks.
   CGContextRelease(cgContext);
   
   // setup texture parameters
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData);
   free(pData);
}

void GERenderingES20::loadTextureCompressed(unsigned int TextureIndex, const char* Name,
                                            unsigned int Size, unsigned int BPP, bool Alpha)
{
   if(TextureIndex >= TEXTURES)
      return;
   
   // read file data
   NSString* sPath = [[NSBundle mainBundle] pathForResource: [NSString stringWithUTF8String: Name] ofType: @""];
   NSData* pData = [[NSData alloc] initWithContentsOfFile: sPath];
   
   if(!pData)
      return;
   
   // bind texture in OpenGL
   glBindTexture(GL_TEXTURE_2D, tTextures[TextureIndex]);
   
   // set parameters
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   
   // determine compressed data format
   GLenum glFormat;
   
   if(BPP == 4)
      glFormat = (Alpha)? GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG: GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
   else
      glFormat = (Alpha)? GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG: GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
   
   // load texture
   glCompressedTexImage2D(GL_TEXTURE_2D, 0, glFormat, Size, Size, 0, [pData length], [pData bytes]);
   
   // release texture data
   [pData release];
}

void GERenderingES20::useCamera(GECamera* Camera)
{
   vCameraPosition = Camera->getPosition();
   matView = GLKMatrix4MakeTranslation(vCameraPosition.X, vCameraPosition.Y, vCameraPosition.Z);
    
   vCameraRotation = Camera->getRotation();
   matView = GLKMatrix4Rotate(matView, vCameraRotation.X, 1.0f, 0.0f, 0.0f);
   matView = GLKMatrix4Rotate(matView, vCameraRotation.Y, 0.0f, 1.0f, 0.0f);
   matView = GLKMatrix4Rotate(matView, vCameraRotation.Z, 0.0f, 0.0f, 1.0f);
}

void GERenderingES20::loadShaders()
{
#ifdef USE_SHADER_HUD
   GEVertexShader cVertexShaderHUD(@"hud");
   GEFragmentShader cFragmentShaderHUD(@"hud");
   attachShaders(GEShaderPrograms::HUD, cVertexShaderHUD, cFragmentShaderHUD);
#endif
   
#ifdef USE_SHADER_TEXT
   GEVertexShader cVertexShaderText(@"text");
   GEFragmentShader cFragmentShaderText(@"text");
   attachShaders(GEShaderPrograms::Text, cVertexShaderText, cFragmentShaderText);
#endif
   
#ifdef USE_SHADER_MESH_COLOR
   GEVertexShader cVertexShaderMeshColor(@"mesh_color");
   GEFragmentShader cFragmentShaderMeshColor(@"mesh_color");
   attachShaders(GEShaderPrograms::MeshColor, cVertexShaderMeshColor, cFragmentShaderMeshColor);
#endif
   
#ifdef USE_SHADER_MESH_TEXTURE
   GEVertexShader cVertexShaderMeshTexture(@"mesh_texture");
   GEFragmentShader cFragmentShaderMeshTexture(@"mesh_texture");
   attachShaders(GEShaderPrograms::MeshTexture, cVertexShaderMeshTexture, cFragmentShaderMeshTexture);
#endif
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
      NSLog(@"Program %d: linking error\n%s", ProgramIndex, sLog);
      
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
   fFonts[Font] = [UIFont fontWithName:[NSString stringWithUTF8String:FontName] size:Size];
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
   GEMatrix4 mMatrix;
   Mesh->getModelMatrix(&mMatrix);
   memcpy(&matModel, &mMatrix, sizeof(GLKMatrix4));
   
   // calculate model-view matrix, normal matrix and transform matrix
   matModelView = GLKMatrix4Multiply(matView, matModel);   
   matNormal = GLKMatrix3InvertAndTranspose(GLKMatrix4GetMatrix3(matModelView), NULL);
   matModelViewProjection = GLKMatrix4Multiply(matProjection, matModelView);      

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
   GEMatrix4 mMatrix;
   Sprite->getModelMatrix(&mMatrix);
   memcpy(&matModel, &mMatrix, sizeof(GLKMatrix4));
   
   // calculate transform matrix
   matModelView = GLKMatrix4Multiply(matView, matModel);   
   matModelViewProjection = GLKMatrix4Multiply(matProjection, matModelView);      

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
   GEMatrix4 mMatrix;
   Label->getModelMatrix(&mMatrix);
   memcpy(&matModel, &mMatrix, sizeof(GLKMatrix4));
   
   // calculate transform matrix
   matModelView = GLKMatrix4Multiply(matView, matModel);   
   matModelViewProjection = GLKMatrix4Multiply(matProjection, matModelView);      

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
   [glContext presentRenderbuffer:GL_RENDERBUFFER];
}

void GERenderingES20::set2D(bool Portrait)
{
   glDisable(GL_DEPTH_TEST);
   
   float fAspect = (float)GEDevice::getScreenHeight() / GEDevice::getScreenWidth();

   matProjection = (Portrait)? GLKMatrix4MakeOrtho(-1.0f, 1.0f, -fAspect, fAspect, 0.1f, 100.0f):
                               GLKMatrix4MakeOrtho(-fAspect, fAspect, -1.0f, 1.0f, 0.1f, 100.0f);   
   
   matView = GLKMatrix4MakeLookAt(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
}

void GERenderingES20::set3D(bool Portrait)
{
	glEnable(GL_DEPTH_TEST);

   float fAspect = (Portrait)? (float)GEDevice::getScreenWidth() / GEDevice::getScreenHeight():
                               (float)GEDevice::getScreenHeight() / GEDevice::getScreenWidth();
   
   matProjection = GLKMatrix4MakePerspective(GLKMathDegreesToRadians(65.0f), fAspect, 0.1f, 100.0f);
}
