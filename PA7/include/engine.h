#ifndef ENGINE_H
#define ENGINE_H

#include <sys/time.h>
#include <assert.h>

#include "window.h"
#include "graphics.h"

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

    Object* selection;

    bool keystate_ctrl;
    bool keystate_r;

    bool keystate_w;
    bool keystate_s;
    bool keystate_a;
    bool keystate_d;

    float pos_x;
    float pos_y;
    float pos_z;

    float move_x;
    float move_y;
    float move_z;

    float view_theta;
    float view_phi;

    bool mouse_active;

    bool killswitch;
  
  private:
    // Window related variables
    Window *m_window;    
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
