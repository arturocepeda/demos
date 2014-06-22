
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Sample application
//
//  --- StateSample.cpp ---
//
//////////////////////////////////////////////////////////////////


#include "StateSample.h"
#include "Core/GEUtils.h"
#include "Core/GEDevice.h"

#include "banana.h"
#include "cube.h"

GEStateSample::GEStateSample(GERendering* Render, GEAudio* Audio, void* GlobalData)
   : GEState(Render, Audio, GlobalData)
   , fMeshCubeR(1.0f)
   , fMeshCubeRInc(-0.01f)
   , fMeshCubeG(0.5f)
   , fMeshCubeGInc(0.005f)
   , fMeshCubeB(0.2f)
   , fMeshCubeBInc(0.015f)
{
}

void GEStateSample::internalInit()
{ 
   cRender->setBackgroundColor(GEColor(0.1f, 0.1f, 0.3f));
   
   // lighting
   cRender->setAmbientLightColor(GEColor(1.0f, 1.0f, 1.0f));
   cRender->setAmbientLightIntensity(0.25f);
   
   cRender->setNumberOfActiveLights(1);
   cRender->setLightPosition((unsigned int)GELights::PointLight1, GEVector3(0.0f, 0.0f, 1.0f));
   cRender->setLightColor((unsigned int)GELights::PointLight1, GEColor(1.0f, 1.0f, 1.0f));
   cRender->setLightIntensity((unsigned int)GELights::PointLight1, 0.6f);
   
   // cameras
   cCamera = new GECamera();
   cCamera->setPosition(0.0f, 0.0f, -4.0f);

   // textures
   cRender->loadTexture(Textures.Background, "background", "jpg");
   cRender->loadTexture(Textures.Banana, "banana", "jpg");
   cRender->loadTexture(Textures.Info, "info", "png");
   cRender->loadTexture(Textures.Basketball, "basketball", "png");
   
   // meshes
   cRender->createMesh(&cMeshBanana);
   cMeshBanana->loadFromArrays(bananaNumVerts, bananaVerts, bananaNormals, bananaTexCoords);
   cMeshBanana->setTexture(cRender->getTexture(Textures.Banana));
   cMeshBanana->scale(2.5f, 2.5f, 2.5f);

   cRender->createMesh(&cMeshCube);
   cMeshCube->loadFromArrays(cubeNumVerts, cubeVerts, cubeNormals);
   cMeshCube->setPosition(0.0f, -1.5f, 0.0f);
   cMeshCube->scale(0.75f, 0.75f, 0.75f);
   cMeshCube->setColor(GEColor(1.0f, 0.5f, 0.2f));

   // sprites
   cRender->createSprite(&cSpriteBackground);
   cSpriteBackground->setTexture(cRender->getTexture(Textures.Background));
   cSpriteBackground->setSize(GEVector2(2.0f, 3.0f));
   
   cRender->createSprite(&cSpriteBall);
   cSpriteBall->setTexture(cRender->getTexture(Textures.Basketball));
   cSpriteBall->setSize(GEVector2(0.4f, 0.4f));
   
   for(int i = 0; i < FINGERS; i++)
   {
      cRender->createSprite(&cSpriteInfo[i]);
      cSpriteInfo[i]->setTexture(cRender->getTexture(Textures.Info));
      cSpriteInfo[i]->setSize(GEVector2(0.25f, 0.25f));
      cSpriteInfo[i]->setVisible(false);
   }
   
   // sounds
   //cAudio->loadSound(Sounds.Music, "song", "caf");
   cAudio->loadSound(Sounds.Touch, "touch", "wav");
   cAudio->setVolume(0, 0.2f);
   //cAudio->playSound(Sounds.Music, 1);
   
   // font
   //cRender->defineFont(0, "Optima-ExtraBlack", 44.0f);
   cRender->defineFont(0, "Test", 24.0f);
    
   // text
   cRender->createLabel(&cText, 0, GEAlignment::CenterCenter, GEVector2(0.16f, 0.16f), "Game Engine");
   cText->setPosition(0.0f, 1.15f);
   cText->setColor(GEColor(1.0f, 0.25f, 0.25f));
   cText->setOpacity(0.0f);
}

void GEStateSample::update(float DeltaTime)
{
   updateCube();
   updateBanana();
   updateBall();
   updateText();
}

void GEStateSample::updateText()
{
   if(cText->getOpacity() < 1.0f)   
      cText->setOpacity(cText->getOpacity() + 0.005f);
}

void GEStateSample::updateBanana()
{
   cMeshBanana->rotate(-0.01f, -0.01f, -0.01f);
}

void GEStateSample::updateCube()
{
   cMeshCube->rotate(0.01f, 0.01f, 0.01f);
   cMeshCube->setColor(GEColor(fMeshCubeR, fMeshCubeG, fMeshCubeB));
   
   fMeshCubeR += fMeshCubeRInc;
   fMeshCubeG += fMeshCubeGInc;
   fMeshCubeB += fMeshCubeBInc;
   
   if(fMeshCubeR >= 1.0f)
   {
      fMeshCubeR = 0.99f;
      fMeshCubeRInc = -fMeshCubeRInc;
   }
   else if(fMeshCubeR <= 0.0f)
   {
      fMeshCubeR = 0.01f;
      fMeshCubeRInc = -fMeshCubeRInc;
   }
   
   if(fMeshCubeG >= 1.0f)
   {
      fMeshCubeG = 0.99f;
      fMeshCubeGInc = -fMeshCubeGInc;
   }
   else if(fMeshCubeG <= 0.0f)
   {
      fMeshCubeG = 0.01f;
      fMeshCubeGInc = -fMeshCubeGInc;
   }
   
   if(fMeshCubeB >= 1.0f)
   {
      fMeshCubeB = 0.99f;
      fMeshCubeBInc = -fMeshCubeBInc;
   }
   else if(fMeshCubeB <= 0.0f)
   {
      fMeshCubeB = 0.01f;
      fMeshCubeBInc = -fMeshCubeBInc;
   }
}

