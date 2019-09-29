
#include "engine.h"
#include <fstream>
#include <cstdlib>

Engine::Engine(string name, int width, int height)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  m_FULLSCREEN = false;
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

void Engine::Run()
{
  m_running = true;

  while(m_running)
  {
    // Update the DT
    m_DT = getDT();

    // Check the keyboard input
    while(SDL_PollEvent(&m_event) != 0)
    {
      Keyboard();
    }

    // Update and render the graphics
    m_graphics->Update(m_DT);
    m_graphics->Render();

    // Swap to the Window
    m_window->Swap();
  }
}

void Engine::Keyboard()
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
      //case  SDLK_1:  selection = m_graphics->m_object; break;
      //case  SDLK_2:  selection = m_graphics->m_moon; break;
      case  SDLK_0:  selection = nullptr; break;
    }
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
    }

  }
  else if (m_event.type == SDL_MOUSEBUTTONDOWN && selection != nullptr)
  {
    // handle mouse down events here
    if (m_event.button.button == SDL_BUTTON_LEFT)
    {
      selection->rotation_angle_mod *= -1;
    }
    else if (m_event.button.button == SDL_BUTTON_RIGHT)
    {
      selection->orbit_angle_mod *= -1;
    }
  }
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
