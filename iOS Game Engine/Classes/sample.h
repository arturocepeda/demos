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

#define FINGERS 5

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
   
   void render();
   
public:
   GESceneSample(GERendering* Render, GEAudio* Audio, void* GlobalData);
   
   void init();
	void update();
	void release();
   
   void inputTouchBegin(int ID, CGPoint* Point);
   void inputTouchMove(int ID, CGPoint* PreviousPoint, CGPoint* CurrentPoint);
   void inputTouchEnd(int ID, CGPoint* Point);
};

#endif
