
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
  bulletCycle = 0;

  camera_theta = 3.141592/2.0;
  camera_phi = 0.0;
  player_movement = 16.0;
  mouse_x = 0.0; mouse_y = 0.0;
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
  m_graphics = new Graphics();
  // Start the graphics, now with path support for shaders


  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, v, f, &PhysStruct))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }


  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();
  return true;
}

bool Engine::Run()
{
  m_running = true;

  btVector3 velocity_player;
  const unsigned char* state = (const unsigned char*) SDL_GetKeyboardState(NULL);


  while(m_running && !killswitch)
  {

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

    if (state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_S])
    {
        velocity_player.setX(player_movement*cos(camera_phi));
        velocity_player.setZ(player_movement*sin(camera_phi));
    } else if (!state[SDL_SCANCODE_W] && state[SDL_SCANCODE_S])
    {
        velocity_player.setX(-player_movement*cos(camera_phi));
        velocity_player.setZ(-player_movement*sin(camera_phi));
    }

    if (state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_D])
    {
        velocity_player.setX(player_movement*sin(camera_phi));
        velocity_player.setZ(-player_movement*cos(camera_phi));
    } else if (!state[SDL_SCANCODE_A] && state[SDL_SCANCODE_D])
    {
        velocity_player.setX(-player_movement*sin(camera_phi));
        velocity_player.setZ(player_movement*cos(camera_phi));
    } 

    btRigidBody::upcast(PhysStruct.dw->getCollisionObjectArray()[0])->setLinearVelocity(velocity_player);
//PhysStruct.dw->getCollisionObjectArray()[0]->activate();
	for (int iter = 0; iter < PhysStruct.dw->getNumCollisionObjects(); iter++)
	{
	    PhysStruct.dw->getCollisionObjectArray()[iter]->activate();
	}

    // WOW
    PhysStruct.dw->stepSimulation(m_DT,10);

	for (int iter = 0; iter < PhysStruct.dw->getNumCollisionObjects(); iter++)
	{

		btCollisionObject* obj = PhysStruct.dw->getCollisionObjectArray()[iter];
		btRigidBody* body = btRigidBody::upcast(obj);
		btTransform trans;
		btScalar m[16];
		if (body && body->getMotionState())
		{
			body->getMotionState()->getWorldTransform(trans);
		} else
		{
			trans = obj->getWorldTransform();
		}
		trans.getOpenGLMatrix(m);
		PhysStruct.vector_of_objects[iter]->SetModel(m);
	}

    //Teleporting dead bullet objects

	    for(int iter = 0; iter < (int)m_graphics->objlist.size(); iter++)
	    {

		if(m_graphics->objlist[iter].lifetime == 1)
		{


		btRigidBody::upcast(m_graphics->objlist[iter].rigidBody)->clearForces();
		btRigidBody::upcast(m_graphics->objlist[iter].rigidBody)->setLinearVelocity(btVector3(0,0,0));
		btRigidBody::upcast(m_graphics->objlist[iter].rigidBody)->setAngularVelocity(btVector3(0,0,0));

		btTransform get = m_graphics->objlist[iter].rigidBody->getWorldTransform();
		btVector3 start(0,-60,0);
		get.setOrigin(start);
		m_graphics->objlist[iter].rigidBody->setWorldTransform(get);
		
		}
		if(m_graphics->objlist[iter].lifetime > 0)
		{
			m_graphics->objlist[iter].lifetime -= 1;
		}
		
		
	    }

    //camera location
    btVector3 getCamera = m_graphics->objlist[0].rigidBody->getWorldTransform().getOrigin();
    glm::vec3 loc(getCamera.getX()+0.0,getCamera.getY()+10.0,getCamera.getZ());
    m_graphics->m_camera->Reposition(loc,camera_theta,camera_phi, m_WINDOW_WIDTH, m_WINDOW_HEIGHT);

    // Update and render the graphics
    m_graphics->Update(m_DT);

    m_graphics->Render();


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
    if (m_event.button.button == SDL_BUTTON_LEFT)
    {


	
	
float xdir;
float ydir;
float zdir;
float launch = 150.0;

//std::cout << "Bringing in bullets" << std::endl;
//int n = 0;
/*
for(float hshift = -1.0; hshift <= 1.0; hshift += 1.0)
{
	for(float vshift = -1.0; vshift <= 1.0; vshift += 1.0)
	{*/

		

		
		//xdir = sin(camera_theta + 0.0625 * vshift) * cos(camera_phi + 0.0625 * hshift);
		//ydir = cos(camera_theta + 0.0625 * vshift);
		//zdir = sin(camera_theta + 0.0625 * vshift) * sin(camera_phi + 0.0625 * hshift);
		xdir = sin(camera_theta) * cos(camera_phi);
		ydir = cos(camera_theta);
		zdir = sin(camera_theta) * sin(camera_phi);
		//std::cout << hshift << "," << vshift << " = " << xdir << "," << ydir << "," << zdir << std::endl;
		
		//int which = m_graphics->objlist.size() - (n + bulletCycle*9) - 1;
		int which = m_graphics->objlist.size() - bulletCycle - 1;
		//std::cout << m_graphics->objlist[which].texture_path << " is " << hshift << "," <<vshift << std::endl;

		m_graphics->objlist[which].lifetime = 60;
		
		btVector3 getOrig = m_graphics->objlist[0].rigidBody->getWorldTransform().getOrigin();
		m_graphics->objlist[which].rigidBody->clearForces();
		m_graphics->objlist[which].rigidBody->setLinearVelocity(btVector3(launch*xdir , launch*ydir,launch*zdir));
		m_graphics->objlist[which].rigidBody->setAngularVelocity(btVector3(0,0,0));
		
		btTransform get = m_graphics->objlist[which].rigidBody->getWorldTransform();
		btVector3 start(getOrig.getX(), getOrig.getY() + 10.0, getOrig.getZ());
		get.setOrigin(start);
		m_graphics->objlist[which].rigidBody->setWorldTransform(get);
		//n += 1;
/*	}
}*/
//bulletCycle = (bulletCycle + 1) % 3;
bulletCycle = (bulletCycle + 1) % 27;

    }
    else if (m_event.button.button == SDL_BUTTON_RIGHT)
    {
      
    }
  }
else if (m_event.type == SDL_MOUSEMOTION)
  {
	
    	camera_phi = ((m_event.motion.x - mouse_x - (1*m_WINDOW_WIDTH/4))/float(m_WINDOW_WIDTH/16));
	camera_theta = clamp((m_event.motion.y - mouse_y - (m_WINDOW_HEIGHT/2))/float(m_WINDOW_HEIGHT/8),0.5,3.14159);

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
