
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
#include "Core/GEEntity.h"
#include "Core/GETime.h"
#include "Rendering/GERenderSystem.h"
#include "Audio/GEAudioSystem.h"
#include "Rendering/GEPrimitives.h"

using namespace GE;
using namespace GE::Core;
using namespace GE::Rendering;
using namespace GE::Audio;

ObjectName _Camera_("Camera");
ObjectName _Light_("Light");
ObjectName _Background_("Background");
ObjectName _CubeTexture_("CubeTexture");
ObjectName _Ball_("Ball");
ObjectName _Info_("Info");
ObjectName _Title_("Title");

const float RotationSpeedFactor = 0.5f;

GEStateSample::GEStateSample(void* GlobalData)
   : State(GlobalData)
   , fMeshCubeR(1.0f)
   , fMeshCubeRInc(-0.01f)
   , fMeshCubeG(0.5f)
   , fMeshCubeGInc(0.005f)
   , fMeshCubeB(0.2f)
   , fMeshCubeBInc(0.015f)
{
}

#include "Core/GEObjectManager.h"

void GEStateSample::internalInit()
{ 
   ObjectManager<Entity> man;

   RenderSystem* cRender = RenderSystem::getInstance();
   AudioSystem* cAudio = AudioSystem::getInstance();

   cRender->setBackgroundColor(Color(0.1f, 0.1f, 0.3f));
   cRender->setAmbientLightColor(Color(0.25f, 0.25f, 0.25f));

   // textures
   cRender->loadTexture("TextureBackground", "background", "jpg");
   cRender->loadTexture("TextureBanana", "banana", "jpg");
   cRender->loadTexture("TextureInfo", "info", "png");
   cRender->loadTexture("TextureBasketball", "basketball", "png");
      
   // sounds
   //cAudio->loadSound(Sounds.Music, "song", "caf");
   cAudio->loadSound(Sounds.Touch, "touch", "wav");
   cAudio->setVolume(0, 0.2f);
   cAudio->playSound(Sounds.Music, 1);
   
   // font
   //cRender->defineFont(0, "Optima-ExtraBlack", 44.0f);
   cRender->defineFont("FontTest", "Test", 24.0f);

   // materials
   cRender->loadMaterials("sample");

   // scene
   cScene = new Scene("Scene");

   Entity* cEntity = 0;
   ComponentTransform* cTransform = 0;
   ComponentLight* cLight = 0;
   ComponentCamera* cCamera = 0;
   ComponentMesh* cMesh = 0;
   ComponentSprite* cSprite = 0;
   ComponentUILabel* cLabel = 0;

   // light
   cEntity = cScene->addEntity(_Light_);
   cTransform = cEntity->addComponent<ComponentTransform>();
   cLight = cEntity->addComponent<ComponentLight>();
   cLight->setLightType(LightType::Directional);
   cLight->setColor(Color(1.0f, 1.0f, 1.0f));
   cLight->setLinearAttenuation(0.1f);

   // camera
   cEntity = cScene->addEntity(_Camera_);
   cTransform = cEntity->addComponent<ComponentTransform>();
   cTransform->setPosition(0.0f, 5.0f, 15.0f);
   cTransform->setForwardVector(Vector3(0.0f, 0.0f, -1.0f));
   cCamera = cEntity->addComponent<ComponentCamera>();
   cRender->setActiveCamera(cCamera);

   // meshes
   cEntity = cScene->addEntity(_CubeTexture_);
   cTransform = cEntity->addComponent<ComponentTransform>();
   cTransform->setScale(0.05f, 0.05f, 0.05f);
   cMesh = cEntity->addComponent<ComponentMesh>();
   cMesh->loadFromFile("humanoid");
   cMesh->setMaterial(cRender->getMaterial("MaterialBanana"));

   // sprites
   cEntity = cScene->addEntity(_Background_);
   cTransform = cEntity->addComponent<ComponentTransform>();
   cSprite = cEntity->addComponent<ComponentSprite>();
   cSprite->setLayer(SpriteLayer::Pre3D);
   cSprite->setMaterial(cRender->getMaterial("MaterialBackground"));
   cSprite->setSize(Vector2(2.0f, 4.0f));

   cEntity = cScene->addEntity(_Ball_);
   cTransform = cEntity->addComponent<ComponentTransform>();
   cSprite = cEntity->addComponent<ComponentSprite>();
   cSprite->setMaterial(cRender->getMaterial("MaterialBasketball"));
   cSprite->setSize(Vector2(0.4f, 0.4f));

#if defined GE_PLATFORM_WINDOWS
   cSprite->setVisible(false);
#endif

   for(int i = 0; i < FINGERS; i++)
   {
      char sBuffer[8];
      sprintf(sBuffer, "Info%02d", i + 1);
      cEntity = cScene->addEntity(sBuffer);
      cEntitiesInfo[i] = cEntity;
      cTransform = cEntity->addComponent<ComponentTransform>();
      cSprite = cEntity->addComponent<ComponentSprite>();
      cSprite->setMaterial(cRender->getMaterial("MaterialInfo"));
      cSprite->setSize(Vector2(0.25f, 0.25f));
      cSprite->setVisible(false);
   }

   // text
   cEntity = cScene->addEntity(_Title_);
   cTransform = cEntity->addComponent<ComponentTransform>();
   cTransform->setPosition(0.0f, 1.15f);
   cLabel = cEntity->addComponent<ComponentUILabel>();
   cLabel->setMaterial(cRender->getMaterial("MaterialText"));
   cLabel->getMaterial()->DiffuseColor = Color(1.0f, 0.25f, 0.25f, 0.0f);
   cLabel->setFont(cRender->getFont("FontTest"));
   cLabel->setAligment(Alignment::CenterCenter);
   cLabel->setCharacterSize(Vector2(0.16f, 0.16f));
   cLabel->setText("Game Engine");
}

