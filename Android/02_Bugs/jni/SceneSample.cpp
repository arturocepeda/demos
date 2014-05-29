
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Sample application
//
//  --- SceneSample.cpp ---
//
//////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstdlib>
#include <ctime>

#include "SceneSample.h"
#include "Core/GEUtils.h"
#include "Core/GEConstants.h"
#include "Core/GEDevice.h"

GESceneSample::GESceneSample(GERendering* Render, GEAudio* Audio, void* GlobalData)
   : GEScene(Render, Audio, GlobalData)
{
}

void GESceneSample::internalInit()
{
   iCurrentFrame = 1;
   iProbability = 1;
   cRender->set2D();
   
   // bugs
   iEscaped = 0;
   iSmashed = 0;
   
   // textures
   cRender->loadTexture(Textures.BugA1, "bugA1", "png");
   cRender->loadTexture(Textures.BugA2, "bugA2", "png");
   cRender->loadTexture(Textures.BugA3, "bugA3", "png");
   cRender->loadTexture(Textures.BugB1, "bugB1", "png");
   cRender->loadTexture(Textures.BugB2, "bugB2", "png");
   cRender->loadTexture(Textures.BugB3, "bugB3", "png");
   cRender->loadTexture(Textures.BugC1, "bugC1", "png");
   cRender->loadTexture(Textures.BugC2, "bugC2", "png");
   cRender->loadTexture(Textures.BugC3, "bugC3", "png");
   cRender->loadTexture(Textures.Floor, "floor", "jpg");

   // sprites
   cRender->createSprite(&cSpriteBackground);
   cSpriteBackground->setTexture(cRender->getTexture(Textures.Floor));
   cSpriteBackground->setScale(1.5f, 1.8f, 1.0f);
   
   cRender->createSprite(&cSpriteBug[0][0]);
   cSpriteBug[0][0]->setTexture(cRender->getTexture(Textures.BugA1));
   cRender->createSprite(&cSpriteBug[0][1]);
   cSpriteBug[0][1]->setTexture(cRender->getTexture(Textures.BugA2));
   cRender->createSprite(&cSpriteBug[0][2]);
   cSpriteBug[0][2]->setTexture(cRender->getTexture(Textures.BugA3));
   
   cRender->createSprite(&cSpriteBug[1][0]);
   cSpriteBug[1][0]->setTexture(cRender->getTexture(Textures.BugB1));
   cRender->createSprite(&cSpriteBug[1][1]);
   cSpriteBug[1][1]->setTexture(cRender->getTexture(Textures.BugB2));
   cRender->createSprite(&cSpriteBug[1][2]);
   cSpriteBug[1][2]->setTexture(cRender->getTexture(Textures.BugB3));
   
   cRender->createSprite(&cSpriteBug[2][0]);
   cSpriteBug[2][0]->setTexture(cRender->getTexture(Textures.BugC1));
   cRender->createSprite(&cSpriteBug[2][1]);
   cSpriteBug[2][1]->setTexture(cRender->getTexture(Textures.BugC2));
   cRender->createSprite(&cSpriteBug[2][2]);
   cSpriteBug[2][2]->setTexture(cRender->getTexture(Textures.BugC3));
   
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

void GESceneSample::release()
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

void GESceneSample::update()
{
   iCurrentFrame++;
   
   // a new bug?
   if(eventOccurs(iProbability))
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

void GESceneSample::generateBug()
{
   // create bug
   SBug sBug;
   
   sBug.Type = rand() % BUG_TYPES;
   sBug.Size = (float)random(BUG_SIZE_MIN, BUG_SIZE_MAX) * 0.01f;
   sBug.Speed = (float)random(BUG_SPEED_MIN, BUG_SPEED_MAX) * 0.001f;
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
   sBug.Angle = (PI * 0.5f) + atan2(sBug.Direction.Y, sBug.Direction.X);
   
   // add to the bug list
   vBugs.push_back(sBug);
}

void GESceneSample::render()
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
      cSprite->setScale(vBugs[i].Size, vBugs[i].Size);
      cSprite->setRotation(0.0f, 0.0f, vBugs[i].Angle);
      cSprite->setOpacity(1.0f);
      cRender->renderSprite(cSprite);
   }
   
   // smashed bugs
   for(i = 0; i < vBugsSmashed.size(); i++)
   {
      cSprite = cSpriteBug[vBugsSmashed[i].Type][vBugsSmashed[i].CurrentStep];
      cSprite->setPosition(vBugsSmashed[i].Position.X, vBugsSmashed[i].Position.Y);
      cSprite->setScale(vBugsSmashed[i].Size, vBugsSmashed[i].Size);
      cSprite->setRotation(0.0f, 0.0f, vBugsSmashed[i].Angle);
      cSprite->setOpacity(vBugsSmashed[i].Opacity);
      cRender->renderSprite(cSprite);
   }
   
   // text
   cRender->useShaderProgram((unsigned int)GEShaderPrograms::Text);
   cRender->renderLabel(cTextSmashed);
   cRender->renderLabel(cTextEscaped);
}

void GESceneSample::inputTouchBegin(int ID, const GEVector2& Point)
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
