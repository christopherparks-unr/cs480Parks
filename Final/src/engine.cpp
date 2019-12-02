
#include "engine.h"
#include "physics.h"
#include <cmath>


float clamp(float n, float lower, float upper) {
  return std::max(lower, std::min(n, upper));
}

Engine::Engine(string name, int width, int height)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  m_FULLSCREEN = false;
  keystate_ctrl = false;
  keystate_r = false;
  killswitch = false;
  PhysStruct.init();

  camera_theta = 3.141592/2.0;
  camera_phi = 0.0;
  player_movement = 16.0;
  mouse_x = 0.0; mouse_y = 0.0;
dynMaxSize = 16;
dynCurObjNum = 0;
for(int a = 0; a < dynMaxSize; a++)
{
dynamicObjects.push_back(nullptr);
}
}

Engine::Engine(string name)
{
  m_WINDOW_NAME = name;
  m_WINDOW_HEIGHT = 0;
  m_WINDOW_WIDTH = 0;
  m_FULLSCREEN = true;
}

Engine::~Engine()
{
  delete m_window;
  delete m_graphics;
  m_window = NULL;
  m_graphics = NULL;
}

bool Engine::Initialize(std::string v, std::string f)
{
  // Start a window
  selection = nullptr;
  m_window = new Window();

SDL_CaptureMouse(SDL_TRUE);
SDL_ShowCursor(SDL_DISABLE);
SDL_SetWindowGrab(m_window->gWindow, SDL_TRUE);
SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);
//SDL_SetRelativeMouseMode(SDL_TRUE);

  if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT))
  {
    printf("The window failed to initialize.\n");
    return false;
  }

  // Start the graphics, now with path support for shaders
  m_graphics = new Graphics();
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, v, f, &PhysStruct))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();

  /*
  for (int iter = 0; iter < 27; iter++) {
    char make[64];
    sprintf(make,"Map%i",iter);
    PhysStruct.add_object(make,m_graphics);
  }
	*/

  return true;
}

bool Engine::Run()
{
  m_running = true;
  //int mouse_rec_x,mouse_rec_y;
  //int mouse_x = 0,mouse_y = 0;

  btVector3 velocity_player;
  const unsigned char* state = (const unsigned char*) SDL_GetKeyboardState(NULL);


  while(m_running && !killswitch)
  {

    //std::cout << "One" << std::endl;
    // Update the DT
    m_DT = getDT();
    // Check the keyboard input
    while(SDL_PollEvent(&m_event) != 0)
    {
      killswitch = Keyboard();
    }

    // check for player keydowns

    velocity_player = btRigidBody::upcast(PhysStruct.dw->getCollisionObjectArray()[0])->getLinearVelocity();
    velocity_player.setX(0.0);
    velocity_player.setZ(0.0);

    if (state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_S]) {
        velocity_player.setX(player_movement*cos(camera_phi));
        velocity_player.setZ(player_movement*sin(camera_phi));
    } else if (!state[SDL_SCANCODE_W] && state[SDL_SCANCODE_S]) {
        velocity_player.setX(-player_movement*cos(camera_phi));
        velocity_player.setZ(-player_movement*sin(camera_phi));
    }

    if (state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_D]) {
        velocity_player.setX(player_movement*sin(camera_phi));
        velocity_player.setZ(-player_movement*cos(camera_phi));
    } else if (!state[SDL_SCANCODE_A] && state[SDL_SCANCODE_D]) {
        velocity_player.setX(-player_movement*sin(camera_phi));
        velocity_player.setZ(player_movement*cos(camera_phi));
    } 

    btRigidBody::upcast(PhysStruct.dw->getCollisionObjectArray()[0])->setLinearVelocity(velocity_player);

    PhysStruct.dw->getCollisionObjectArray()[0]->activate();
    //std::cout << "Half" << std::endl;

    // WOW
    PhysStruct.dw->stepSimulation(m_DT,10);

    //for (int iter = PhysStruct.dw->getNumCollisionObjects() - 2 - PhysStruct.dynObjectNum; iter >= 0; iter--)
	for (int iter = PhysStruct.dw->getNumCollisionObjects() - 1 - PhysStruct.dynObjectNum; iter >= 0; iter--)
    {
	//std::cout << "Stepping through static object " << iter << std::endl;
	btCollisionObject* obj = PhysStruct.dw->getCollisionObjectArray()[iter];
	btRigidBody* body = btRigidBody::upcast(obj);
	btTransform trans;
	btScalar m[16];
	if (body && body->getMotionState()) {
		body->getMotionState()->getWorldTransform(trans);
	} else {
		trans = obj->getWorldTransform();
	}
	trans.getOpenGLMatrix(m);
	PhysStruct.vector_of_objects[iter]->SetModel(m);
    }
