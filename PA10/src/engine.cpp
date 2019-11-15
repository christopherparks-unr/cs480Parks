
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
	rigidBody->setFriction(0.0);
	rigidBody->setRestitution(obj_to_add->restitution);
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
	shape->calculateLocalInertia(mass,inertia);
	btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, shape, inertia);
	btRigidBody *rigidBody = new btRigidBody(shapeRigidBodyCI);
	rigidBody->setFriction(0.0);
	rigidBody->setRestitution(obj_to_add->restitution);
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
	shape->calculateLocalInertia(mass,inertia);
	btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, shape, inertia);
	btRigidBody *rigidBody = new btRigidBody(shapeRigidBodyCI);
	rigidBody->setFriction(1.0);
	rigidBody->setRestitution(obj_to_add->restitution);
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
	rigidBody->setFriction(0.0);
	rigidBody->setRestitution(obj_to_add->restitution);
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
  PhysStruct.init();
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
  score = 0;
score_count = false;
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
  PhysStruct.add_object("Tray", m_graphics);
  PhysStruct.add_object("Tray1", m_graphics);

  PhysStruct.add_object("Inv", m_graphics);
  PhysStruct.add_sphere("Sphere", m_graphics, 1.0, 1.0);
  PhysStruct.add_box("Paddle_L", m_graphics, 10000.0, btVector3(6.3, 0.75, 0.5));
  PhysStruct.add_box("Paddle_R", m_graphics, 10000.0, btVector3(6.3, 0.75, 0.5));
  PhysStruct.add_box("Plunger", m_graphics, 3.0, btVector3(1.0, 1.0, 1.0));
  PhysStruct.add_object("Bax", m_graphics);
  PhysStruct.add_object("Bax1", m_graphics);
PhysStruct.add_object("Bumpers", m_graphics);

  btRigidBody::upcast(PhysStruct.dw->getCollisionObjectArray()[4])->setGravity(btVector3(0.0,0.0,0.0));
  btRigidBody::upcast(PhysStruct.dw->getCollisionObjectArray()[5])->setGravity(btVector3(0.0,0.0,0.0));

	ballsLeft = 3;
  return true;
}

bool ball_in_left(btVector3 where)
{
	if (where.getX() >= 5.0 && where.getX() <= 8.0) {
		if (where.getZ() >= -22.217-1.5 && where.getZ() <= -22.217+1.5) {
			return true;
		}
	}
	return false;
}

bool ball_in_right(btVector3 where)
{
	if (where.getX() >= -4.5 && where.getX() <= -1.5) {
		if (where.getZ() >= -22.217-1.5 && where.getZ() <= -22.217+1.5) {
			return true;
		}
	}
	return false;
}

