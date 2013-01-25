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
#include "WallAsset.h"

#include "BallisticInterpolator.h"
#include "MovementInterpolator.h"

#include "Camera.h"

using namespace std;

#define RUN_GRAPHICS_DISPLAY 0x00;

string filename = "data/ogre.md2";
vector<shared_ptr<GameAsset> > assets;
vector<shared_ptr<GameAsset> > player;

vector<shared_ptr<CubeAsset> > launch;

vector<shared_ptr<GameAsset> > walls;

int cubeSize = 3;

int cubeX = -10;
int cubeY =   0;
int cubeZ =  50;

int tempX = 	0;
int tempY =   	0;

int theWidth = 640;
int theHeight = 480;

int count =0;

int camMode = 0;

string green = "shaders/green.f.glsl";
string red = "shaders/red.f.glsl";
string blue = "shaders/blue.f.glsl";
string white = "shaders/white.f.glsl";
string grey = "shaders/grey.f.glsl";
string yellow = "shaders/yellow.f.glsl";

int cubeShade = 0;

Vector3 noMove(0.0f, 0.0f, 0.0f);
shared_ptr<BallisticInterpolator> still = shared_ptr<BallisticInterpolator>( new BallisticInterpolator(noMove, 50));

Vector3 vShootRight(10.0f, 10.0f, 0.0f);
shared_ptr<BallisticInterpolator> shoot = shared_ptr<BallisticInterpolator>( new BallisticInterpolator(vShootRight, 50));
Vector3 vShootLeft(-10.0f, 10.0f, 0.0f);
Vector3 vShootUp(0.0f, 20.0f, 0.0f);


shared_ptr<CubeAsset> cubeA;// = shared_ptr<CubeAsset> (new CubeAsset(cubeX,cubeY,cubeZ,1, blue));
shared_ptr<CubeAsset> cubeB;// = shared_ptr<CubeAsset> (new CubeAsset(cubeX,cubeY,cubeZ,1, blue));
shared_ptr<CubeAsset> cubeC;// = shared_ptr<CubeAsset> (new CubeAsset(cubeX,cubeY,cubeZ,1, blue));

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
  for(auto wa : walls) {
          wa->update();
        }

  for(auto i: player){
	  for(auto j: walls){
		  if(i->collidesWith(*j)) {
			  cout << "Player>wall collision" << endl;
			  if(cubeX<0){
			  i = shared_ptr<GameAsset>(new CubeAsset(cubeX+5,cubeY,cubeZ,cubeSize,red));
			  }
		  }
	  }
  }

  for(auto i : player) {
    for(auto j : player) {
      if((i != j) && i->collidesWith(*j)) {
	cout << "Asset colliding"  << endl;
	//cubeSize +=0.5 ;
	if(player.size()>1){
	player.pop_back();
	}
	//j=shared_ptr<GameAsset> (new CubeAsset(cubeX,cubeY,cubeZ,cubeSize,red));

	shared_ptr<CubeAsset> cubeA = shared_ptr<CubeAsset> (new CubeAsset(cubeX+3,cubeY,cubeZ,1, yellow));
	shoot = shared_ptr<BallisticInterpolator>( new BallisticInterpolator(vShootRight, 30));
	cubeA->setInterpolator(shoot);

	shared_ptr<CubeAsset> cubeB = shared_ptr<CubeAsset> (new CubeAsset(cubeX+3,cubeY,cubeZ,1, yellow));
	shoot = shared_ptr<BallisticInterpolator>( new BallisticInterpolator(vShootLeft, 30));
	cubeB->setInterpolator(shoot);

	shared_ptr<CubeAsset> cubeC = shared_ptr<CubeAsset> (new CubeAsset(cubeX+3,cubeY,cubeZ,1, yellow));
	shoot = shared_ptr<BallisticInterpolator>( new BallisticInterpolator(vShootUp, 30));
	cubeC->setInterpolator(shoot);

	launch.push_back(cubeA);
	launch.push_back(cubeB);
	launch.push_back(cubeC); //code for cubeC

	if(count < 10){
		if (cubeShade ==0){
	shared_ptr<GameAsset> temp = shared_ptr<GameAsset> (new CubeAsset(tempX,tempY,50,rand()%3+1,green));
	player.push_back(temp);
	cubeShade=1;
		}else if(cubeShade==1){
	shared_ptr<GameAsset> temp = shared_ptr<GameAsset> (new CubeAsset(tempX,tempY,50,rand()%3+1,blue));
	player.push_back(temp);
	cubeShade=0;
		}

	count++;

	}

	cout<<"Your score;"<<endl;
	cout<<count<<endl;

	if (count>=10){
		cout<<"You Win!"<<endl;
		exit(1);
	}

	tempX = rand()%(theWidth/12)-theWidth/20;
	tempY = rand()%(theHeight/12)-theHeight/20;

	//cout << theWidth/16 <<endl; //finding stage size

	//cout << tempX << endl;
	//cout << tempY << endl;



      }
    }
  }

