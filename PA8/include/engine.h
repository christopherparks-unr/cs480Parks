#ifndef ENGINE_H
#define ENGINE_H

#include <sys/time.h>
#include <assert.h>
#include <bullet/btBulletDynamicsCommon.h>

#include "window.h"
#include "graphics.h"
struct Physics
{
	btBroadphaseInterface *bp;
	btDefaultCollisionConfiguration *cc;
	btCollisionDispatcher *dp;
	btSequentialImpulseConstraintSolver *s;
	btDiscreteDynamicsWorld *dw;

	void init()
	{
		bp = new btDbvtBroadphase();
		cc = new btDefaultCollisionConfiguration();
		dp = new btCollisionDispatcher(cc);
		s = new btSequentialImpulseConstraintSolver();
		dw = new btDiscreteDynamicsWorld(dp, bp, s, cc);
	}

	void add_object(std::string obj, Graphics *m_graphics)
	{
		Object* obj_to_add = m_graphics->object_search_by_name(obj);
		btVector3 triArray[3];
		btTriangleMesh *objTriMesh = new btTriangleMesh();
		for(int i = 0; i < (int)obj_to_add->Indices.size(); i += 3)
		{
			for (int j = 0; j < 3; j++) {
				glm::vec3 location = obj_to_add->Vertices[obj_to_add->Indices[i+j]].vertex;
				triArray[j] = btVector3(location[0],location[1],location[2]);
			}
			objTriMesh->addTriangle(triArray[0],triArray[1],triArray[2]);
		}
		btCollisionShape *shape = new btBvhTriangleMeshShape(objTriMesh, true);
		btDefaultMotionState *shapeMotionState = NULL;
		shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion(1, 1, 1, 1), btVector3(0, 0, 0)));
		btScalar mass(0);
		btVector3 inertia(0,0,0);
		btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, shape, inertia);
		btRigidBody *rigidBody = new btRigidBody(shapeRigidBodyCI);
		dw->addRigidBody(rigidBody, 0, 0);

		
		
	}/*
	void add_primitive(std::string type)
	{
		btCollisionShape *shape;
		if(type.compare("Triangle") == 0)
		{
			//shape = new btBvhTriangleMeshShape()
		}
	}*/
	
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
    bool Initialize(std::string v, std::string f);
    bool Run();
    bool Keyboard();
    unsigned int getDT();
    long long GetCurrentTimeMillis();

    Object* selection;
    Physics PhysStruct;

    bool keystate_ctrl;
    bool keystate_r;

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