bool Engine::Run()
{
  m_running = true;
  float left_rotation = 0.0;
  float left_goal = 3.141592/4.0;
  float right_rotation = 0.0;
  float right_goal = 3.141592/4.0;
  int left_counter = 0;
  int right_counter = 0;

  while(m_running && !killswitch)
  {
    // Update the DT
    m_DT = getDT();
	if(score_count)
	score++;
    // Check the keyboard input
    while(SDL_PollEvent(&m_event) != 0)
    {
      killswitch = Keyboard(&left_goal,&right_goal,&left_counter,&right_counter);
    }
glm::vec3 eye = {m_graphics->object_search_by_name("Sphere")->GetModel()[3][0], 40.0, -50.0 + m_graphics->object_search_by_name("Sphere")->GetModel()[3][2]};
glm::vec3 focus = {m_graphics->object_search_by_name("Sphere")->GetModel()[3][0], 0.0, 10.0 + m_graphics->object_search_by_name("Sphere")->GetModel()[3][2]};
m_graphics->m_camera->Reposition(eye, focus, m_WINDOW_WIDTH, m_WINDOW_HEIGHT);

if(m_graphics->object_search_by_name("Sphere")->GetModel()[3][2] < -27.0)
{
score_count = false;
	btTransform get = PhysStruct.dw->getCollisionObjectArray()[3]->getWorldTransform();
	btVector3 start(-18.0,-4.3,-22.5);
	get.setOrigin(start);
	PhysStruct.dw->getCollisionObjectArray()[3]->setWorldTransform(get);
	ballsLeft--;
}

    // physics
	PhysStruct.dw->getCollisionObjectArray()[3]->activate();
	PhysStruct.dw->getCollisionObjectArray()[4]->activate();
	PhysStruct.dw->getCollisionObjectArray()[5]->activate();
	PhysStruct.dw->getCollisionObjectArray()[6]->activate();
	PhysStruct.dw->getCollisionObjectArray()[7]->activate();

	//

	left_rotation += (left_goal-left_rotation)/6.0;
	right_rotation += (right_goal-right_rotation)/6.0;

	left_counter = max(0,left_counter-1);
	right_counter = max(0,right_counter-1);

	if (left_counter == 0) {
		left_goal = 0.0;
		PhysStruct.dw->getCollisionObjectArray()[4]->setRestitution(m_graphics->object_search_by_name("Paddle_L")->restitution);
	}
	if (right_counter == 0) {
		right_goal = 0.0;
		PhysStruct.dw->getCollisionObjectArray()[5]->setRestitution(m_graphics->object_search_by_name("Paddle_R")->restitution);
	}

	{
	btQuaternion rot;
	btTransform get;
	get.setIdentity();
	rot.setEuler(left_rotation,0.0,0.0);
	btVector3 start(9.853,-4.3,-23.217);
	
	
	get.setRotation(rot);
	get.setOrigin(start);
	PhysStruct.dw->getCollisionObjectArray()[4]->setWorldTransform(get);
	btRigidBody::upcast(PhysStruct.dw->getCollisionObjectArray()[4])->clearForces();
	btRigidBody::upcast(PhysStruct.dw->getCollisionObjectArray()[4])->setLinearVelocity(btVector3(0,0,0));
	if (left_goal == 0.0) {
		btRigidBody::upcast(PhysStruct.dw->getCollisionObjectArray()[4])->setAngularVelocity(btVector3(0,0.0,0));
	} else {
		btRigidBody::upcast(PhysStruct.dw->getCollisionObjectArray()[4])->setAngularVelocity(btVector3(0,0.5*pow(6.0,-left_rotation),0));
	}
	}

	{
	btQuaternion rot;
	btTransform get;
	get.setIdentity();
	rot.setEuler(-right_rotation,0.0,0.0);
	btVector3 start(-6.456,-4.3,-23.217);
	
	get.setRotation(rot);
	get.setOrigin(start);
	PhysStruct.dw->getCollisionObjectArray()[5]->setWorldTransform(get);
	btRigidBody::upcast(PhysStruct.dw->getCollisionObjectArray()[5])->clearForces();
	btRigidBody::upcast(PhysStruct.dw->getCollisionObjectArray()[5])->setLinearVelocity(btVector3(0,0,0));
	if (right_goal == 0.0) {
		btRigidBody::upcast(PhysStruct.dw->getCollisionObjectArray()[5])->setAngularVelocity(btVector3(0,0.0,0));
	} else {
		btRigidBody::upcast(PhysStruct.dw->getCollisionObjectArray()[5])->setAngularVelocity(btVector3(0,-0.5*pow(6.0,-right_rotation),0));
	}
	}

	// WOW
    PhysStruct.dw->stepSimulation(m_DT,10);

	// fix vis sphere

	{
		btTransform get = PhysStruct.dw->getCollisionObjectArray()[3]->getWorldTransform();
		btVector3 start = get.getOrigin();
		start.setY(-4.9);
		get.setOrigin(start);
		PhysStruct.dw->getCollisionObjectArray()[3]->setWorldTransform(get);
	}

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
	glm::mat4 model_mod;
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			if (y == x) {
				model_mod[y][x] = 1;
			} else {
				model_mod[y][x] = 0;
			}
		}
	}
		PhysStruct.vector_of_objects[iter]->SetModel(m);
	/*
	if (iter == 4) {
		glm::vec3 loc;
		loc[0] = 9.853; loc[1] = -4.5; loc[2] = -23.217;
		model_mod = glm::translate(model_mod,loc);
		model_mod = glm::rotate(model_mod,left_rotation,glm::vec3(0.0,1.0,0.0));
		PhysStruct.vector_of_objects[iter]->SetModel(model_mod);
	} else if (iter == 5) {
		glm::vec3 loc;
		loc[0] = -6.456; loc[1] = -4.5; loc[2] = -23.217;
		model_mod = glm::translate(model_mod,loc);
		model_mod = glm::rotate(model_mod,-right_rotation,glm::vec3(0.0,1.0,0.0));
		PhysStruct.vector_of_objects[iter]->SetModel(model_mod);
	} else {
	}
	*/

    }

	//

	{
	btTransform get = PhysStruct.dw->getCollisionObjectArray()[6]->getWorldTransform();
	btVector3 orig = get.getOrigin();

	if (orig.getZ() >= -26.5) {
		btRigidBody::upcast(PhysStruct.dw->getCollisionObjectArray()[6])->clearForces();
		btRigidBody::upcast(PhysStruct.dw->getCollisionObjectArray()[6])->setLinearVelocity(btVector3(0,0,0));
		btRigidBody::upcast(PhysStruct.dw->getCollisionObjectArray()[6])->setAngularVelocity(btVector3(0,0,0));

		btTransform get = PhysStruct.dw->getCollisionObjectArray()[6]->getWorldTransform();
		btVector3 start(-17.3277,-4.6,-26.51);
		get.setOrigin(start);
		PhysStruct.dw->getCollisionObjectArray()[6]->setWorldTransform(get);
	}
	}

	/*
	{
	btTransform read = PhysStruct.dw->getCollisionObjectArray()[3]->getWorldTransform();
	btVector3 get = read.getOrigin();
	if (ball_in_right(get)) {
		std::cout << "LEFT" << std::endl;
	}
	}
	*/	

    // Update and render the graphics
    m_graphics->Update(m_DT);
    m_graphics->Render();

    // Swap to the Window
    m_window->Swap();
	if(ballsLeft == 0)
	{
		m_running = false; //Ends program
		//killswitch = true; //Restarts program
	}
  }
	
  return killswitch;
}

