
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
#include "Core/GEConstants.h"

using namespace GE;
using namespace GE::States;
using namespace GE::Rendering;
using namespace GE::Audio;

GEStateSample::GEStateSample(RenderSystem* Render, AudioSystem* Audio, void* GlobalData)
   : State(Render, Audio, GlobalData)
{
}

void GEStateSample::internalInit()
{
   cRender->setBackgroundColor(Color(0.1f, 0.1f, 0.2f));
   
   // lighting
   cRender->setAmbientLightColor(Color(1.0f, 1.0f, 1.0f));
   cRender->setAmbientLightIntensity(0.4f);

   cRender->setNumberOfActiveLights(1);
   cRender->setLightPosition((unsigned int)Lights::PointLight1, Vector3(0.0f, 0.0f, 1.0f));
   cRender->setLightColor((unsigned int)Lights::PointLight1, Color(1.0f, 1.0f, 1.0f));
   cRender->setLightIntensity((unsigned int)Lights::PointLight1, 0.6f);

   // cameras
   cCamera = new Camera();
   cCamera->setPosition(0.0f, 0.0f, -4.0f);

   // textures
   cRender->loadTexture(Textures.Banana, "banana", "jpg");
   cRender->loadTexture(Textures.Bulb, "bulb", "png");
   
   // mesh
   cRender->createMesh(&cMeshBanana);
   cMeshBanana->loadFromFile("banana");
   cMeshBanana->setTexture(cRender->getTexture(Textures.Banana));
   cMeshBanana->scale(2.5f, 2.5f, 2.5f);

   cRender->createMesh(&cMeshLight);
   cMeshLight->loadFromFile("cube");
   cMeshLight->setTexture(cRender->getTexture(Textures.Bulb));
   cMeshLight->setPosition(Vector3(0.0f, 0.0f, 1.0f));
   cMeshLight->setScale(Vector3(0.2f, 0.2f, 0.2f));
   cMeshLight->setColor(Color(1.0f, 1.0f, 0.0f));
   
   // sprite
   cRender->createSprite(&cSpriteBulb);
   cSpriteBulb->setTexture(cRender->getTexture(Textures.Bulb));
   cSpriteBulb->setSize(Vector2(0.5f, 0.5f));
   cSpriteBulb->setOpacity(0.75f);
   
   vBulbPosition.set(0.0f, 0.0f, 0.0f);
   bMoveBulb = false;
}

void GEStateSample::update(float DeltaTime)
{
   cMeshBanana->rotate(DeltaTime * 0.0001f, DeltaTime * 0.001f, DeltaTime * 0.0005f);
}

void GEStateSample::render()
{
   // camera
   cRender->set3D();
   cRender->useCamera(cCamera);

   // mesh
   cRender->useShaderProgram((unsigned int)ShaderPrograms::MeshTexture);
   cRender->renderMesh(cMeshBanana);
   cRender->useShaderProgram((unsigned int)ShaderPrograms::MeshColor);
   cRender->renderMesh(cMeshLight);
   
   // sprite
   cRender->set2D();
   cRender->useShaderProgram((unsigned int)ShaderPrograms::HUD);
   cRender->renderSprite(cSpriteBulb);
}

void GEStateSample::release()
{
   // release objects
   delete cCamera;
   delete cMeshBanana;
   delete cSpriteBulb;
}

void GEStateSample::inputTouchBegin(int ID, const Vector2& Point)
{
   if(ID > 0)
      return;
   
   Vector2 vDelta = Point;
   vDelta.X -= vBulbPosition.X;
   vDelta.X *= vDelta.X;
   vDelta.Y -= vBulbPosition.Y;
   vDelta.Y *= vDelta.Y;
   
   if((vDelta.X + vDelta.Y) < FINGER_NEAR)
      bMoveBulb = true;
}

void GEStateSample::inputTouchMove(int ID, const Vector2& PreviousPoint, const Vector2& CurrentPoint)
{
   if(!bMoveBulb || ID > 0)
      return;
   
   cRender->setLightPosition((unsigned int)Lights::PointLight1, Vector3(CurrentPoint.X, CurrentPoint.Y, 1.0f));
   cMeshLight->setPosition(Vector3(CurrentPoint.X, CurrentPoint.Y, 1.0f));
   vBulbPosition.set(CurrentPoint.X, CurrentPoint.Y, 0.0f);
   cSpriteBulb->setPosition(vBulbPosition);
}

void GEStateSample::inputTouchEnd(int ID, const Vector2& Point)
{
   if(ID == 0)
      bMoveBulb = false;
}
