
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  iOS
//
//  --- ViewController.mm ---
//
//////////////////////////////////////////////////////////////////


#import "ViewController.h"

#import "GERenderSystemES20.h"
#import "GEAudioSystemOpenAL.h"
#import "GEState.h"
#import "GETimer.h"
#import "GEDevice.h"

#import "config.h"

#import "StateSample.h"


using namespace GE;
using namespace GE::Core;
using namespace GE::Rendering;
using namespace GE::Audio;
using namespace GE::States;


@interface ViewController () <UIAccelerometerDelegate>
{
   // Render system
   RenderSystemES20* cRender;
   
   // Audio system
   AudioSystemOpenAL* cAudio;
   unsigned int iAudioUpdateFrame;
   
   // State management
   std::vector<State*> cStates;
   int iCurrentState;
   
   // Input management
   UITouch* iFingerID[GE_MAX_FINGERS];
   UIAccelerometer* uiAccel;
   
   Line* cPixelToScreenX;
   Line* cPixelToScreenY;
   
   // Timer
   Timer cTimer;
   double dTime;
}

@property (strong, nonatomic) EAGLContext* context;
@property (strong, nonatomic) GLKBaseEffect* effect;

-(void) accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration;
-(void) selectState:(unsigned int)state;
-(Vector2) pixelToScreen:(const GE::Vector2&)pixelPosition;

@end


@implementation ViewController

@synthesize context = _context;
@synthesize effect = _effect;

-(BOOL) prefersStatusBarHidden
{
   return YES;
}

-(void) dealloc
{
   [_context release];
   [_effect release];
   [super dealloc];
}

-(void) viewDidLoad
{
   [super viewDidLoad];

   // initialize OpenGL
   self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

   if(!self.context) 
       NSLog(@"Failed to create ES context");
    
   GLKView* view = (GLKView*)self.view;
   view.context = self.context;
   view.drawableDepthFormat = GLKViewDrawableDepthFormat24;

   [EAGLContext setCurrentContext:self.context];
   
   // set frames per second
   self.preferredFramesPerSecond = GE_FPS;
   
   // enable multiple touch
   self.view.userInteractionEnabled = YES;
   self.view.multipleTouchEnabled = YES;
   self.view.exclusiveTouch = YES;
   
   // IDs for touch management
   for(int i = 0; i < GE_MAX_FINGERS; i++)
      iFingerID[i] = 0;
   
   // device orientation
#ifdef GE_ORIENTATION_PORTRAIT
   Device::Orientation = DeviceOrientation::Portrait;
#else
   Device::Orientation = DeviceOrientation::Landscape;
#endif
   
   // pixel space to screen space
   if(Device::Orientation == DeviceOrientation::Portrait)
   {
      cPixelToScreenX = new Line(0.0f, -1.0f, Device::getTouchPadWidth(), 1.0f);
      cPixelToScreenY = new Line(0.0f, Device::getAspectRatio(), Device::getTouchPadHeight(), -Device::getAspectRatio());
   }
   else
   {
      cPixelToScreenX = new Line(0.0f, -1.0f, Device::getTouchPadHeight(), 1.0f);
      cPixelToScreenY = new Line(0.0f, Device::getAspectRatio(), Device::getTouchPadWidth(), -Device::getAspectRatio());
   }
   
#ifdef GE_USE_ACCELEROMETER
   // accelerometer
   uiAccel = [UIAccelerometer sharedAccelerometer];
   uiAccel.updateInterval = GE_ACCELEROMETER_UPDATE;
   uiAccel.delegate = self;
#endif
   
   // initialize rendering system
   cRender = new RenderSystemES20();
   cRender->setBackgroundColor(Color(0.5f, 0.5f, 1.0f));
   
   // initialize audio system
   cAudio = new AudioSystemOpenAL();
   cAudio->init();
   iAudioUpdateFrame = 0;
   
   // create states
   cStates.push_back(new GEStateSample(0));
   // ...
   // ...
   
   // start the timer
   cTimer.start();
   dTime = 0.0;
   
   // select the first state   
   iCurrentState = 0;
   cStates[0]->init();
}

-(void) viewDidUnload
{    
   [super viewDidUnload];
   
   [EAGLContext setCurrentContext:self.context];
    
   if([EAGLContext currentContext] == self.context) 
      [EAGLContext setCurrentContext:nil];

   self.context = nil;
   
   delete cPixelToScreenX;
   delete cPixelToScreenY;
   
   while(!cStates.empty())
   {
      delete cStates.back();
      cStates.pop_back();
   }
   
   // release rendering system
   delete cRender;
   
   // release audio system
   cAudio->release();
   delete cAudio;
}