/*
	std::cout << PhysStruct.dw->getNumCollisionObjects() << " " << 15 - PhysStruct.dynObjectNum << std::endl;
	int a = 15;
for (int iter = PhysStruct.dw->getNumCollisionObjects() - 1; iter > PhysStruct.dw->getNumCollisionObjects() - 15 + PhysStruct.dynObjectNum - 1; iter--)
    {
	cout << "iter = " << iter << std::endl;
	if(PhysStruct.DynObjects[a] != nullptr)
	{
		std::cout << "Stepping through dynamic object " << a << std::endl;
		btCollisionObject* obj = PhysStruct.dw->getCollisionObjectArray()[iter];
		btRigidBody* body = btRigidBody::upcast(obj);
		btTransform trans;
		btScalar m[16];
		if (body && body->getMotionState()) {
			body->getMotionState()->getWorldTransform(trans);
		} else {
			trans = obj->getWorldTransform();
		}
		trans.getOpenGLMatrix(m);
		glm::mat4 with;
		for (int f = 0; f < 16; f++) {
    			with[f/4][f%4] = m[f];
		}
		std::cout << "Setting model" << std::endl;
		PhysStruct.DynObjects[a]->SetModel(with);
		std::cout << "Okay, we should be fine" << std::endl;
	}
	a--;
    }
*/
if(PhysStruct.dynObjectNum == 1)
{
	int iter = PhysStruct.dw->getNumCollisionObjects() - 1;
		btCollisionObject* obj = PhysStruct.dw->getCollisionObjectArray()[iter];
		btRigidBody* body = btRigidBody::upcast(obj);
		btTransform trans;
		btScalar m[16];
		if (body && body->getMotionState()) {
			body->getMotionState()->getWorldTransform(trans);
		} else {
			trans = obj->getWorldTransform();
		}
		trans.getOpenGLMatrix(m);
		/*
		glm::mat4 with;
		for (int f = 0; f < 16; f++) {
    			with[f/4][f%4] = m[f];
		}
		*/
		PhysStruct.DynObject->SetModel(m);
   }

    //camera location
    btVector3 get = PhysStruct.dw->getCollisionObjectArray()[0]->getWorldTransform().getOrigin();
    glm::vec3 loc(get.getX()+0.0,get.getY()+10.0,get.getZ());
    m_graphics->m_camera->Reposition(loc,camera_theta,camera_phi, m_WINDOW_WIDTH, m_WINDOW_HEIGHT);
    //std::cout << "Two" << std::endl;

    // Update and render the graphics
    m_graphics->Update(m_DT);
if(PhysStruct.dynObjectNum == 1)
{
	m_graphics->Render(&PhysStruct);
}
else
{
    m_graphics->Render();
}

    // Swap to the Window
    m_window->Swap();
    //SDL_WarpMouseInWindow(m_window->gWindow,m_WINDOW_WIDTH/2,m_WINDOW_HEIGHT/2);
  }
	
  return killswitch;
}

