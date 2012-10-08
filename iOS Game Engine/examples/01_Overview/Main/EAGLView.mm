
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>
#import "EAGLView.h"
#import "sample.h"


// A class extension to declare private methods
@interface EAGLView ()

- (BOOL) createFramebuffer;
- (void) destroyFramebuffer;
- (void) selectScene :(unsigned int) iScene;

@end


@implementation EAGLView

@synthesize animating;
@dynamic animationFrameInterval;

// You must implement this method
+(Class) layerClass 
{
   return [CAEAGLLayer class];
}

//The GL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:
-(id) initWithCoder:(NSCoder*)coder 
{    
   if((self = [super initWithCoder:coder])) 
   {
      // Get the layer
      CAEAGLLayer* eaglLayer = (CAEAGLLayer*)self.layer;
        
      eaglLayer.opaque = YES;
      eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                     [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
        
      context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
        
      if(!context || ![EAGLContext setCurrentContext:context]) 
      {
         [self release];
         return nil;
      }
      
      animating = FALSE;
		displayLinkSupported = FALSE;
		animationFrameInterval = 1;
		displayLink = nil;
		animationTimer = nil;
		
		// A system version of 3.1 or greater is required to use CADisplayLink. The NSTimer
		// class is used as fallback when it isn't available.
		NSString* reqSysVer = @"3.1";
		NSString* currSysVer = [[UIDevice currentDevice] systemVersion];
      
		if([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending)
			displayLinkSupported = TRUE;
  
      cgBounds = [self bounds];
           
      // enable multiple touch
      self.userInteractionEnabled = YES;
      self.multipleTouchEnabled = YES;
      self.exclusiveTouch = YES;
      
#ifdef _USE_ACCELEROMETER_
      // accelerometer
      uiAccel = [UIAccelerometer sharedAccelerometer];
      uiAccel.updateInterval = ACCELEROMETER_UPDATE;
      uiAccel.delegate = self;
#endif
      
      // IDs for touch management
      for(int i = 0; i < MAX_FINGERS; i++)
         iFingerID[i] = 0;
   }
   
   return self;
}

-(void) render
{
   cScenes[iCurrentScene]->update();
   
   // scene change request
   if(cScenes[iCurrentScene]->getNextScene() >= 0)
      [self selectScene :cScenes[iCurrentScene]->getNextScene()];
}

-(void) accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration
{
   cScenes[iCurrentScene]->updateAccelerometerStatus(acceleration.x, acceleration.y, acceleration.z);
}

-(void) layoutSubviews 
{
   [EAGLContext setCurrentContext :context];
   
   [self destroyFramebuffer];
   [self createFramebuffer];
   
   // initialize rendering system
   cRender = new GERendering(context, viewFramebuffer, viewRenderbuffer);
   
   // initialize audio system
   cAudio = new GEAudio();
   
   // create scenes
   cScenes[0] = (GEScene*)new GESceneSample(cRender, cAudio, NULL);
      // ...
      // ...
   
   // select the first scene   
   iCurrentScene = 0;
   cScenes[0]->init();
   
   [self render];
}

-(void) selectScene:(unsigned int)iScene
{
   cScenes[iCurrentScene]->release();
   iCurrentScene = iScene;
   cScenes[iCurrentScene]->init();
}

-(BOOL) createFramebuffer
{    
   glGenFramebuffersOES(1, &viewFramebuffer);
   glGenRenderbuffersOES(1, &viewRenderbuffer);
    
   glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
   glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
   [context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer*)self.layer];
   glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, viewRenderbuffer);
    
   glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
   glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
    
   // depth buffer
   glGenRenderbuffersOES(1, &depthRenderbuffer);
   glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
   glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, backingWidth, backingHeight);
   glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthRenderbuffer);
    
   if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES) 
   {
      NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
      return NO;
   }
	
	glViewport(0, 0, backingWidth, backingHeight);
    
   return YES;
}