void GEStateSample::updateBall()
{
   // get ball position
   vBallPosition = cSpriteBall->getPosition();
   
   // bounds control (left/right)
   if((vBallPosition.X < BOUNDS_LEFT) ||
      (vBallPosition.X > BOUNDS_RIGHT))
   {
      // correct position
      if(vBallPosition.X < 0.0f)
         vBallPosition.X = BOUNDS_LEFT;
      else
         vBallPosition.X = BOUNDS_RIGHT;
      
      cSpriteBall->setPosition(vBallPosition);
      
      // bounce
      vBallVelocity.X = (fabs(vBallVelocity.X) > STOPPED)? -vBallVelocity.X * BOUNCE: 0.0f;
   }
   
   // bounds control (top/bottom)
   if((vBallPosition.Y > BOUNDS_TOP) ||
      (vBallPosition.Y < BOUNDS_BOTTOM))
   {
      // correct position
      if(vBallPosition.Y < 0.0f)
         vBallPosition.Y = BOUNDS_BOTTOM;
      else
         vBallPosition.Y = BOUNDS_TOP;
      
      cSpriteBall->setPosition(vBallPosition);
      
      // bounce
      vBallVelocity.Y = (fabs(vBallVelocity.Y) > STOPPED)? -vBallVelocity.Y * BOUNCE: 0.0f;
   }
   
   // move and rotate the ball
   cSpriteBall->move(vBallVelocity);
   cSpriteBall->rotate(0.0f, 0.0f, ((vBallPosition.Y < 0.0f)? -1: 1) * vBallVelocity.X * ROTATION);
   cSpriteBall->rotate(0.0f, 0.0f, ((vBallPosition.X < 0.0f)? 1: -1) * vBallVelocity.Y * ROTATION);
}

void GEStateSample::render()
{
   // background
   cRender->set2D();
   cRender->useShaderProgram((unsigned int)GEShaderPrograms::HUD);
   cRender->renderSprite(cSpriteBackground);

   // camera
   cRender->set3D();
   cRender->useCamera(cCamera);

   // meshes
   cRender->useShaderProgram((unsigned int)GEShaderPrograms::MeshColor);
   cRender->renderMesh(cMeshCube);
   cRender->useShaderProgram((unsigned int)GEShaderPrograms::MeshTexture);
   cRender->renderMesh(cMeshBanana);

   // sprites
   cRender->set2D();
   cRender->useShaderProgram((unsigned int)GEShaderPrograms::HUD);
   cRender->renderSprite(cSpriteBall);
    
   for(int i = 0; i < FINGERS; i++)
      cRender->renderSprite(cSpriteInfo[i]);

   // text
   cRender->set2D();
   cRender->useShaderProgram((unsigned int)GEShaderPrograms::Text);
   cRender->renderLabel(cText);
}

void GEStateSample::release()
{
   // stop audio sources and release sounds
   cAudio->stop(Sounds.Music);
   cAudio->stop(Sounds.Touch);   
   cAudio->unloadAllSounds();
   
   // release objects
   delete cCamera;
   delete cMeshBanana;
   delete cMeshCube;

   delete cSpriteBackground;
   delete cSpriteBall;
   
   for(int i = 0; i < FINGERS; i++)
      delete cSpriteInfo[i];

   delete cText;
}

void GEStateSample::inputTouchBegin(int ID, const GEVector2& Point)
{
   cAudio->playSound(Sounds.Touch, 0);

   GEVector2 vScreenPosition = cRender->pixelToScreen(Point);
   cSpriteInfo[ID]->setPosition(vScreenPosition.X, vScreenPosition.Y, 0.0f);
   cSpriteInfo[ID]->show();
}

void GEStateSample::inputTouchMove(int ID, const GEVector2& PreviousPoint, const GEVector2& CurrentPoint)
{
   if(ID == 0)
   {
      cCamera->move((CurrentPoint.X - PreviousPoint.X) * TOUCH_SCALE,
                    (-CurrentPoint.Y + PreviousPoint.Y) * TOUCH_SCALE,
                    0.0f);
   }
   
   GEVector2 vScreenPosition = cRender->pixelToScreen(CurrentPoint);
   cSpriteInfo[ID]->setPosition(vScreenPosition.X, vScreenPosition.Y);
}

void GEStateSample::inputTouchEnd(int ID, const GEVector2& Point)
{
   cSpriteInfo[ID]->hide();
}

void GEStateSample::updateAccelerometerStatus(const GEVector3& Status)
{
   vBallVelocity.X += Status.X * ACC_SCALE;
   vBallVelocity.Y += Status.Y * ACC_SCALE;
}