bool Engine::Keyboard()
{
  if(m_event.type == SDL_QUIT)
  {
    m_running = false;
  }
  else if (m_event.type == SDL_KEYDOWN)
  {
    // handle key down events here
    switch(m_event.key.keysym.sym)
    {
      case SDLK_0:  selection = nullptr; break;
      case SDLK_LCTRL:
      case SDLK_RCTRL: keystate_ctrl = true; break;
      case SDLK_r: keystate_r = true; break;

      case SDLK_f: m_graphics->shade_use = !(m_graphics->shade_use); break;

      case SDLK_x: m_graphics->ambient_lighting = max(0.0, m_graphics->ambient_lighting - 0.1); break;
      case SDLK_z: m_graphics->ambient_lighting = min(1.0, m_graphics->ambient_lighting + 0.1); break;
      case SDLK_h: m_graphics->diffuse_lighting = max(0.1, m_graphics->diffuse_lighting - 0.2); break;
      case SDLK_g: m_graphics->diffuse_lighting = min(1.5, m_graphics->diffuse_lighting + 0.2); break;
      case SDLK_v: m_graphics->specular_lighting = max(0.1, m_graphics->specular_lighting - 0.2); break;
      case SDLK_c: m_graphics->specular_lighting = min(1.5, m_graphics->specular_lighting + 0.2); break;

      case SDLK_i: m_graphics->spotlight_angle = max(0.1, m_graphics->spotlight_angle - 0.1); break;
      case SDLK_o: m_graphics->spotlight_angle = min(1.5, m_graphics->spotlight_angle + 0.1); break;
      case SDLK_ESCAPE: exit(0); break;

    }
  }
  else if(m_event.type == SDL_KEYUP)
  {
    switch(m_event.key.keysym.sym)
    {
      case SDLK_LCTRL:
      case SDLK_RCTRL: keystate_ctrl = false; break;
      case SDLK_r: keystate_r = false; break;
    }
  }
  else if (m_event.type == SDL_MOUSEBUTTONDOWN)
  {
    // handle mouse down events here
    if (m_event.button.button == SDL_BUTTON_LEFT && PhysStruct.dynObjectNum == 0)
    {
      //Add dynamic object

btVector3 get = PhysStruct.dw->getCollisionObjectArray()[0]->getWorldTransform().getOrigin();
	Object * temp = new Object("../assets/scenes/sphere.obj",
		0,
                "../assets/scenes/checker.jpg",
		get.getX(),
		get.getY() + 10.0,
		get.getZ(),
		0.5,
		0.9,
		1200);
	//std::cout << "Adding obj to dynObj" << std::endl;
		PhysStruct.DynObject = temp;
//std::cout << "Adding sphere to physstruct" << std::endl;
		PhysStruct.add_dynamic(PhysStruct.DynObject, float(4.0));
		dynCurObjNum = 1;
	
//std::cout << "Calculating impulse" << std::endl;
float xdir = sin(camera_theta) * cos(camera_phi);
float ydir = sin(camera_theta) * sin(camera_phi);
float zdir = cos(camera_theta);
float magni = sqrt(xdir * xdir + ydir * ydir + zdir * zdir);
float launch = 100.0;

btRigidBody::upcast(PhysStruct.dw->getCollisionObjectArray()[PhysStruct.vector_of_objects.size() - 1 + dynCurObjNum - 1])->applyCentralImpulse(btVector3(launch * xdir / magni, launch * ydir / magni, launch * zdir / magni)); 
//std::cout << "Done" << std::endl;

    }
    else if (m_event.button.button == SDL_BUTTON_RIGHT)
    {
      
    }
  }
else if (m_event.type == SDL_MOUSEMOTION)
  {
	
    	camera_phi = (/*camera_phi +*/ (m_event.motion.x - mouse_x - (1*m_WINDOW_WIDTH/4))/float(m_WINDOW_WIDTH/16));
	camera_theta = clamp(/*camera_theta +*/ (m_event.motion.y - mouse_y - (m_WINDOW_HEIGHT/2))/float(m_WINDOW_HEIGHT/8),0.5,3.14159);
	//std::cout << camera_phi << "," << camera_theta << std::endl;

    
//SDL_WarpMouseInWindow(m_window->gWindow,m_WINDOW_WIDTH/2,m_WINDOW_HEIGHT/2);

    	//camera_phi = (m_event.motion.x) / 40.0;
	//camera_theta = (m_event.motion.y) / 40.0;
/*
	std::cout << "MOT: " << m_event.motion.x
	<< "," << m_event.motion.y
	<< " MOU: " << mouse_x
	<< "," << mouse_y
	<< " SUB1: " << (m_event.motion.x - mouse_x -  (1*m_WINDOW_WIDTH/4))
	<<"," << (m_event.motion.y - mouse_y - (m_WINDOW_HEIGHT/2))
	<< " SUB2: " << (m_event.motion.x - (m_WINDOW_WIDTH/2))
	<<"," << (m_event.motion.y - (m_WINDOW_HEIGHT/2))
	<< std::endl;
*/
SDL_GetRelativeMouseState(&mouse_x,&mouse_y);

  }
  if(keystate_ctrl == true && keystate_r == true)
    {
      std::cout << "Reloading configuration file" << std::endl;
      return true;
    }
    return false;
}

unsigned int Engine::getDT()
{
  long long TimeNowMillis = GetCurrentTimeMillis();
  assert(TimeNowMillis >= m_currentTimeMillis);
  unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  m_currentTimeMillis = TimeNowMillis;
  return DeltaTimeMillis;
}

long long Engine::GetCurrentTimeMillis()
{
  timeval t;
  gettimeofday(&t, NULL);
  long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  return ret;
}
