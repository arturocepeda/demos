//
//  sample.h
//  GameEngine sample
//
//  Created by Arturo Cepeda on 10/07/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef GameEngine_GESceneSample_h
#define GameEngine_GESceneSample_h

#include "GEScene.h"

#define BOUNDS_TOP      (CDevice::getOrthoTop() * 0.89f)
#define BOUNDS_BOTTOM   (CDevice::getOrthoBottom() * 0.89f)
#define BOUNDS_LEFT     (CDevice::getOrthoLeft() * 0.83f)
#define BOUNDS_RIGHT    (CDevice::getOrthoRight() * 0.83f)

#define FINGERS      5
#define ACC_SCALE    0.004f
#define STOPPED      0.004f
#define BOUNCE       0.55f
#define ROTATION     280

class GESceneSample : public GEScene
{
private:
   GECamera* cCamera;
   
   GEMesh* cMeshBanana;
   GEMesh* cMeshCube;
   
   GESprite* cSpriteBall;
   GESprite* cSpriteInfo[FINGERS];
   GESprite* cBackground;
   
   GELabel* cText;
   
   GEVector vBallPosition;
   GEVector vBallVelocity;
   
   void updateText();
   void updateBanana();
   void updateCube();
   void updateBall();
   
   void render();
   
public:
   GESceneSample(GERendering* Render, GEAudio* Audio, void* GlobalData);
   
   void init();
	void update();
	void release();
   
   void inputTouchBegin(int ID, CGPoint* Point);
   void inputTouchMove(int ID, CGPoint* PreviousPoint, CGPoint* CurrentPoint);
   void inputTouchEnd(int ID, CGPoint* Point);
   
   void updateAccelerometerStatus(float X, float Y, float Z);
};

#endif