-(void) didReceiveMemoryWarning
{
   [super didReceiveMemoryWarning];
}

-(BOOL) shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
#ifdef GE_ORIENTATION_PORTRAIT
   if(interfaceOrientation == UIInterfaceOrientationPortrait)
      return YES;
   if(interfaceOrientation == UIInterfaceOrientationPortraitUpsideDown)
      return YES;
#endif
#ifdef GE_ORIENTATION_LANDSCAPE
   if(interfaceOrientation == UIInterfaceOrientationLandscapeRight)
      return YES;
   if(interfaceOrientation == UIInterfaceOrientationLandscapeLeft)
      return YES;
#endif   
   
   return NO;
}

-(void) accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration
{
   cStates[iCurrentState]->updateAccelerometerStatus(Vector3(acceleration.x, acceleration.y, acceleration.z));
}

-(void) selectState:(unsigned int)state
{
   cStates[iCurrentState]->release();
   iCurrentState = state;
   cStates[iCurrentState]->init();
}

-(void) update
{
   // delta time
   double dCurrentTime = cTimer.getTime();
   float fDeltaTime = (dCurrentTime - dTime) * 0.001f;
   dTime = dCurrentTime;
   
   // state update
   cStates[iCurrentState]->update(fDeltaTime);
   
   // audio system update
   iAudioUpdateFrame++;
   
   if(iAudioUpdateFrame == GE_AUDIO_UPDATE_FRAMES)
   {
      iAudioUpdateFrame = 0;
      cAudio->update();
   }
    
   // state change request
   if(cStates[iCurrentState]->getNextState() >= 0)
      [self selectState:cStates[iCurrentState]->getNextState()];
}

-(void) glkView:(GLKView *)view drawInRect:(CGRect)rect
{
   cRender->renderBegin();
   cRender->renderFrame();
   [self.context presentRenderbuffer:GL_RENDERBUFFER];
   cRender->clearRenderingQueues();
}

-(Vector2) pixelToScreen:(const GE::Vector2&)pixelPosition
{
   return Vector2((float)cPixelToScreenX->y(pixelPosition.X), (float)cPixelToScreenY->y(pixelPosition.Y));
}

-(void) touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
   NSArray* uiTouchesList = [touches allObjects];
   
   for(UITouch* uiTouch in uiTouchesList)
   {
      for(int i = 0; i < GE_MAX_FINGERS; i++)
      {
         if(iFingerID[i] == 0)
         {
            CGPoint cgPoint = [uiTouch locationInView: self.view];
            
            iFingerID[i] = uiTouch;
            cStates[iCurrentState]->inputTouchBegin(i, [self pixelToScreen: Vector2(cgPoint.x, cgPoint.y)]);
            
            break;
         }
      }
   }
}

-(void) touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
   NSArray* uiTouchesList = [touches allObjects];   
   
   for(UITouch* uiTouch in uiTouchesList)
   {
      for(int i = 0; i < GE_MAX_FINGERS; i++)
      {
         if(iFingerID[i] == uiTouch)
         {
            CGPoint cgPreviousPoint = [uiTouch previousLocationInView: self.view];
            CGPoint cgCurrentPoint = [uiTouch locationInView: self.view];
            
            cStates[iCurrentState]->inputTouchMove(i,
                                                   [self pixelToScreen: Vector2(cgPreviousPoint.x, cgPreviousPoint.y)],
                                                   [self pixelToScreen: Vector2(cgCurrentPoint.x, cgCurrentPoint.y)]);            
            break;
         }
      }
   }   
}

-(void) touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
   NSArray* uiTouchesList = [touches allObjects];
   
   for(UITouch* uiTouch in uiTouchesList)
   {
      for(int i = 0; i < GE_MAX_FINGERS; i++)
      {
         if(iFingerID[i] == uiTouch)
         {
            CGPoint cgPoint = [uiTouch locationInView: self.view];
            
            iFingerID[i] = 0;
            cStates[iCurrentState]->inputTouchEnd(i, [self pixelToScreen: Vector2(cgPoint.x, cgPoint.y)]);
            
            break;
         }
      }
   }
}

-(void) touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event
{
   [self touchesEnded:touches withEvent:event];
}

@end
