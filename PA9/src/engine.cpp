
#include "engine.h"
#include <bullet/btBulletDynamicsCommon.h>

void Physics::add_object(std::string obj, Graphics *m_graphics)
{
	Object* obj_to_add = m_graphics->object_search_by_name(obj);
  btVector3 offset = btVector3(obj_to_add->x_offset, obj_to_add->y_offset, obj_to_add->z_offset);
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
	shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), offset));
	btScalar mass(0.0);
	btVector3 inertia(0,0,0);
	btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, shape, inertia);
	btRigidBody *rigidBody = new btRigidBody(shapeRigidBodyCI);
	dw->addRigidBody(rigidBody);
}

void Physics::add_sphere(std::string obj, Graphics *m_graphics, float m, float r)
{
	Object* obj_to_add = m_graphics->object_search_by_name(obj);
  btVector3 offset = btVector3(obj_to_add->x_offset, obj_to_add->y_offset, obj_to_add->z_offset);
	vector_of_objects.push_back(obj_to_add);
	btCollisionShape *shape = new btSphereShape(btScalar(r));
	vector_of_shapes.push_back(shape);

	btDefaultMotionState *shapeMotionState = NULL;
	shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), offset));
	btScalar mass(m);
	btVector3 inertia(0,0,0);
	btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, shape, inertia);
	btRigidBody *rigidBody = new btRigidBody(shapeRigidBodyCI);
	dw->addRigidBody(rigidBody);
}

void Physics::add_box(std::string obj, Graphics *m_graphics, float m, btVector3 size)
{
	Object* obj_to_add = m_graphics->object_search_by_name(obj);
  btVector3 offset = btVector3(obj_to_add->x_offset, obj_to_add->y_offset, obj_to_add->z_offset);
	vector_of_objects.push_back(obj_to_add);
	btCollisionShape *shape = nullptr;
	shape = new btBoxShape(size);
	vector_of_shapes.push_back(shape);

	btDefaultMotionState *shapeMotionState = NULL;
	shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), offset));
	btScalar mass(m);
	btVector3 inertia(0,0,0);
	btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, shape, inertia);
	btRigidBody *rigidBody = new btRigidBody(shapeRigidBodyCI);
	dw->addRigidBody(rigidBody);
}

void Physics::add_cylinder(std::string obj, Graphics *m_graphics, float m, btVector3 idk)
{
	Object* obj_to_add = m_graphics->object_search_by_name(obj);
  btVector3 offset = btVector3(obj_to_add->x_offset, obj_to_add->y_offset, obj_to_add->z_offset);
	vector_of_objects.push_back(obj_to_add);
	btCollisionShape *shape = nullptr;
	shape = new btCylinderShape(idk);
	vector_of_shapes.push_back(shape);

	btDefaultMotionState *shapeMotionState = NULL;
	shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), offset));
	btScalar mass(m);
	btVector3 inertia(0,0,0);
	btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, shape, inertia);
	btRigidBody *rigidBody = new btRigidBody(shapeRigidBodyCI);
	dw->addRigidBody(rigidBody);
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
  use_vertex_lighting = false;
  PhysStruct.init();
}

Engine::~Engine()
{
  delete m_window;
  delete m_graphics;
  m_window = NULL;
  m_graphics = NULL;
}

bool Engine::Initialize(std::string v1, std::string f1, std::string v2, std::string f2)
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
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, v1, f1, v2, f2))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();
  // No errors
  
  PhysStruct.add_object("Tray", m_graphics);
  PhysStruct.add_sphere("Sphere", m_graphics, 1.0, 1.0);
  PhysStruct.add_box("Box", m_graphics, 1.0, btVector3(1.0, 1.0, 1.0));
  PhysStruct.add_cylinder("Cylinder", m_graphics, 0.0, btVector3(1.0,1.0,1.0));
  

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

    // physics
    PhysStruct.dw->stepSimulation(m_DT,10);

    for (int iter = PhysStruct.dw->getNumCollisionObjects() - 1; iter >= 0; iter--)
    {

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
      case SDLK_l:
      {
        use_vertex_lighting = !use_vertex_lighting;
        m_graphics->use_vertex_lighting = use_vertex_lighting;
        std::cout << "AGH!" << std::endl;
        break;
      }

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

    }
    else if (m_event.button.button == SDL_BUTTON_RIGHT)
    {

    }
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
