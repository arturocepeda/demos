
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
   State* cStates[NUM_STATES];
   int iCurrentState;
   
   // Input management
   int iFingerID[MAX_FINGERS];
   UIAccelerometer* uiAccel;
   
   // Timer
   Timer cTimer;
   double dTime;
}

@property (strong, nonatomic) EAGLContext* context;
@property (strong, nonatomic) GLKBaseEffect* effect;

-(void) accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration;
-(void) selectState:(unsigned int) iState;

@end


@implementation ViewController

@synthesize context = _context;
@synthesize effect = _effect;

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
   self.preferredFramesPerSecond = FPS;
   
   // enable multiple touch
   self.view.userInteractionEnabled = YES;
   self.view.multipleTouchEnabled = YES;
   self.view.exclusiveTouch = YES;
   
   // IDs for touch management
   for(int i = 0; i < MAX_FINGERS; i++)
      iFingerID[i] = 0;
   
#ifdef USE_ACCELEROMETER
   // accelerometer
   uiAccel = [UIAccelerometer sharedAccelerometer];
   uiAccel.updateInterval = ACCELEROMETER_UPDATE;
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
   cStates[0] = (State*)new GEStateSample(cRender, cAudio, NULL);
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
#ifdef PORTRAIT_UP
   if(interfaceOrientation == UIInterfaceOrientationPortrait)
      return YES;
#endif
#ifdef PORTRAIT_DOWN
   if(interfaceOrientation == UIInterfaceOrientationPortraitUpsideDown)
      return YES;
#endif
#ifdef LANDSCAPE_HOME_LEFT
   if(interfaceOrientation == UIInterfaceOrientationLandscapeRight)
      return YES;
#endif
#ifdef LANDSCAPE_HOME_RIGHT
   if(interfaceOrientation == UIInterfaceOrientationLandscapeLeft)
      return YES;
#endif   
   
   return NO;
}

-(void) accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration
{
   cStates[iCurrentState]->updateAccelerometerStatus(Vector3(acceleration.x, acceleration.y, acceleration.z));
}

-(void) selectState:(unsigned int)iState
{
   cStates[iCurrentState]->release();
   iCurrentState = iState;
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
   cStates[iCurrentState]->render();
   [self.context presentRenderbuffer:GL_RENDERBUFFER];
}

-(void) touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
   NSArray* uiTouchesList = [touches allObjects];
   
   for(UITouch* uiTouch in uiTouchesList)
   {
      for(int i = 0; i < MAX_FINGERS; i++)
      {
         if(iFingerID[i] == 0)
         {
            CGPoint cgPoint = [uiTouch locationInView: self.view];
            
            iFingerID[i] = (int)uiTouch;
            cStates[iCurrentState]->inputTouchBegin(i, Vector2(cgPoint.x, cgPoint.y));
            
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
      for(int i = 0; i < MAX_FINGERS; i++)
      {
         if(iFingerID[i] == (int)uiTouch)
         {
            CGPoint cgPreviousPoint = [uiTouch previousLocationInView: self.view];
            CGPoint cgCurrentPoint = [uiTouch locationInView: self.view];
            
            cStates[iCurrentState]->inputTouchMove(i,
                                                   Vector2(cgPreviousPoint.x, cgPreviousPoint.y),
                                                   Vector2(cgCurrentPoint.x, cgCurrentPoint.y));            
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
      for(int i = 0; i < MAX_FINGERS; i++)
      {
         if(iFingerID[i] == (int)uiTouch)
         {
            CGPoint cgPoint = [uiTouch locationInView: self.view];
            
            iFingerID[i] = 0;
            cStates[iCurrentState]->inputTouchEnd(i, Vector2(cgPoint.x, cgPoint.y));
            
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
