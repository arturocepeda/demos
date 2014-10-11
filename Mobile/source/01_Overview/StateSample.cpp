
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

using namespace GE;
using namespace GE::States;
using namespace GE::Rendering;
using namespace GE::Audio;

GEStateSample::GEStateSample(RenderSystem* Render, AudioSystem* Audio, void* GlobalData)
   : State(Render, Audio, GlobalData)
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
   cRender->setBackgroundColor(Color(0.1f, 0.1f, 0.3f));
   
   // lighting
   cRender->setAmbientLightColor(Color(1.0f, 1.0f, 1.0f));
   cRender->setAmbientLightIntensity(0.25f);
   
   cRender->setNumberOfActiveLights(1);
   cRender->setLightPosition((unsigned int)Lights::PointLight1, Vector3(0.0f, 0.0f, 1.0f));
   cRender->setLightColor((unsigned int)Lights::PointLight1, Color(1.0f, 1.0f, 1.0f));
   cRender->setLightIntensity((unsigned int)Lights::PointLight1, 0.6f);
   
   // cameras
   cCamera = new Camera();
   cCamera->setPosition(0.0f, 0.0f, -4.0f);

   // textures
   cRender->loadTexture(Textures.Background, "background", "jpg");
   cRender->loadTexture(Textures.Banana, "banana", "jpg");
   cRender->loadTexture(Textures.Info, "info", "png");
   cRender->loadTexture(Textures.Basketball, "basketball", "png");
   
   // meshes
   cRender->createMesh(&cMeshBanana);
   cMeshBanana->loadFromFile("banana");
   cMeshBanana->setTexture(cRender->getTexture(Textures.Banana));
   cMeshBanana->scale(2.0f, 2.0f, 2.0f);

   cRender->createMesh(&cMeshCube);
   cMeshCube->loadFromFile("cube");
   cMeshCube->setPosition(0.0f, -1.5f, 0.0f);
   cMeshCube->scale(0.5f, 0.5f, 0.5f);
   cMeshCube->setColor(Color(1.0f, 0.5f, 0.2f));

   // sprites
   cRender->createSprite(&cSpriteBackground);
   cSpriteBackground->setTexture(cRender->getTexture(Textures.Background));
   cSpriteBackground->setSize(Vector2(2.0f, 4.0f));
   
   cRender->createSprite(&cSpriteBall);
   cSpriteBall->setTexture(cRender->getTexture(Textures.Basketball));
   cSpriteBall->setSize(Vector2(0.4f, 0.4f));
   
   for(int i = 0; i < FINGERS; i++)
   {
      cRender->createSprite(&cSpriteInfo[i]);
      cSpriteInfo[i]->setTexture(cRender->getTexture(Textures.Info));
      cSpriteInfo[i]->setSize(Vector2(0.25f, 0.25f));
      cSpriteInfo[i]->setVisible(false);
   }
   
   // sounds
   //cAudio->loadSound(Sounds.Music, "song", "caf");
   cAudio->loadSound(Sounds.Touch, "touch", "wav");
   cAudio->setVolume(0, 0.2f);
   cAudio->playSound(Sounds.Music, 1);
   
   // font
   //cRender->defineFont(0, "Optima-ExtraBlack", 44.0f);
   cRender->defineFont(0, "Test", 24.0f);
    
   // text
   cRender->createLabel(&cText, 0, Alignment::CenterCenter, Vector2(0.16f, 0.16f), "Game Engine");
   cText->setPosition(0.0f, 1.15f);
   cText->setColor(Color(1.0f, 0.25f, 0.25f));
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
   cMeshCube->setColor(Color(fMeshCubeR, fMeshCubeG, fMeshCubeB));
   
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
   cRender->useShaderProgram((unsigned int)ShaderPrograms::HUD);
   cRender->renderSprite(cSpriteBackground);

   // camera
   cRender->set3D();
   cRender->useCamera(cCamera);

   // meshes
   cRender->useShaderProgram((unsigned int)ShaderPrograms::MeshColor);
   cRender->renderMesh(cMeshCube);
   cRender->useShaderProgram((unsigned int)ShaderPrograms::MeshTexture);
   cRender->renderMesh(cMeshBanana);

   // sprites
   cRender->set2D();
   cRender->useShaderProgram((unsigned int)ShaderPrograms::HUD);
   cRender->renderSprite(cSpriteBall);
    
   for(int i = 0; i < FINGERS; i++)
      cRender->renderSprite(cSpriteInfo[i]);

   // text
   cRender->set2D();
   cRender->useShaderProgram((unsigned int)ShaderPrograms::Text);
   cRender->renderLabel(cText);
}

void GEStateSample::release()
{
   // stop audio sources and release sounds
   //cAudio->stop(Sounds.Music);
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

void GEStateSample::inputTouchBegin(int ID, const Vector2& Point)
{
   cAudio->playSound(Sounds.Touch, 0);

   cSpriteInfo[ID]->setPosition(Point.X, Point.Y, 0.0f);
   cSpriteInfo[ID]->show();
}

void GEStateSample::inputTouchMove(int ID, const Vector2& PreviousPoint, const Vector2& CurrentPoint)
{
   if(ID == 0)
   {
      cCamera->move((CurrentPoint.X - PreviousPoint.X) * TOUCH_SCALE,
                    (CurrentPoint.Y - PreviousPoint.Y) * TOUCH_SCALE,
                    0.0f);
   }
   
   cSpriteInfo[ID]->setPosition(CurrentPoint.X, CurrentPoint.Y);
}

void GEStateSample::inputTouchEnd(int ID, const Vector2& Point)
{
   cSpriteInfo[ID]->hide();
}

void GEStateSample::updateAccelerometerStatus(const Vector3& Status)
{
   vBallVelocity.X += Status.X * ACC_SCALE;
   vBallVelocity.Y += Status.Y * ACC_SCALE;
}