/*
  for(auto i : launch) {
      for(auto j : walls) {
        if((i != j) && i->collidesWith(*j)) { //whys this not working?
        	cout << "They're colliding, ok?"  << endl;
        	//i->setInterpolator(still);
        } else {
        	i->setInterpolator(still);
        }
      }
  }
  */

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
  for(auto wal : walls){
    	wal ->draw();
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

	cout<<"Hit the Target 10 times to win, Goodluck!"<<endl;

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

//store in a single vector

shared_ptr<GameAsset> target = shared_ptr<GameAsset> (new CubeAsset(10,0,50,3, green));
player.push_back(target); //code for target


//shared_ptr<CubeAsset> cubeB = shared_ptr<CubeAsset> (new CubeAsset(cubeX,cubeY,cubeZ,1, blue));
/*
launch.push_back(cubeB); //code for cubeB
*/
shared_ptr<CubeAsset> cubeA = shared_ptr<CubeAsset> (new CubeAsset(cubeX,cubeY,cubeZ,1, red));
shared_ptr<CubeAsset> cubeB = shared_ptr<CubeAsset> (new CubeAsset(cubeX,cubeY,cubeZ,1, red));
shared_ptr<CubeAsset> cubeC = shared_ptr<CubeAsset> (new CubeAsset(cubeX,cubeY,cubeZ,1, red));


shared_ptr<GameAsset> wallR = shared_ptr<GameAsset> (new WallAsset(0,0,50, 30, 25,20,"LR",white));
walls.push_back(wallR); //code for walls
shared_ptr<GameAsset> wallL = shared_ptr<GameAsset> (new WallAsset(0,0,50,-30, 25,20,"LR",white));
walls.push_back(wallL); //code for walls
shared_ptr<GameAsset> wallT = shared_ptr<GameAsset> (new WallAsset(0,0,50, 30, 25,20,"TB",grey));
walls.push_back(wallT); //code for walls
shared_ptr<GameAsset> wallB = shared_ptr<GameAsset> (new WallAsset(0,0,50, 30,-25,20,"TB",grey));
walls.push_back(wallB); //code for walls
/*
shared_ptr<GameAsset> gun = shared_ptr<GameAsset> (new Gun(-7,0,40));
assets.push_back(gun); //code for gun
*/

Vector3 lRight(1.0f, 0.0f, 0.0f);
shared_ptr<BallisticInterpolator> right = shared_ptr<BallisticInterpolator>( new BallisticInterpolator(lRight, 50));

Vector3 lLeft(-1.0f, 0.0f, 0.0f);
shared_ptr<BallisticInterpolator> left = shared_ptr<BallisticInterpolator>( new BallisticInterpolator(lLeft, 50));

Vector3 lUp(0.0f, 1.0f, 0.0f);
shared_ptr<BallisticInterpolator> up = shared_ptr<BallisticInterpolator>( new BallisticInterpolator(lUp, 50));

Vector3 lDown(0.0f, -1.0f, 0.0f);
shared_ptr<BallisticInterpolator> down = shared_ptr<BallisticInterpolator>( new BallisticInterpolator(lDown, 50));

Vector3 moveTest(10.0f,0.0f,0.0f);
shared_ptr<MovementInterpolator> mvTest = shared_ptr<MovementInterpolator>( new MovementInterpolator(moveTest));





//cubeB->setInterpolator(li);
//cubeB->setInterpolator(li);


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
			  //cubeB->setInterpolator(still);
			  break;
			case SDL_KEYDOWN:
			  Matrix4 camera = Camera::getInstance().getCameraM();
// grrrr
			  switch(event.key.keysym.sym){
			  /*case SDLK_f:
				  cubeC = shared_ptr<CubeAsset> (new CubeAsset(cubeX+3,cubeY,cubeZ,1, blue));
				  shoot = shared_ptr<BallisticInterpolator>( new BallisticInterpolator(vShootRight, 50));
				  cubeC->setInterpolator(shoot);
				  launch.push_back(cubeC); //code for cubeB
				  break;*/
			  case SDLK_c:
				  if(camMode==0){
					  camMode=1;
				  } else {
					  camMode=0;
					  Camera::getInstance().setCamera(Matrix4::identity()); //reset camera pos?
					  //display();
				  }
				  break;
			  case SDLK_LEFT:
				  if(camMode==0){
					  cubeX -= 5;
					  player[0]=shared_ptr<GameAsset> (new CubeAsset(cubeX,cubeY,cubeZ,cubeSize,red));
					  //cubeB->setInterpolator(left);
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
				  //player1->setInterpolator(mvTest);
				  //cubeB->setInterpolator(right);
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
				  //cubeB->setInterpolator(up);
				  } else {

			    Camera::getInstance().setCamera(camera * Matrix4::translation(Vector3(0.0, 0.0, -2.0)) );

				  }
			    break;
			  case SDLK_DOWN:
				  if(camMode==0){
				  cubeY -= 5;
				  player[0]=shared_ptr<GameAsset> (new CubeAsset(cubeX,cubeY,cubeZ,cubeSize,red));
				  //cubeB->setInterpolator(down);
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