-(void) destroyFramebuffer 
{
   glDeleteFramebuffersOES(1, &viewFramebuffer);
   viewFramebuffer = 0;
   glDeleteRenderbuffersOES(1, &viewRenderbuffer);
   viewRenderbuffer = 0;
    
   if(depthRenderbuffer)
   {
      glDeleteRenderbuffersOES(1, &depthRenderbuffer);
      depthRenderbuffer = 0;
   }
}

-(NSInteger) animationFrameInterval
{
	return animationFrameInterval;
}

-(void) setAnimationFrameInterval :(NSInteger)frameInterval
{
	// Frame interval defines how many display frames must pass between each time the
	// display link fires. The display link will only fire 30 times a second when the
	// frame internal is two on a display that refreshes 60 times a second. The default
	// frame interval setting of one will fire 60 times a second when the display refreshes
	// at 60 times a second. A frame interval setting of less than one results in undefined
	// behavior.
	if(frameInterval >= 1)
	{
		animationFrameInterval = frameInterval;
		
		if(animating)
		{
			[self stopAnimation];
			[self startAnimation];
		}
	}
}

-(void) startAnimation
{
	if(!animating)
	{
		if(displayLinkSupported)
		{
			// CADisplayLink is API new to iPhone SDK 3.1. Compiling against earlier versions will result in a warning, but can be dismissed
			// if the system version runtime check for CADisplayLink exists in -initWithCoder:. The runtime check ensures this code will
			// not be called in system versions earlier than 3.1.
			
			displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(render)];
			[displayLink setFrameInterval:animationFrameInterval];
			[displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
		}
		else
			animationTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)((1.0 / 60.0) * animationFrameInterval) target:self selector:@selector(render) userInfo:nil repeats:TRUE];
		
		animating = TRUE;
	}
}

-(void) stopAnimation
{
	if(animating)
	{
		if(displayLinkSupported)
		{
			[displayLink invalidate];
			displayLink = nil;
		}
		else
		{
			[animationTimer invalidate];
			animationTimer = nil;
		}
		
		animating = FALSE;
	}
}

-(void) dealloc
{
   cScenes[iCurrentScene]->release();
   delete cAudio;

   if([EAGLContext currentContext] == context)
      [EAGLContext setCurrentContext:nil];
    
   [context release];
	
   [super dealloc];
}

-(void) touchesBegan :(NSSet*)touches withEvent:(UIEvent*)event
{
   NSArray* uiTouchesList = [touches allObjects];
   
   for(UITouch* uiTouch in uiTouchesList)
   {
      for(int i = 0; i < MAX_FINGERS; i++)
      {
         if(iFingerID[i] == 0)
         {
            CGPoint cgPoint = [uiTouch locationInView: self];
            
            iFingerID[i] = (int)uiTouch;
            cScenes[iCurrentScene]->inputTouchBegin(i, &cgPoint);
            
            break;
         }
      }
   }
}

-(void) touchesMoved :(NSSet*)touches withEvent:(UIEvent*)event
{
   NSArray* uiTouchesList = [touches allObjects];   
   
   for(UITouch* uiTouch in uiTouchesList)
   {
      for(int i = 0; i < MAX_FINGERS; i++)
      {
         if(iFingerID[i] == (int)uiTouch)
         {
            CGPoint cgPreviousPoint = [uiTouch previousLocationInView: self];
            CGPoint cgCurrentPoint = [uiTouch locationInView: self];
         
            cScenes[iCurrentScene]->inputTouchMove(i, &cgPreviousPoint, &cgCurrentPoint);
            
            break;
         }
      }
   }   
}

-(void) touchesEnded :(NSSet*)touches withEvent:(UIEvent*)event
{
   NSArray* uiTouchesList = [touches allObjects];

   for(UITouch* uiTouch in uiTouchesList)
   {
      for(int i = 0; i < MAX_FINGERS; i++)
      {
         if(iFingerID[i] == (int)uiTouch)
         {
            CGPoint cgPoint = [uiTouch locationInView: self];
         
            iFingerID[i] = 0;
            cScenes[iCurrentScene]->inputTouchEnd(i, &cgPoint);
            
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
