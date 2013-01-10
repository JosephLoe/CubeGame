#define GLEW_STATIC // Easier debugging
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL/SDL.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <string>

#include "GameAsset.h"
#include "Md2Asset.h"


#include "TriangularPyramidAsset.h"
#include "CubeAsset.h"
#include "Gun.h"

#include "BallisticInterpolator.h"
#include "Camera.h"

using namespace std;

#define RUN_GRAPHICS_DISPLAY 0x00;

string filename = "data/ogre.md2";
vector<shared_ptr<GameAsset> > assets;
vector<shared_ptr<GameAsset> > player;
vector<shared_ptr<GameAsset> > launch;
int cubeSize = 3;

int cubeX = -10;
int cubeY =   0;
int cubeZ =  50;

int tempX = 	0;
int tempY =   	0;

int theWidth = 640;
int theHeight = 480;

int count =0;

string green = "shaders/green.f.glsl";
string red = "shaders/red.f.glsl";
string blue = "shaders/blue.f.glsl";

int cubeShade = 0;

/*
 * SDL timers run in separate threads.  In the timer thread
 * push an event onto the event queue.  This event signifies
 * to call display() from the thread in which the OpenGL 
 * context was created.
 */
Uint32 display(Uint32 interval, void *param) {
	SDL_Event event;
	event.type = SDL_USEREVENT;
	event.user.code = RUN_GRAPHICS_DISPLAY;
	event.user.data1 = 0;
	event.user.data2 = 0;
	SDL_PushEvent(&event);
	return interval;
}

void display() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  // This O(n + n^2 + n) sequence of loops is written for clarity,
  // not efficiency
  for(auto it : assets) {
    it->update();
  }
  for(auto plz : player) {
      plz->update();
    }
  for(auto la : launch) {
        la->update();
      }

  for(auto i : assets) {
    for(auto j : player) {
      if((i != j) && i->collidesWith(*j)) {
	cout << "We have a collision"  << endl;
	cubeSize +=1 ;
	assets.pop_back();
	j=shared_ptr<GameAsset> (new CubeAsset(cubeX,cubeY,cubeZ,cubeSize,red));

	if(count < 10){
		if (cubeShade ==0){
	shared_ptr<GameAsset> temp = shared_ptr<GameAsset> (new CubeAsset(tempX,tempY,50,rand()%3+1,green));
	assets.push_back(temp);
	cubeShade=1;
		}else{
	shared_ptr<GameAsset> temp = shared_ptr<GameAsset> (new CubeAsset(tempX,tempY,50,rand()%3+1,blue));
	assets.push_back(temp);
	cubeShade=0;
		}

	count++;

	}

	tempX = rand()%(theWidth/8)-theWidth/16;
	tempY = rand()%(theHeight/8)-theHeight/16;

	//cout << theWidth/16 <<endl; //finding stage size

	cout << tempX << endl;
	cout << tempY << endl;


      }
    }
  }

  for(auto it : assets) {
	//  glColorMask(1.0f,0.0f,0.0f,1.0f); // CHANGE COLOR OF BOX!
	  //glStencilMask(0xf0);  //not sure wtf this does.
    it->draw();
  }
  for(auto pl : player){
	pl ->draw();
	//pl->update();
  }
  for(auto lau : launch){
  	lau ->draw();
    }


  // Don't forget to swap the buffers
  SDL_GL_SwapBuffers();
}

int main(int argc, char ** argv) {
	SDL_Surface * surf;
	Uint32 width = theWidth;
	Uint32 height = theHeight;
	Uint32 colour_depth = 16; // in bits
	Uint32 delay = 1000/60; // in milliseconds

	// Initialise SDL - when using C/C++ it's common to have to
	// initialise libraries by calling a function within them.
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER)<0) {
			cout << "Failed to initialise SDL: " << SDL_GetError() << endl;
			SDL_Quit();
	}

	// When we close a window quit the SDL application
	atexit(SDL_Quit);

	// Create a new window with an OpenGL surface
	if (!(surf = SDL_SetVideoMode(width, height, colour_depth, SDL_OPENGL))) {
			cout << "Failed to initialise video mode: " << SDL_GetError() << endl;
			SDL_Quit();
	}

	// Initialise GLEW - an easy way to ensure OpenGl 2.0+
	// The *must* be done after we have set the video mode - otherwise we have no OpenGL context.
	glewInit();
	if (!glewIsSupported("GL_VERSION_2_0")) {
	  cerr<< "OpenGL 2.0 not available" << endl;
	  return 1;
	}

    // initialise depth testing
	glEnable (GL_DEPTH_TEST);

	srand(time(0));
