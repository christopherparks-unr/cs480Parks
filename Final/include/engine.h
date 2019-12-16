#ifndef ENGINE_H
#define ENGINE_H

#include <sys/time.h>
#include <assert.h>

#include "window.h"
#include "graphics.h"
#include "physics.h"

class Engine
{
  public:
    Engine(string name, int width, int height);
    Engine(string name);
    ~Engine();
    bool Initialize(std::string v, std::string f);
    bool Run();
    bool Keyboard();
    unsigned int getDT();
    long long GetCurrentTimeMillis();
    int bulletCycle;

    Object* selection;
    Physics PhysStruct;

    bool keystate_ctrl;
    bool keystate_r;
    bool killswitch;
    float player_movement;

    float camera_theta, camera_phi;
    int mouse_x, mouse_y;
    Window *m_window;    

  private:
    // Window related variables
    string m_WINDOW_NAME;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN;
    SDL_Event m_event;

    Graphics *m_graphics;
    unsigned int m_DT;
    long long m_currentTimeMillis;
    bool m_running;
};

#endif // ENGINE_H