bool Engine::Keyboard(float* left_goal, float* right_goal, int* left_counter, int* right_counter)
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
	
	
	case SDLK_q: {
		if (*left_counter == 0) {
			*left_goal = 3.141592/4.0;
			*left_counter = 20;
			PhysStruct.dw->getCollisionObjectArray()[4]->setRestitution(4.0);
		}
	} break;
	case SDLK_e: {
		if (*right_counter == 0) {
			*right_goal = 3.141592/4.0;
			*right_counter = 20;
			PhysStruct.dw->getCollisionObjectArray()[5]->setRestitution(4.0);
		}
	} break;
	


	case SDLK_SPACE: {
	score_count = true;
	btRigidBody::upcast(PhysStruct.dw->getCollisionObjectArray()[6])->applyCentralImpulse(btVector3(0.0,0.0,36.0)); 
	} break;

      case SDLK_l: m_graphics->object_search_by_name("Tray1")->specular_shininess = max(0.25, (m_graphics->object_search_by_name("Tray1")->specular_shininess) / 2.0); break;
      case SDLK_k: m_graphics->object_search_by_name("Tray1")->specular_shininess = min(256.0, (m_graphics->object_search_by_name("Tray1")->specular_shininess) * 2.0); break;

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
  else if (m_event.type == SDL_MOUSEBUTTONDOWN && selection != nullptr)
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
