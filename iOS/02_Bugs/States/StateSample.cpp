
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

#include <iostream>
#include <cstdlib>
#include <ctime>

#include "StateSample.h"
#include "Core/GEUtils.h"
#include "Core/GEConstants.h"
#include "Core/GEDevice.h"

GEStateSample::GEStateSample(GERendering* Render, GEAudio* Audio, void* GlobalData)
   : GEState(Render, Audio, GlobalData)
   , cRandEvent(0.0f)
   , cRandBugType(0, BUG_TYPES - 1)
   , cRandBugSize(BUG_SIZE_MIN * 0.01f, BUG_SIZE_MAX * 0.01f)
   , cRandBugSpeed(BUG_SPEED_MIN * 0.001f, BUG_SPEED_MAX * 0.001f)
{
}

void GEStateSample::internalInit()
{
   iCurrentFrame = 1;
   iProbability = 1;
   cRender->set2D();
   
   // bugs
   iEscaped = 0;
   iSmashed = 0;
   
   // textures
   cRender->loadTexture(0, "floor", "jpg", false);
   cRender->loadTexture(1, "bugs", "png", true);

   // sprites
   cRender->createSprite(&cSpriteBackground);
   cSpriteBackground->setTexture(cRender->getTexture(0));
   cSpriteBackground->setSize(GEVector2(4.0f, 4.0f));
   
   for(unsigned int i = 0; i < BUG_TYPES; i++)
   {
      for(unsigned int j = 0; j < BUG_STEPS; j++)
      {
         cRender->createSprite(&cSpriteBug[i][j]);
         cSpriteBug[i][j]->setTexture(cRender->getTexture(1));
         cSpriteBug[i][j]->setTextureAtlasIndex(1 + i * BUG_TYPES + j);
      }
   }
   
   // font
   //cRender->defineFont(0, "Papyrus", 24.0f);
   cRender->defineFont(0, "Test", 24.0f);
    
   // text
   cRender->createLabel(&cTextSmashed, 0, TopLeft, GEVector2(0.1f, 0.1f), "Smashed: 0");
   cTextSmashed->setPosition(-0.9f, 1.3f);
   cTextSmashed->setColor(GEColor(0.5f, 0.5f, 0.8f));
   cTextSmashed->setHorizontalSpacing(0.1f);
   
   cRender->createLabel(&cTextEscaped, 0, TopLeft, GEVector2(0.1f, 0.1f), "Escaped: 0");
   cTextEscaped->setPosition(0.09f, 1.3f);
   cTextEscaped->setColor(GEColor(0.8f, 0.5f, 0.5f));
   cTextEscaped->setHorizontalSpacing(0.1f);
   
   // sound
   cAudio->loadSound(0, "hit", "wav");
   
   // randomize
   srand(time(0));
}

void GEStateSample::release()
{
   // release rendering objects
   cRender->releaseSprite(&cSpriteBackground);
   
   for(int i = 0; i < BUG_TYPES; i++)
   {
      for(int j = 0; j < BUG_STEPS; j++)
         cRender->releaseSprite(&cSpriteBug[i][j]);
   }
   
   cRender->releaseLabel(&cTextEscaped);
   cRender->releaseLabel(&cTextSmashed);

   cRender->releaseFont(0);
   
   // release sound buffers
   cAudio->unloadAllSounds();
}

void GEStateSample::update(float DeltaTime)
{
   iCurrentFrame++;

   // a new bug?
   cRandEvent.setProbability(iProbability / 100.0f);

   if(cRandEvent.occurs())
      generateBug();
   
   // running bugs
   for(unsigned int i = 0; i < vBugs.size(); i++)
   {
      vBugs[i].Position.X += vBugs[i].Direction.X * vBugs[i].Speed;
      vBugs[i].Position.Y += vBugs[i].Direction.Y * vBugs[i].Speed;
      
      // target reached?
      if((pow(vBugs[i].Destiny.X - vBugs[i].Position.X, 2) + pow(vBugs[i].Destiny.Y - vBugs[i].Position.Y, 2)) < 
          pow(vBugs[i].Size, 2))
      {
         // remove bug from the bugs list and increment the number of escaped bugs
         vBugs.erase(vBugs.begin() + i);
         iEscaped++;
         
         // update label
         sprintf(sTextBuffer, "Escaped: %d", iEscaped);
         cTextEscaped->setText(sTextBuffer);
         
         i--;         
         continue;
      }

      if(iCurrentFrame % 4 == 0)
      {
         vBugs[i].CurrentStep++;
      
         if(vBugs[i].CurrentStep == BUG_STEPS)
            vBugs[i].CurrentStep = 0;
      }
   }
   
   // smashed bugs
   for(unsigned int i = 0; i < vBugsSmashed.size(); i++)
   {
      vBugsSmashed[i].Opacity -= 0.02f;
      
      if(vBugsSmashed[i].Opacity <= 0.0f)
      {
         vBugsSmashed.erase(vBugsSmashed.begin() + i);
         i--;
      }
   }
   
   // probability
   if(iCurrentFrame % PROB_CHANGE == 0) 
      iProbability++;
}

