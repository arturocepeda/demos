//
//  sample.mm
//  GameEngine sample
//
//  Created by Arturo Cepeda on 10/07/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "sample.h"
#include "banana.h"
#include "cube.h"

GESceneSample::GESceneSample(GERendering* Render, GEAudio* Audio, void* GlobalData) :
               GEScene(Render, Audio, GlobalData)
{
}

void GESceneSample::init()
{   
   iNextScene = -1;
   
   // device info
   NSLog(@"\nDevice type: %s", (CDevice::iPhone())? "iPhone": "iPad");
   
   if(!CDevice::iPhone())
      NSLog(@"\nRetina display: %s", (CDevice::displayRetina())? "yes": "no");
   
   // cameras
   cCamera = new GECamera();
   cCamera->setPosition(0.07f, 0.0f, 0.0f);
   cCamera->setAngle(0.0f, 30.0f, 0.0f);
   cCamera->use();
   
   // textures
   cRender->loadTexture(0, @"banana.jpg");
   cRender->loadTexture(1, @"info.png");
   cRender->loadTexture(2, @"basketball.png");
   cRender->loadTexture(3, @"brick_wall.jpeg");
   
   // meshes
   cMeshBanana = new GEMesh();
   cMeshBanana->loadFromHeader(bananaNumVerts, bananaVerts, bananaNormals, cRender->getTexture(0),
                               bananaTexCoords);
   cMeshBanana->scale(1.5f, 1.5f, 1.5f);
   
   cMeshCube = new GEMesh();
   cMeshCube->loadFromHeader(cubeNumVerts, cubeVerts, cubeNormals);
   cMeshCube->setPosition(0.5f, -1.0f, -2.0f);
   cMeshCube->scale(0.5f, 0.5f, 0.5f);
   cMeshCube->setColor(0.75f, 0.25f, 0.25f);
   cMeshCube->setOpacity(0.75f);
   
   // sprites
   cSpriteBall = new GESprite(cRender->getTexture(2), cRender->getTextureSize(2));
   cSpriteBall->setPosition(2.0f, 0.9f, 0.0f);
   cSpriteBall->rotate(0.0f, 0.0f, 0.0f);
   cSpriteBall->scale(0.4f, 0.4f, 0.4f);
   
   for(int i = 0; i < FINGERS; i++)
   {
      cSpriteInfo[i] = new GESprite(cRender->getTexture(1), cRender->getTextureSize(1));
      cSpriteInfo[i]->scale(0.15f, 0.15f, 0.15f);
      cSpriteInfo[i]->rotate(0.0f, 0.0f, 90.0f);
      cSpriteInfo[i]->setVisible(false);
   }
   
   cBackground = new GESprite(cRender->getTexture(3), cRender->getTextureSize(3));
   cBackground->rotate(0.0f, 0.0f, 90.0f);
   cBackground->scale(2.0f, 2.0f, 2.0f);
   
   // sounds
   cAudio->loadSound(0, @"song.caf");
   cAudio->loadSound(1, @"cartoon008.wav");   
   cAudio->setSourceVolume(1, 0.25f);
   cAudio->playSound(0, 0);
   
   // text
   cText = new GELabel(@"ARTURO CEPEDA\niOS Game Engine", @"Optima-ExtraBlack", 44.0f,
                       UITextAlignmentCenter, 512);
   cText->setPosition(-0.5f, -0.5f, 0.0f);
   cText->setScale(2.4f, 2.4f, 2.4f);
   cText->setOpacity(0.0f);
}

void GESceneSample::update()
{
   render();   
  
   cMeshBanana->rotate(0.1f, 0.1f, 0.1f);
   cMeshCube->rotate(0.4f, 0.6f, 0.8f);
   
   cSpriteBall->move(-0.005f, -0.003f, 0.0f);
   cSpriteBall->rotate(0.0f, 0.0f, 4.5f);
   
   cText->setOpacity(cText->getOpacity() + 0.001f);
}

void GESceneSample::render()
{
   cRender->renderBegin();
   
   // background
   cRender->set2D();
   cRender->renderSprite(cBackground);
   cRender->set3D();
   
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
   cAudio->stop(0);
   cAudio->stop(1);   
   cAudio->unloadAllSounds();
   
   // release objects
   delete cCamera;
   delete cMeshBanana;
   delete cMeshCube;
   delete cSpriteBall;
   
   for(int i = 0; i < FINGERS; i++)
      delete cSpriteInfo[i];
   
   delete cBackground;
   delete cText;
}

void GESceneSample::inputTouchBegin(int ID, CGPoint* Point)
{
   NSLog(@"\nFinger %d on:  (%.0f, %.0f)", ID, Point->x, Point->y);
   
   cAudio->playSound(1, 1);
   cSpriteInfo[ID]->setPosition(cPixelToPositionX->y(Point->x), cPixelToPositionY->y(Point->y), 0.0f);
   cSpriteInfo[ID]->show();
}

void GESceneSample::inputTouchMove(int ID, CGPoint* PreviousPoint, CGPoint* CurrentPoint)
{
   cSpriteInfo[ID]->setPosition(cPixelToPositionX->y(CurrentPoint->x), 
                                cPixelToPositionY->y(CurrentPoint->y), 
                                0.0f);
}

void GESceneSample::inputTouchEnd(int ID, CGPoint* Point)
{
   NSLog(@"\nFinger %d off: (%.0f, %.0f)", ID, Point->x, Point->y);
   
   cSpriteInfo[ID]->hide();
}
