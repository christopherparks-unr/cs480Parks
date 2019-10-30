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


	void add_object(std::string obj, Graphics *m_graphics)
	{
		Object* obj_to_add = m_graphics->object_search_by_name(obj);
		assert(obj_to_add);
		vector_of_objects.push_back(obj_to_add);
		btVector3 triArray[3];
		btTriangleMesh *objTriMesh = new btTriangleMesh();
		for(int i = 0; i < (int)obj_to_add->Vertices.size(); i += 3)
		{
			for (int j = 0; j < 3; j++) {
				glm::vec3 location = obj_to_add->Vertices[i+j].vertex;
				triArray[j] = btVector3(location[0],location[1],location[2]);
			}
			objTriMesh->addTriangle(triArray[0],triArray[1],triArray[2]);
		}
		btCollisionShape *shape = new btBvhTriangleMeshShape(objTriMesh, true);
		vector_of_shapes.push_back(shape);
		
		btDefaultMotionState *shapeMotionState = NULL;
		shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0.0, -5.0, 0.0)));
		btScalar mass(0.0);
		btVector3 inertia(0,0,0);
		btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, shape, inertia);
		btRigidBody *rigidBody = new btRigidBody(shapeRigidBodyCI);
		dw->addRigidBody(rigidBody);
	}
	void add_sphere(std::string obj, Graphics *m_graphics, float r)
	{
		Object* obj_to_add = m_graphics->object_search_by_name(obj);
		vector_of_objects.push_back(obj_to_add);
		btCollisionShape *shape = new btSphereShape(btScalar(r));
		vector_of_shapes.push_back(shape);

		btDefaultMotionState *shapeMotionState = NULL;
		shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0.0, 25.0, 0.0)));
		btScalar mass(1.0);
		btVector3 inertia(0,0,0);
		btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, shape, inertia);
		btRigidBody *rigidBody = new btRigidBody(shapeRigidBodyCI);
		dw->addRigidBody(rigidBody);
	}

	void add_box(std::string obj, Graphics *m_graphics, btVector3 v, float f)
	{
		Object* obj_to_add = m_graphics->object_search_by_name(obj);
		vector_of_objects.push_back(obj_to_add);
		btCollisionShape *shape = nullptr;
		shape = new btBoxShape(btVector3(5.0,5.0,5.0));
		vector_of_shapes.push_back(shape);

		btDefaultMotionState *shapeMotionState = NULL;
		shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0.0, -5.0, 0.0)));
		btScalar mass(0.0);
		btVector3 inertia(0,0,0);
		btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, shape, inertia);
		btRigidBody *rigidBody = new btRigidBody(shapeRigidBodyCI);
		dw->addRigidBody(rigidBody);
	}


	
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
