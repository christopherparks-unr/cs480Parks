#ifndef ENGINE_H
#define ENGINE_H

#include <sys/time.h>
#include <assert.h>


#include "window.h"
#include "graphics.h"

struct Physics
{
	btBroadphaseInterface *bp;
	btDefaultCollisionConfiguration *cc;
	btCollisionDispatcher *dp;
	btSequentialImpulseConstraintSolver *s;
	btDiscreteDynamicsWorld *dw;

	std::vector<btCollisionShape*> vector_of_shapes;
	std::vector<Object*> vector_of_objects;

	void init()
	{
		bp = new btDbvtBroadphase();
		cc = new btDefaultCollisionConfiguration();
		dp = new btCollisionDispatcher(cc);
		s = new btSequentialImpulseConstraintSolver();
		dw = new btDiscreteDynamicsWorld(dp, bp, s, cc);
		dw->setGravity(btVector3(0,-0.51,0));
		vector_of_shapes.clear();
		vector_of_objects.clear();
	}


	void add_object(std::string obj, Graphics *m_graphics);

	void add_sphere(std::string obj, Graphics *m_graphics, float m, float r);

	void add_box(std::string obj, Graphics *m_graphics, float m, btVector3 size);

	void add_cylinder(std::string obj, Graphics *m_graphics, float m, btVector3 idk);


	
	void destruct()
	{
		delete bp;
		delete cc;
		delete dp;
		delete s;
		delete dw;
		bp = nullptr;
		cc = nullptr;
		dp = nullptr;
		s = nullptr;
		dw = nullptr;
		
	}
};

class Engine
{
  public:
    Engine(string name, int width, int height);
    Engine(string name);
    ~Engine();
    bool Initialize(std::string v1, std::string f1, std::string v2, std::string f2);
    bool Run();
    bool Keyboard();
    unsigned int getDT();
    long long GetCurrentTimeMillis();

    Object* selection;
    Physics PhysStruct;

    bool keystate_ctrl;
    bool keystate_r;
    bool use_vertex_lighting;

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