void GEStateSample::update()
{
   updateCube();
   updateBall();
   updateText();
   cScene->update();
   cScene->render();
}

void GEStateSample::updateText()
{
   Entity* cText = cScene->getEntity(_Title_);

   ComponentRenderable* cRenderable = cText->getComponent<ComponentRenderable>();
   Material* sMaterial = cRenderable->getMaterial();

   if(sMaterial->DiffuseColor.getOpacity() < 1.0f)   
      sMaterial->DiffuseColor.setOpacity(sMaterial->DiffuseColor.getOpacity() + 0.005f);
}

void GEStateSample::updateCube()
{
   float fRotationSpeed = RotationSpeedFactor * Time::getDelta();

   Entity* cCube = cScene->getEntity(_CubeTexture_);
   ComponentTransform* cTransform = cCube->getComponent<ComponentTransform>();
   Rotation cRotation(Vector3(0.0f, 1.0f, 0.0f), fRotationSpeed);
   cTransform->rotate(cRotation);

   ComponentRenderable* cRenderable = cCube->getComponent<ComponentRenderable>();
   cRenderable->getMaterial()->DiffuseColor = Color(fMeshCubeR, fMeshCubeG, fMeshCubeB);
   
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
   Entity* cBall = cScene->getEntity(_Ball_);
   ComponentTransform* cTransform = cBall->getComponent<ComponentTransform>();

   // get ball position
   vBallPosition = cTransform->getPosition();
   
   // bounds control (left/right)
   if((vBallPosition.X < BOUNDS_LEFT) ||
      (vBallPosition.X > BOUNDS_RIGHT))
   {
      // correct position
      if(vBallPosition.X < 0.0f)
         vBallPosition.X = BOUNDS_LEFT;
      else
         vBallPosition.X = BOUNDS_RIGHT;
      
      cTransform->setPosition(vBallPosition);
      
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
      
      cTransform->setPosition(vBallPosition);
      
      // bounce
      vBallVelocity.Y = fabs(vBallVelocity.Y) > STOPPED ? -vBallVelocity.Y * BOUNCE : 0.0f;
   }
   
   // move and rotate the ball
   cTransform->move(vBallVelocity);
   cTransform->rotate(Rotation(Vector3::UnitZ, (vBallPosition.Y < 0.0f ? 1.0f : -1.0f) * vBallVelocity.X * ROTATION));
   cTransform->rotate(Rotation(Vector3::UnitZ, (vBallPosition.X < 0.0f ? -1.0f : 1.0f) * vBallVelocity.Y * ROTATION));
}

void GEStateSample::release()
{
   AudioSystem* cAudio = AudioSystem::getInstance();

   // stop audio sources and release sounds
   //cAudio->stop(Sounds.Music);
   cAudio->stop(Sounds.Touch);
   cAudio->unloadAllSounds();

   // release scene
   delete cScene;
}

void GEStateSample::inputMouseWheel(int Delta)
{
   Entity* cCamera = cScene->getEntity(_Camera_);
   ComponentTransform* cTransform = cCamera->getComponent<ComponentTransform>();
   cTransform->move(cTransform->getForwardVector() * Time::getDelta() * (float)Delta * 100.0f);
}

void GEStateSample::inputTouchBegin(int ID, const Vector2& Point)
{
   AudioSystem* cAudio = AudioSystem::getInstance();
   cAudio->playSound(Sounds.Touch, 0);

   ComponentTransform* cTransform = cEntitiesInfo[ID]->getComponent<ComponentTransform>();
   cTransform->setPosition(Point.X, Point.Y);

   ComponentRenderable* cRenderable = cEntitiesInfo[ID]->getComponent<ComponentRenderable>();
   cRenderable->show();

   Entity* cam = cScene->getEntity(_Camera_);
   cam->getComponent<ComponentTransform>()->move(0.0f, 0.0f, 0.1f);
}

void GEStateSample::inputTouchMove(int ID, const Vector2& PreviousPoint, const Vector2& CurrentPoint)
{
   if(ID == 0)
   {
      Entity* cCamera = cScene->getEntity(_Camera_);
      ComponentTransform* cTransform = cCamera->getComponent<ComponentTransform>();

      cTransform->move((CurrentPoint.X - PreviousPoint.X) * TOUCH_SCALE,
                       (CurrentPoint.Y - PreviousPoint.Y) * TOUCH_SCALE,
                       0.0f);
   }

   ComponentTransform* cTransform = cEntitiesInfo[ID]->getComponent<ComponentTransform>();
   cTransform->setPosition(CurrentPoint.X, CurrentPoint.Y);
}

void GEStateSample::inputTouchEnd(int ID, const Vector2& Point)
{
   ComponentRenderable* cRenderable = cEntitiesInfo[ID]->getComponent<ComponentRenderable>();
   cRenderable->hide();
}

void GEStateSample::updateAccelerometerStatus(const Vector3& Status)
{
   vBallVelocity.X += Status.X * ACC_SCALE;
   vBallVelocity.Y += Status.Y * ACC_SCALE;
}