/*
for(int yay=0; yay<10; yay++){
	for(int temp=0; temp<10; temp++){
		shared_ptr<GameAsset> a = shared_ptr<GameAsset> (new CubeAsset(yay*2, temp*2,  20));
		assets.push_back(a); //add cube
	}
}  // loops to make platform
*/

/*
shared_ptr<GameAsset> z = shared_ptr<GameAsset> (new TriangularPyramidAsset(246,229,50));
assets.push_back(z); //code for triangle
*/




shared_ptr<GameAsset> player1 = shared_ptr<GameAsset> (new CubeAsset(cubeX,cubeY,cubeZ, cubeSize, red));
player.push_back(player1); //code for player


shared_ptr<GameAsset> cubeA = shared_ptr<GameAsset> (new CubeAsset(10,0,50,3, green));
assets.push_back(cubeA); //code for cubeA

shared_ptr<CubeAsset> cubeB = shared_ptr<CubeAsset> (new CubeAsset(0,0,50,3, blue));
launch.push_back(cubeB); //code for cubeA

/*
shared_ptr<GameAsset> gun = shared_ptr<GameAsset> (new Gun(-7,0,40));
assets.push_back(gun); //code for gun
*/

Vector3 launch(10.0f, 10.0f, 0.0f);
shared_ptr<BallisticInterpolator> li = shared_ptr<BallisticInterpolator>( new BallisticInterpolator(launch, 50));
cubeB->setInterpolator(li);
//cubeB->setInterpolator(li);

int camMode = 0;

	// Call the function "display" every delay milliseconds
	SDL_AddTimer(delay, display, NULL);

	// Add the main event loop
	SDL_Event event;
	while (SDL_WaitEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
			  SDL_Quit();
			  break;
			case SDL_USEREVENT:
				display();
				break;
			case SDL_KEYUP:
			  Camera::getInstance().setCamera(Matrix4::identity());

			  break;
			case SDL_KEYDOWN:
			  Matrix4 camera = Camera::getInstance().getCameraM();
// grrrr
			  switch(event.key.keysym.sym){
			  case SDLK_f:
				 // player[0]->update();
				  break;
			  case SDLK_c:
				  if(camMode==0){
					  camMode=1;
				  } else {
					  camMode=0;
				  }
				  break;
			  case SDLK_LEFT:
				  if(camMode==0){
				  cubeX -= 5;
				  //cubeSize += 1;
				  player[0]=shared_ptr<GameAsset> (new CubeAsset(cubeX,cubeY,cubeZ,cubeSize,red));
				  }
				  if(camMode==1){
			    //Camera::getInstance().setCamera((camera * Matrix4::rotationY(5.0/180.0)));
				Camera::getInstance().setCamera(camera * Matrix4::translation(Vector3(1.0, 0.0, 0.0)) );
				  }
				 break;
			  case SDLK_RIGHT:

				  if(camMode==0){
				  cubeX += 5;
				  player[0]=shared_ptr<GameAsset> (new CubeAsset(cubeX,cubeY,cubeZ,cubeSize,red));

				  }
				  /*
				  for(auto pl2 : player){
				  	pl2 ->draw();
				  	pl2->update();
				    }
				    */

				  //TODO try and get the camera to rotate and move at same time!
				  if(camMode==1){
				  Camera::getInstance().setCamera(camera * Matrix4::translation(Vector3(-1.0, 0.0, 0.0)) );
				  }
			    break;
			  case SDLK_UP:

				  if(camMode==0){
				  cubeY += 5;
				  player[0]=shared_ptr<GameAsset> (new CubeAsset(cubeX,cubeY,cubeZ,cubeSize,red));
				  } else {

			    Camera::getInstance().setCamera(camera * Matrix4::translation(Vector3(0.0, 0.0, -2.0)) );

				  }
			    break;
			  case SDLK_DOWN:
				  if(camMode==0){
				  cubeY -= 5;
				  player[0]=shared_ptr<GameAsset> (new CubeAsset(cubeX,cubeY,cubeZ,cubeSize,red));
				  } else {
			    Camera::getInstance().setCamera(camera * Matrix4::translation(Vector3(0.0, 0.0, 2.0)) );
				  }
			    break;
			  default:
			    break;
			  }
			  break;
			}
	}
}
