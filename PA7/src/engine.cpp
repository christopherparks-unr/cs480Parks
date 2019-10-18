
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

  selection = m_graphics->object_search_by_name("Earth");

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

    //If the user is not in freecam mode, force the camera to look at the selected planet
    if (selection != nullptr) {
        m_graphics->m_camera->Reposition(selection->orbit_angle, selection->radius,m_WINDOW_WIDTH,m_WINDOW_HEIGHT);
    }

    //Draw the graphics
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
    //If the user is in freecam mode and is dragging the mouse, calculate spherical coordinate changes base upon the mouse movement
    view_phi = view_phi + (float)(m_event.motion.xrel / (m_WINDOW_WIDTH * 0.1));
    if(view_phi >= 2 * M_PI) {view_phi -= 2 * M_PI;}
    if(view_phi < 0) {view_phi += 2 * M_PI;}
    view_theta = view_theta + (float)(-m_event.motion.yrel / (m_WINDOW_HEIGHT * 0.1));
    if(view_theta >= 2 * M_PI) {view_theta -= 2 * M_PI;}
    if(view_theta < 0) {view_theta += 2 * M_PI;}
  }
  else if (m_event.type == SDL_KEYDOWN)
  {
    // handle key down events here
    switch(m_event.key.keysym.sym)
    {
      //Set the selected planet if they press a number
      case  SDLK_1:  selection = m_graphics->object_search_by_name("Mercury"); break;
      case  SDLK_2:  selection = m_graphics->object_search_by_name("Venus"); break;
      case  SDLK_3:  selection = m_graphics->object_search_by_name("Earth"); break;
      case  SDLK_4:  selection = m_graphics->object_search_by_name("Mars"); break;
      case  SDLK_5:  selection = m_graphics->object_search_by_name("Jupiter"); break;
      case  SDLK_6:  selection = m_graphics->object_search_by_name("Saturn"); break;
      case  SDLK_7:  selection = m_graphics->object_search_by_name("Uranus"); break;
      case  SDLK_8:  selection = m_graphics->object_search_by_name("Neptune"); break;
      case  SDLK_9:  selection = m_graphics->object_search_by_name("Pluto"); break;
      case  SDLK_0:  
                if (selection != nullptr) {
                  //Re-enter freecam by setting near-identical coordinates
                pos_x = 8.0 + (sin(selection->orbit_angle) * selection->radius);
                pos_y = 8.0;
                pos_z = 8.0 + (cos(selection->orbit_angle) * selection->radius);
                view_theta = -M_PI / 5;
                view_phi = -3*M_PI / 4;
                } selection = nullptr; break;
      case SDLK_LCTRL:
      case SDLK_RCTRL: keystate_ctrl = true; break;
      case SDLK_r: keystate_r = true; break;
      case SDLK_w: keystate_w = true; break;
      case SDLK_s: keystate_s = true; break;
      case SDLK_a: keystate_a = true; break;
      case SDLK_d: keystate_d = true; break;
      case SDLK_F12: m_graphics->menu_draw = false; m_graphics->meme_draw = !m_graphics->meme_draw; break; //Menu and meme visibility toggles
      case SDLK_m: m_graphics->meme_draw = false; m_graphics->menu_draw = !m_graphics->menu_draw; break;

    }
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
    if (m_event.button.button == SDL_BUTTON_RIGHT)
    {
      mouse_active = true; //Set the freecam drag flag to true
    }
    if (m_event.button.button == SDL_BUTTON_LEFT)
    {
        if (m_event.button.x >= 50 && m_event.button.x <= 85 &&
            m_event.button.y >= 110 && m_event.button.y <= 150) {
            if (m_graphics->simulation_speed > 0 && m_graphics->menu_draw) {
                m_graphics->simulation_speed -= 1;
            } //Simulate button pressed on menu to speed up and slow down simulation
        } else if (m_event.button.x >= 270 && m_event.button.x <= 310 &&
                   m_event.button.y >= 110 && m_event.button.y <= 150) {
            if (m_graphics->simulation_speed < 4 && m_graphics->menu_draw) {
                m_graphics->simulation_speed += 1;
            }

        }
    }

  }
  else if (m_event.type == SDL_MOUSEBUTTONUP)
  {
    // handle mouse down events here
    if (m_event.button.button == SDL_BUTTON_RIGHT)
    {
      mouse_active = false; //Unset freecam drag flag
    }

  }
  //Freecam movement variables, which get added to the camera position
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

  //Reposition the camera so it looks at an arbitrary point
  m_graphics->m_camera->Reposition(pos_x, pos_y, pos_z,
  pos_x + (1.0 * (float)cos(view_theta) * (float)cos(view_phi)),
  pos_y + (1.0 * (float)sin(view_theta)),
  pos_z + (1.0 * (float)cos(view_theta) * (float)sin(view_phi)),
  m_WINDOW_WIDTH, m_WINDOW_HEIGHT);

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
