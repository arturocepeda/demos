
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
#include "Rendering/GERenderSystem.h"
#include "Audio/GEAudioSystem.h"

using namespace GE;
using namespace GE::Core;
using namespace GE::States;
using namespace GE::Rendering;
using namespace GE::Audio;

ObjectName _Camera_("Camera");
ObjectName _Background_("Background");
ObjectName _Bananas_("Bananas");
ObjectName _Cube_("Cube");
ObjectName _Ball_("Ball");
ObjectName _Info_("Info");
ObjectName _Title_("Title");

const float RotationSpeedFactor = 0.0005f;

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

void GEStateSample::internalInit()
{ 
   RenderSystem* cRender = RenderSystem::getInstance();
   AudioSystem* cAudio = AudioSystem::getInstance();

   cRender->setBackgroundColor(Color(0.1f, 0.1f, 0.3f));
   
   // lighting
   cRender->setAmbientLightColor(Color(1.0f, 1.0f, 1.0f));
   cRender->setAmbientLightIntensity(0.25f);
   
   cRender->setNumberOfActiveLights(1);
   cRender->setLightPosition((unsigned int)Lights::PointLight1, Vector3(0.0f, 0.0f, 1.0f));
   cRender->setLightColor((unsigned int)Lights::PointLight1, Color(1.0f, 1.0f, 1.0f));
   cRender->setLightIntensity((unsigned int)Lights::PointLight1, 0.6f);

   // textures
   cRender->loadTexture(Textures.Background, "background", "jpg");
   cRender->loadTexture(Textures.Banana, "banana", "jpg");
   cRender->loadTexture(Textures.Info, "info", "png");
   cRender->loadTexture(Textures.Basketball, "basketball", "png");
      
   // sounds
   //cAudio->loadSound(Sounds.Music, "song", "caf");
   cAudio->loadSound(Sounds.Touch, "touch", "wav");
   cAudio->setVolume(0, 0.2f);
   cAudio->playSound(Sounds.Music, 1);
   
   // font
   //cRender->defineFont(0, "Optima-ExtraBlack", 44.0f);
   cRender->defineFont(0, "Test", 24.0f);

   // scene
   cScene = new Scene("Scene");

   Entity* cEntity = 0;
   ComponentTransform* cTransform = 0;
   ComponentCamera* cCamera = 0;
   ComponentMesh* cMesh = 0;
   ComponentSprite* cSprite = 0;
   ComponentUILabel* cLabel = 0;

   // camera
   cEntity = new Entity(_Camera_);
   cTransform = cEntity->addComponent<ComponentTransform>();
   cTransform->setPosition(0.0f, 0.0f, -4.0f);
   cCamera = cEntity->addComponent<ComponentCamera>();
   cScene->addEntity(cEntity);
   cRender->setActiveCamera(cCamera);

   // meshes
   cEntity = new Entity(_Bananas_);
   cTransform = cEntity->addComponent<ComponentTransform>();
   cTransform->scale(2.0f, 2.0f, 2.0f);
   cMesh = cEntity->addComponent<ComponentMesh>();
   cMesh->loadFromFile("banana");
   cMesh->getMaterial().ShaderProgram = ShaderPrograms::MeshTexture;
   cMesh->getMaterial().DiffuseTexture = cRender->getTexture(Textures.Banana);
   cScene->addEntity(cEntity);

   cEntity = new Entity(_Cube_);
   cTransform = cEntity->addComponent<ComponentTransform>();
   cTransform->setPosition(0.0f, -1.5f, 0.0f);
   cTransform->scale(0.5f, 0.5f, 0.5f);
   cMesh = cEntity->addComponent<ComponentMesh>();
   cMesh->loadFromFile("cube");
   cMesh->getMaterial().ShaderProgram = ShaderPrograms::MeshColor;
   cMesh->getMaterial().DiffuseColor = Color(1.0f, 0.5f, 0.2f);
   cScene->addEntity(cEntity);

   // sprites
   cEntity = new Entity(_Background_);
   cTransform = cEntity->addComponent<ComponentTransform>();
   cSprite = cEntity->addComponent<ComponentSprite>();
   cSprite->setLayer(SpriteLayer::Pre3D);
   cSprite->getMaterial().DiffuseTexture = cRender->getTexture(Textures.Background);
   cSprite->setSize(Vector2(2.0f, 4.0f));
   cScene->addEntity(cEntity);

   cEntity = new Entity(_Ball_);
   cTransform = cEntity->addComponent<ComponentTransform>();
   cSprite = cEntity->addComponent<ComponentSprite>();
   cSprite->getMaterial().DiffuseTexture = cRender->getTexture(Textures.Basketball);
   cSprite->setSize(Vector2(0.4f, 0.4f));
   cScene->addEntity(cEntity);

   for(int i = 0; i < FINGERS; i++)
   {
      cEntity = new Entity(_Info_);
      cEntitiesInfo[i] = cEntity;
      cTransform = cEntity->addComponent<ComponentTransform>();
      cSprite = cEntity->addComponent<ComponentSprite>();
      cSprite->getMaterial().DiffuseTexture = cRender->getTexture(Textures.Info);
      cSprite->setSize(Vector2(0.25f, 0.25f));
      cSprite->setVisible(false);
      cScene->addEntity(cEntity);
   }

   // text
   cEntity = new Entity(_Title_);
   cTransform = cEntity->addComponent<ComponentTransform>();
   cTransform->setPosition(0.0f, 1.15f);
   cLabel = cEntity->addComponent<ComponentUILabel>();
   cLabel->getMaterial().DiffuseColor = Color(1.0f, 0.25f, 0.25f);
   cLabel->getMaterial().DiffuseColor.setOpacity(0.0f);
   cLabel->setFont(0);
   cLabel->setAligment(Alignment::CenterCenter);
   cLabel->setCharacterSize(Vector2(0.16f, 0.16f));
   cLabel->setText("Game Engine");
   cScene->addEntity(cEntity);
}

