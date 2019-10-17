
#include "engine.h"
#include <cmath>

Engine::Engine(string name, int width, int height)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  m_FULLSCREEN = false;
  keystate_ctrl = false;
  keystate_r = false;
  killswitch = false;
}

Engine::Engine(string name)
{
  m_WINDOW_NAME = name;
  m_WINDOW_HEIGHT = 0;
  m_WINDOW_WIDTH = 0;
  m_FULLSCREEN = true;

  keystate_w = false;
  keystate_s = false;
  keystate_a = false;
  keystate_d = false;

  move_x = 0.0;
  move_y = 0.0;
  move_z = 0.0;

  move_speed = 32.0;

  pos_x = 0.0;
  pos_y = 32.0;
  pos_z = -64.0;

  view_theta = 0.0; //Vertical
  view_phi = 0.0; //Horizontal

  mouse_active = false;
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
  if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT))
  {
    printf("The window failed to initialize.\n");
    return false;
  }
  // Start the graphics, now with path support for shaders
  m_graphics = new Graphics();
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, v, f))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();
  // No errors
  return true;
}

bool Engine::Run()
{
  m_running = true;

  while(m_running && !killswitch)
  {
    // Update the DT
    m_DT = getDT();

    // Check the keyboard input
    while(SDL_PollEvent(&m_event) != 0)
    {
      killswitch = Keyboard();
    }

    // Update and render the graphics
    m_graphics->Update(m_DT);
    m_graphics->Render();

    // Swap to the Window
    m_window->Swap();
  }
  return killswitch;
}

bool Engine::Keyboard()
{
  if(m_event.type == SDL_QUIT)
  {
    m_running = false;
  }
  else if(m_event.type == SDL_MOUSEMOTION && mouse_active)
  {
    view_phi = view_phi + (float)atan(m_event.motion.xrel / (m_WINDOW_WIDTH * 32.0));
    if(view_phi >= 2 * M_PI) {view_phi -= 2 * M_PI;}
    if(view_phi < 0) {view_phi += 2 * M_PI;}
    view_theta = view_theta + (float)atan(m_event.motion.yrel / (m_WINDOW_HEIGHT * 32.0));
    if(view_theta >= 2 * M_PI) {view_theta -= 2 * M_PI;}
    if(view_theta < 0) {view_theta += 2 * M_PI;}
  }
  else if (m_event.type == SDL_KEYDOWN)
  {
    // handle key down events here
    switch(m_event.key.keysym.sym)
    {
      //case  SDLK_1:  selection = m_graphics->m_cube; break;
      //case  SDLK_2:  selection = m_graphics->m_moon; break;
      case  SDLK_0:  selection = nullptr; break;
      case SDLK_LCTRL:
      case SDLK_RCTRL: keystate_ctrl = true; break;
      case SDLK_r: keystate_r = true; break;
      case SDLK_w: keystate_w = true; break;
      case SDLK_s: keystate_s = true; break;
      case SDLK_a: keystate_a = true; break;
      case SDLK_d: keystate_d = true; break;
    }/*
    if(selection != nullptr)
    {
      switch(m_event.key.keysym.sym)
      {
        case SDLK_ESCAPE: m_running = false; break;
        case SDLK_q:      selection->rotation_angle_mod *= -1; break; //Reverse rotation direction
        case SDLK_w:      selection->orbit_angle_mod *= -1; break; //Reverse orbit direction
        case SDLK_a:      selection->rotation_angle_paused = selection->rotation_angle_paused == 0.0f ? 1.0f : 0.0f; break; //Pause and unpause rotation
        case SDLK_s:      selection->orbit_angle_paused = selection->orbit_angle_paused == 0.0f ? 1.0f : 0.0f; break; //Pause and unpause orbit
      }
    }*/
  }
  else if(m_event.type == SDL_KEYUP)
  {
    switch(m_event.key.keysym.sym)
    {
      case SDLK_LCTRL:
      case SDLK_RCTRL: keystate_ctrl = false; break;
      case SDLK_r: keystate_r = false; break;
      case SDLK_w: keystate_w = false; break;
      case SDLK_s: keystate_s = false; break;
      case SDLK_a: keystate_a = false; break;
      case SDLK_d: keystate_d = false; break;
    }
  }
  else if (m_event.type == SDL_MOUSEBUTTONDOWN)
  {
    // handle mouse down events here
    if (m_event.button.button == SDL_BUTTON_LEFT)
    {
      if(!mouse_active)
      {
        SDL_SetRelativeMouseMode(SDL_TRUE);
        //SDL_WarpMouseInWindow(m_window->gWindow, round(m_WINDOW_WIDTH / 2), round(m_WINDOW_HEIGHT / 2));
      }
      mouse_active = true;
    }

  }
  else if (m_event.type == SDL_MOUSEBUTTONUP)
  {
    // handle mouse down events here
    if (m_event.button.button == SDL_BUTTON_LEFT)
    {
      SDL_SetRelativeMouseMode(SDL_FALSE);
      mouse_active = false;
    }

  }
  move_x = 0.0;
  move_z = 0.0;
  move_y = 0.0;
  if(keystate_w)
  {
    move_x += (float)cos(view_theta) * cos(view_phi);
    move_z += (float)cos(view_theta) * sin(view_phi);
    move_y += (float)sin(view_theta);
  }
  if(keystate_s)
  {
    move_x += -1.0 * (float)cos(view_theta) * cos(view_phi);
    move_z += -1.0 * (float)cos(view_theta) * sin(view_phi);
    move_y += -1.0 * (float)sin(view_theta);
  }
  
  if(keystate_a)
  {
    move_x += (float)cos(view_phi - (M_PI / 2));
    move_z += (float)sin(view_phi - (M_PI / 2));
    move_y += 0.0;
  }
  if(keystate_d)
  {
    move_x += (float)cos(view_phi + (M_PI / 2));
    move_z += (float)sin(view_phi + (M_PI / 2));
    move_y += 0.0;
  }

  pos_x += move_x;
  pos_y += move_y;
  pos_z += move_z;

  m_graphics->m_camera->Reposition(pos_x, pos_y, pos_z,
  pos_x + (1.0 * (float)cos(view_theta) * (float)cos(view_phi)),
  pos_y + (1.0 * (float)sin(view_theta)),
  pos_z + (1.0 * (float)cos(view_theta) * (float)sin(view_phi)),
  m_WINDOW_WIDTH, m_WINDOW_HEIGHT);
/*
printf("Pos: (%f,%f,%f) View: (%f, %f) -> (%f, %f, %f)\n", pos_x, pos_y, pos_z, view_phi, view_theta,
pos_x + move_speed * (float)cos(view_theta) * (float)cos(view_phi),
pos_y + move_speed * (float)sin(view_theta),
pos_z + move_speed * (float)cos(view_theta) * (float)sin(view_phi));
*/
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
