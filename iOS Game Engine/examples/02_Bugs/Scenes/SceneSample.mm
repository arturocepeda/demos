
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  iOS Game Engine
//
//  Sample application
//
//  --- SceneSample.mm ---
//
//////////////////////////////////////////////////////////////////


#include "SceneSample.h"
#include "GEUtils.h"

GESceneSample::GESceneSample(GERendering* Render, GEAudio* Audio, void* GlobalData) :
               GEScene(Render, Audio, GlobalData)
{
}

void GESceneSample::init()
{   
   iNextScene = -1;
   iCurrentFrame = 1;
   iProbability = 1;
   cRender->set2D();
   
   // bugs
   iEscaped = 0;
   iSmashed = 0;
   
   // textures
   cRender->loadTexture(Textures.BugA1, @"bugA1.png");
   cRender->loadTexture(Textures.BugA2, @"bugA2.png");
   cRender->loadTexture(Textures.BugA3, @"bugA3.png");
   cRender->loadTexture(Textures.BugB1, @"bugB1.png");
   cRender->loadTexture(Textures.BugB2, @"bugB2.png");
   cRender->loadTexture(Textures.BugB3, @"bugB3.png");
   cRender->loadTexture(Textures.BugC1, @"bugC1.png");
   cRender->loadTexture(Textures.BugC2, @"bugC2.png");
   cRender->loadTexture(Textures.BugC3, @"bugC3.png");
   cRender->loadTexture(Textures.Floor, @"floor.jpg");

   // sprites
   cSpriteBackground = new GESprite();
   cSpriteBackground->setTexture(cRender->getTexture(Textures.Floor));
   cSpriteBackground->setScale(1.5f, 1.8f, 1.0f);
   
   cSpriteBug[0][0] = new GESprite();
   cSpriteBug[0][0]->setTexture(cRender->getTexture(Textures.BugA1));
   cSpriteBug[0][1] = new GESprite();
   cSpriteBug[0][1]->setTexture(cRender->getTexture(Textures.BugA2));
   cSpriteBug[0][2] = new GESprite();
   cSpriteBug[0][2]->setTexture(cRender->getTexture(Textures.BugA3));
   
   cSpriteBug[1][0] = new GESprite();
   cSpriteBug[1][0]->setTexture(cRender->getTexture(Textures.BugB1));
   cSpriteBug[1][1] = new GESprite();
   cSpriteBug[1][1]->setTexture(cRender->getTexture(Textures.BugB2));
   cSpriteBug[1][2] = new GESprite();
   cSpriteBug[1][2]->setTexture(cRender->getTexture(Textures.BugB3));
   
   cSpriteBug[2][0] = new GESprite();
   cSpriteBug[2][0]->setTexture(cRender->getTexture(Textures.BugC1));
   cSpriteBug[2][1] = new GESprite();
   cSpriteBug[2][1]->setTexture(cRender->getTexture(Textures.BugC2));
   cSpriteBug[2][2] = new GESprite();
   cSpriteBug[2][2]->setTexture(cRender->getTexture(Textures.BugC3));
   
   // text
   cTextSmashed = new GELabel(@"Smashed: 0", @"Papyrus", 24.0f, UITextAlignmentLeft, 256, 64);
   cTextSmashed->setPosition(-0.9f, 1.3f);
   cTextSmashed->setScale(1.5f, 1.5f);
   cTextSmashed->setColor(0.25f, 0.25f, 0.5f);
   
   cTextEscaped = new GELabel(@"Escaped: 0", @"Papyrus", 24.0f, UITextAlignmentLeft, 256, 64);
   cTextEscaped->setPosition(0.09f, 1.3f);
   cTextEscaped->setScale(1.5f, 1.5f);
   cTextEscaped->setColor(0.5f, 0.25f, 0.25f);
   
   // sound
   cAudio->loadSound(0, @"hit.wav");
   
   // randomize
   srand(time(0));
}

void GESceneSample::release()
{
   // release rendering objects
   delete cSpriteBackground;
   
   for(int i = 0; i < BUG_TYPES; i++)
   {
      for(int j = 0; j < BUG_STEPS; j++)
         delete cSpriteBug[i][j];
   }
   
   delete cTextEscaped;
   delete cTextSmashed;
   
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
         cTextEscaped->setText([[NSString alloc] initWithFormat:@"Escaped: %d", iEscaped]);
         
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
   
   memset(&sBug.Position, 0, sizeof(GEVector));
   memset(&sBug.Destiny, 0, sizeof(GEVector));
   
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
   cRender->useProgram(GEPrograms.HUD);
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
   cRender->useProgram(GEPrograms.Text);
   cRender->renderLabel(cTextSmashed);
   cRender->renderLabel(cTextEscaped);
}

void GESceneSample::inputTouchBegin(int ID, CGPoint* Point)
{
   // just one finger at the same time!
   if(ID > 0)
      return;
   
   // hit sound
   cAudio->playSound(0, 0);
   
   // get finger position
   float fFingerX = cPixelToPositionX->y(Point->x);
   float fFingerY = cPixelToPositionY->y(Point->y);
   
   // check if a bug has been smashed
   for(unsigned int i = 0; i < vBugs.size(); i++)
   {
      if((pow(fFingerX - vBugs[i].Position.X, 2) + pow(fFingerY - vBugs[i].Position.Y, 2)) < 
          pow(vBugs[i].Size * 2, 2))
      {
         // add the bug to the smashed list and increment the number of smashed bugs
         vBugsSmashed.push_back(vBugs[i]);
         iSmashed++;
         
         // update label
         cTextSmashed->setText([[NSString alloc] initWithFormat:@"Smashed: %d", iSmashed]);
         
         // remove bug from the bugs list
         vBugs.erase(vBugs.begin() + i);
         return;
      }
   }
}

void GESceneSample::inputTouchMove(int ID, CGPoint* PreviousPoint, CGPoint* CurrentPoint)
{
}

void GESceneSample::inputTouchEnd(int ID, CGPoint* Point)
{
}

void GESceneSample::updateAccelerometerStatus(float X, float Y, float Z)
{
}