void GEStateSample::update(float DeltaTime)
{
   updateCube(DeltaTime);
   updateBanana(DeltaTime);
   updateBall(DeltaTime);
   updateText(DeltaTime);

   cScene->render();
}

void GEStateSample::updateText(float fDeltaTime)
{
   Entity* cText = cScene->getEntity(_Title_);

   ComponentRenderable* cRenderable = static_cast<ComponentRenderable*>(cText->getComponent(ComponentType::Renderable));
   Material& sMaterial = cRenderable->getMaterial();

   if(sMaterial.DiffuseColor.getOpacity() < 1.0f)   
      sMaterial.DiffuseColor.setOpacity(sMaterial.DiffuseColor.getOpacity() + 0.005f);
}

void GEStateSample::updateBanana(float fDeltaTime)
{
   float fRotationSpeed = RotationSpeedFactor * fDeltaTime;

   Entity* cBanana = cScene->getEntity(_Bananas_);
   ComponentTransform* cTransform = static_cast<ComponentTransform*>(cBanana->getComponent(ComponentType::Transform));
   cTransform->rotate(-fRotationSpeed, -fRotationSpeed, -fRotationSpeed);
}

void GEStateSample::updateCube(float fDeltaTime)
{
   float fRotationSpeed = RotationSpeedFactor * fDeltaTime;

   Entity* cCube = cScene->getEntity(_Cube_);
   ComponentTransform* cTransform = static_cast<ComponentTransform*>(cCube->getComponent(ComponentType::Transform));
   cTransform->rotate(fRotationSpeed, fRotationSpeed, fRotationSpeed);

   ComponentRenderable* cRenderable = static_cast<ComponentRenderable*>(cCube->getComponent(ComponentType::Renderable));
   cRenderable->getMaterial().DiffuseColor = Color(fMeshCubeR, fMeshCubeG, fMeshCubeB);
   
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

void GEStateSample::updateBall(float fDeltaTime)
{
   Entity* cBall = cScene->getEntity(_Ball_);
   ComponentTransform* cTransform = static_cast<ComponentTransform*>(cBall->getComponent(ComponentType::Transform));

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
      vBallVelocity.Y = (fabs(vBallVelocity.Y) > STOPPED)? -vBallVelocity.Y * BOUNCE: 0.0f;
   }
   
   // move and rotate the ball
   cTransform->move(vBallVelocity);
   cTransform->rotate(0.0f, 0.0f, ((vBallPosition.Y < 0.0f)? -1: 1) * vBallVelocity.X * ROTATION);
   cTransform->rotate(0.0f, 0.0f, ((vBallPosition.X < 0.0f)? 1: -1) * vBallVelocity.Y * ROTATION);
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

void GEStateSample::inputTouchBegin(int ID, const Vector2& Point)
{
   AudioSystem* cAudio = AudioSystem::getInstance();
   cAudio->playSound(Sounds.Touch, 0);

   ComponentTransform* cTransform = static_cast<ComponentTransform*>(cEntitiesInfo[ID]->getComponent(ComponentType::Transform));
   cTransform->setPosition(Point.X, Point.Y);

   ComponentRenderable* cRenderable = static_cast<ComponentRenderable*>(cEntitiesInfo[ID]->getComponent(ComponentType::Renderable));
   cRenderable->show();
}

void GEStateSample::inputTouchMove(int ID, const Vector2& PreviousPoint, const Vector2& CurrentPoint)
{
   if(ID == 0)
   {
      Entity* cCamera = cScene->getEntity(_Camera_);
      ComponentTransform* cTransform = static_cast<ComponentTransform*>(cCamera->getComponent(ComponentType::Transform));

      cTransform->move((CurrentPoint.X - PreviousPoint.X) * TOUCH_SCALE,
                       (CurrentPoint.Y - PreviousPoint.Y) * TOUCH_SCALE,
                       0.0f);
   }

   ComponentTransform* cTransform = static_cast<ComponentTransform*>(cEntitiesInfo[ID]->getComponent(ComponentType::Transform));
   cTransform->setPosition(CurrentPoint.X, CurrentPoint.Y);
}

void GEStateSample::inputTouchEnd(int ID, const Vector2& Point)
{
   ComponentRenderable* cRenderable = static_cast<ComponentRenderable*>(cEntitiesInfo[ID]->getComponent(ComponentType::Renderable));
   cRenderable->hide();
}

void GEStateSample::updateAccelerometerStatus(const Vector3& Status)
{
   vBallVelocity.X += Status.X * ACC_SCALE;
   vBallVelocity.Y += Status.Y * ACC_SCALE;
}
