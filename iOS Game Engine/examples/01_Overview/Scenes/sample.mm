
/*
   Arturo Cepeda Pérez
 
   Sample Scene
 
   --- sample.mm ---
 */

#include "sample.h"
#include "banana.h"
#include "cube.h"
#include "GEUtils.h"

GESceneSample::GESceneSample(GERendering* Render, GEAudio* Audio, void* GlobalData) :
               GEScene(Render, Audio, GlobalData)
{
}

void GESceneSample::init()
{   
   iNextScene = -1;
   
   // device info
   NSLog(@"\nDevice type: %s", (GEDevice::iPhone())? "iPhone": "iPad");
   
   if(!GEDevice::iPhone())
      NSLog(@"\nRetina display: %s", (GEDevice::displayRetina())? "yes": "no");
   
   // cameras
   cCamera = new GECamera();
   cCamera->setPosition(0.0f, 0.0f, -3.0f);
   cCamera->use();
   
   // textures
   cRender->loadTexture(Textures.Background, @"background.jpg");
   cRender->loadTexture(Textures.Banana, @"banana.jpg");
   cRender->loadTexture(Textures.Info, @"info.png");
   cRender->loadTexture(Textures.Basketball, @"basketball.png");
   
   // meshes
   cMeshBanana = new GEMesh();
   cMeshBanana->loadFromHeader(bananaNumVerts, bananaVerts, bananaNormals, 
                               cRender->getTexture(Textures.Banana), bananaTexCoords);
   cMeshBanana->scale(1.5f, 1.5f, 1.5f);
   
   cMeshCube = new GEMesh();
   cMeshCube->loadFromHeader(cubeNumVerts, cubeVerts, cubeNormals);
   cMeshCube->setPosition(0.0f, -1.0f, 0.0f);
   cMeshCube->scale(0.5f, 0.5f, 0.5f);
   cMeshCube->setColor(0.75f, 0.25f, 0.25f);
   cMeshCube->setOpacity(0.75f);
   
   // sprites
   cSpriteBackground = new GESprite(cRender->getTexture(Textures.Background),
                                    cRender->getTextureSize(Textures.Background));
   cSpriteBackground->scale(1.0f, 1.5f, 1.0f);
   
   cSpriteBall = new GESprite(cRender->getTexture(Textures.Basketball), 
                              cRender->getTextureSize(Textures.Basketball));
   cSpriteBall->scale(0.2f, 0.2f, 0.2f);
   
   for(int i = 0; i < FINGERS; i++)
   {
      cSpriteInfo[i] = new GESprite(cRender->getTexture(Textures.Info), 
                                    cRender->getTextureSize(Textures.Info));
      cSpriteInfo[i]->scale(0.15f, 0.15f, 0.15f);
      cSpriteInfo[i]->rotate(0.0f, 0.0f, 90.0f);
      cSpriteInfo[i]->setVisible(false);
   }
   
   // sounds
   cAudio->loadSound(Sounds.Music, @"song.caf");
   cAudio->loadSound(Sounds.Touch, @"touch.wav");   
   cAudio->setSourceVolume(1, 0.25f);
   cAudio->playSound(Sounds.Music, 0);
   
   // text
   cText = new GELabel(@"ARTURO CEPEDA\niOS Game Engine", @"Optima-ExtraBlack", 44.0f,
                       UITextAlignmentCenter, 512);
   cText->setPosition(-0.5f, -0.5f, 0.0f);
   cText->setScale(2.4f, 2.4f, 2.4f);
   cText->setOpacity(0.0f);
}

void GESceneSample::update()
{
   updateText();
   updateBanana();
   updateCube();
   updateBall();
   
   render();
}

void GESceneSample::updateText()
{
   cText->setOpacity(cText->getOpacity() + 0.001f);
}

void GESceneSample::updateBanana()
{
   cMeshBanana->rotate(0.1f, 0.1f, 0.1f);
}

void GESceneSample::updateCube()
{
   cMeshCube->rotate(0.4f, 0.6f, 0.8f);
}

void GESceneSample::updateBall()
{
   // get ball position
   cSpriteBall->getPosition(&vBallPosition);
   
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

void GESceneSample::render()
{
   cRender->renderBegin();
   
   // background
   cRender->set2D();
   cRender->renderSprite(cSpriteBackground);
   
   // camera
   cRender->set3D();
   cCamera->use();
   
   // meshes
   cRender->renderMesh(cMeshBanana);
   cRender->renderMesh(cMeshCube);
   
   // sprites and text
   cRender->set2D();
   cRender->renderSprite(cSpriteBall);
   
   for(int i = 0; i < FINGERS; i++)
      cRender->renderSprite(cSpriteInfo[i]);
   
   cText->setColor(0.1f, 0.1f, 0.1f);
   cText->move(0.005f, 0.005f, 0.0f);
   cRender->renderLabel(cText);
   cText->setColor(0.75f, 0.0f, 0.0f);
   cText->move(-0.005f, -0.005f, 0.0f);
   cRender->renderLabel(cText);
   
   cRender->renderEnd();
}

void GESceneSample::release()
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

void GESceneSample::inputTouchBegin(int ID, CGPoint* Point)
{
   cAudio->playSound(Sounds.Touch, 1);
   cSpriteInfo[ID]->setPosition(cPixelToPositionX->y(Point->x), cPixelToPositionY->y(Point->y), 0.0f);
   cSpriteInfo[ID]->show();
}

void GESceneSample::inputTouchMove(int ID, CGPoint* PreviousPoint, CGPoint* CurrentPoint)
{
   if(ID == 0)
   {
      cCamera->move((CurrentPoint->x - PreviousPoint->x) * TOUCH_SCALE, 
                    (-CurrentPoint->y + PreviousPoint->y) * TOUCH_SCALE, 
                    0.0f);
   }
   
   cSpriteInfo[ID]->setPosition(cPixelToPositionX->y(CurrentPoint->x), 
                                cPixelToPositionY->y(CurrentPoint->y));
}

void GESceneSample::inputTouchEnd(int ID, CGPoint* Point)
{
   cSpriteInfo[ID]->hide();
}

void GESceneSample::updateAccelerometerStatus(float X, float Y, float Z)
{
   vBallVelocity.X += X * ACC_SCALE;
   vBallVelocity.Y += Y * ACC_SCALE;
}