void GEStateSample::generateBug()
{
   // create bug
   SBug sBug;
   
   sBug.Type = cRandBugType.generate();
   sBug.Size = cRandBugSize.generate();
   sBug.Speed = cRandBugSpeed.generate();
   sBug.Opacity = 1.0f;
   sBug.CurrentStep = 0;
   
   memset(&sBug.Position, 0, sizeof(GEVector3));
   memset(&sBug.Destiny, 0, sizeof(GEVector3));
   
   // line
   int iType = rand() % 4;
   
   switch(iType) 
   {
      // up to down
      case 0:         
         sBug.Position.X = (float)(-100 + rand() % 200) * 0.01f;
         sBug.Position.Y = BOUNDS_TOP + 0.2f;
         sBug.Destiny.X = (float)(-100 + rand() % 200) * 0.01f; 
         sBug.Destiny.Y = BOUNDS_BOTTOM - 0.2f;
         break;
      
      // down to up
      case 1:
         sBug.Position.X = (float)(-100 + rand() % 200) * 0.01f;
         sBug.Position.Y = BOUNDS_BOTTOM - 0.2f;
         sBug.Destiny.X = (float)(-100 + rand() % 200) * 0.01f; 
         sBug.Destiny.Y = BOUNDS_TOP + 0.2f;
         break;
         
      // left to right
      case 2:
         sBug.Position.X = BOUNDS_LEFT - 0.2f;
         sBug.Position.Y = (float)(-150 + rand() % 300) / 100.0f;
         sBug.Destiny.X = BOUNDS_RIGHT + 0.2f;
         sBug.Destiny.Y = (float)(-150 + rand() % 300) / 100.0f; 
         break;
         
      // right to left
      case 3:
         sBug.Position.X = BOUNDS_RIGHT + 0.2f;
         sBug.Position.Y = (float)(-150 + rand() % 300) / 100.0f;
         sBug.Destiny.X = BOUNDS_LEFT - 0.2f;
         sBug.Destiny.Y = (float)(-150 + rand() % 300) / 100.0f;
         break;
   }
   
   sBug.Direction = sBug.Destiny - sBug.Position;
   sBug.Direction.normalize();
   sBug.Angle = atan2(sBug.Direction.Y, sBug.Direction.X);
   
   // add to the bug list
   vBugs.push_back(sBug);
}

void GEStateSample::render()
{
   unsigned int i;
   GESprite* cSprite;
   
   // background
   cRender->useShaderProgram((unsigned int)GEShaderPrograms::HUD);
   cRender->renderSprite(cSpriteBackground);
   
   // running bugs
   for(i = 0; i < vBugs.size(); i++)
   {
      cSprite = cSpriteBug[vBugs[i].Type][vBugs[i].CurrentStep];
      cSprite->setPosition(vBugs[i].Position.X, vBugs[i].Position.Y);
      cSprite->setSize(GEVector2(vBugs[i].Size, vBugs[i].Size));
      cSprite->setRotation(0.0f, 0.0f, vBugs[i].Angle);
      cSprite->setOpacity(1.0f);
      cRender->renderSprite(cSprite);
   }
   
   // smashed bugs
   for(i = 0; i < vBugsSmashed.size(); i++)
   {
      cSprite = cSpriteBug[vBugsSmashed[i].Type][vBugsSmashed[i].CurrentStep];
      cSprite->setPosition(vBugsSmashed[i].Position.X, vBugsSmashed[i].Position.Y);
      cSprite->setSize(GEVector2(vBugsSmashed[i].Size, vBugsSmashed[i].Size));
      cSprite->setRotation(0.0f, 0.0f, vBugsSmashed[i].Angle);
      cSprite->setOpacity(vBugsSmashed[i].Opacity);
      cRender->renderSprite(cSprite);
   }
   
   // text
   cRender->useShaderProgram((unsigned int)GEShaderPrograms::Text);
   cRender->renderLabel(cTextSmashed);
   cRender->renderLabel(cTextEscaped);
}

void GEStateSample::inputTouchBegin(int ID, const GEVector2& Point)
{
   // just one finger at the same time!
   if(ID > 0)
      return;
   
   // hit sound
   cAudio->playSound(0, 0);
   
   // get finger position
   GEVector2 vFinger = cRender->pixelToScreen(Point);
   
   // check if a bug has been smashed
   for(unsigned int i = 0; i < vBugs.size(); i++)
   {
      if((pow(vFinger.X - vBugs[i].Position.X, 2) + pow(vFinger.Y - vBugs[i].Position.Y, 2)) <
          pow(vBugs[i].Size * 2, 2))
      {
         // add the bug to the smashed list and increment the number of smashed bugs
         vBugsSmashed.push_back(vBugs[i]);
         iSmashed++;
         
         // update label
         sprintf(sTextBuffer, "Smashed: %d", iSmashed);
         cTextSmashed->setText(sTextBuffer);
         
         // remove bug from the bugs list
         vBugs.erase(vBugs.begin() + i);
         return;
      }
   }
}
