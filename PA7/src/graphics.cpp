#include "graphics.h"

std::vector<gen_object> genlist;

//Parses through the ini file to set model attributes such as scale, orbit radius, and rotation speed
void read_ini()
{
	gen_object make;
	int index_read = 0;

	genlist.clear();

	std::string s;
	std::filebuf fb;
	if (fb.open("../main.ini",std::ios::in)) {
		std::istream in(&fb);
		while (std::getline(in,s)) {
			if (s.length() == 0) {continue;}

      //Determines the name of the object being loaded
			if (s[0] == '[')
      {
				if (index_read != 0) {
					genlist.push_back(make);
				}

				make.name.clear();

				int read = 1;
				while (s[read] != ']') {
					make.name.push_back(s[read]);
					read++;
				}

				index_read += 1;
				make.scene_path = "../assets/scenes/cube.obj";
				make.mesh_index = 0;
				make.texture_path = "../assets/scenes/default.jpg";
				make.scale = 1.0f;
				make.radius = 0.0f;
				make.rotation_angle_mod = 0.25f;
                                make.rotation_offset = 0.0f;
				make.orbit_angle_mod = 0.0f;
                                make.orbit_offset = 0.0f;
				make.parent = "nullptr";

        //Defines the path of the file to import
			} else if (s.find("scene_path=") == 0) {
				make.scene_path = s.substr(11,s.length());
        //Defines the name of the mesh to use. The name of the mesh is defined by assimp... Specifically the aiMesh's mName variable
			} else if (s.find("mesh_index=") == 0) {
                                s = s.substr(11,s.length());
				make.mesh_index = std::stoi(s);
        //Defines a texture path, which overrides the default texture assigned to the mesh
                        } else if (s.find("texture_path=") == 0) {
				make.texture_path = s.substr(13,s.length());
        //Scale of the object. 1.0 is default
			} else if (s.find("scale=") == 0) {
				s = s.substr(6,s.length());
				make.scale = std::stof(s);
        //Orbit radius
      } else if (s.find("radius=") == 0) {
				s = s.substr(7,s.length());
				make.radius = std::stof(s);
        //Rotation speed
			} else if (s.find("rotation_speed=") == 0) {
				s = s.substr(15,s.length());
				make.rotation_angle_mod = std::stof(s);
       //Initial rotation offset
			} else if (s.find("rotation_offset=") == 0) {
				s = s.substr(16,s.length());
				make.rotation_offset = std::stof(s);
        //Orbit speed
			} else if (s.find("orbit_speed=") == 0) {
				s = s.substr(12,s.length());
				make.orbit_angle_mod = std::stof(s);
        //Initial orbit offset
			} else if (s.find("orbit_offset=") == 0) {
				s = s.substr(13,s.length());
				make.orbit_offset = std::stof(s);
        //Parent object
			} else if (s.find("parent=") == 0) {
				s = s.substr(7,s.length());
				make.parent = s;
			}
		}
		fb.close();
	}
}

//Function that returns a parent object's pointer given its name, as defined within main.ini (eg. [ModelA])
Object* Graphics::object_search_by_name(std::string findme)
{
  for (int iter = 0; iter < (int) genlist.size(); iter++)
  {
    if(findme.compare(genlist[iter].name) == 0)
    {
      return &(objlist[iter]);
    }
  }
  return nullptr;
}

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

  genlist.clear();

  objlist.clear();

}

bool Graphics::Initialize(int width, int height, std::string v, std::string f)
{
	read_ini();

  // Used for the linux OS
  #if !defined(__APPLE__) && !defined(MACOSX)
    glewExperimental = GL_TRUE;

    auto status = glewInit();

    // This is here to grab the error that comes from glew init.
    // This error is an GL_INVALID_ENUM that has no effects on the performance
    glGetError();

    //Check for error
    if (status != GLEW_OK)
    {
      std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
      return false;
    }
  #endif

  // For OpenGL 3
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Init Camera
  m_camera = new Camera();
  if(!m_camera->Initialize(width, height))
  {
    printf("Camera Failed to Initialize\n");
    return false;
  }

  // Init menuspace camera
  m_menu_camera = new Camera();
  if(!m_menu_camera->Initialize(width, height))
  {
    printf("Menu camera Failed to Initialize\n");
    return false;
  }

  //Create the objects
	objlist.clear();

  //Iterate through the list of object information, create the objects with their attributes, and push them into an object list
	for (int iter = 0; iter < (int) genlist.size(); iter++)
	{
    Object temp(genlist[iter].scene_path,genlist[iter].mesh_index,
                genlist[iter].texture_path,
		genlist[iter].scale,genlist[iter].radius,
		genlist[iter].rotation_angle_mod,genlist[iter].rotation_offset,
		genlist[iter].orbit_angle_mod,genlist[iter].orbit_offset,
                nullptr);

		objlist.push_back(temp);
	}

  //Parent info is done separately so that the pointers will always point to what they're supposed to; otherwise the vector could be reallocated elsewhere in memory
  for (int iter = 0; iter < (int) genlist.size(); iter++)
  {
    Object* par_ptr = nullptr;
    if(genlist[iter].parent.compare("nullptr") != 0)
    {
      par_ptr = object_search_by_name(genlist[iter].parent);
    }

    objlist[iter].SetParent(par_ptr);
  }

  simulation_speed = 3;

  menu_obj[0] = new Object("../assets/scenes/menu.obj",0,"../assets/scenes/menu_zero.png",1.0,0.0,0.0,0.0,0.0,0.0,nullptr);
  menu_obj[1] = new Object("../assets/scenes/menu.obj",0,"../assets/scenes/menu_one.png",1.0,0.0,0.0,0.0,0.0,0.0,nullptr);
  menu_obj[2] = new Object("../assets/scenes/menu.obj",0,"../assets/scenes/menu_two.png",1.0,0.0,0.0,0.0,0.0,0.0,nullptr);
  menu_obj[3] = new Object("../assets/scenes/menu.obj",0,"../assets/scenes/menu_three.png",1.0,0.0,0.0,0.0,0.0,0.0,nullptr);
  menu_obj[4] = new Object("../assets/scenes/menu.obj",0,"../assets/scenes/menu_four.png",1.0,0.0,0.0,0.0,0.0,0.0,nullptr);

  meme_draw = false;
  meme = new Object("../assets/scenes/menu.obj",0,"../assets/scenes/harris.png",1.0,0.0,0.0,0.0,0.0,0.0,nullptr);

  // Set up the shaders
  m_shader = new Shader();
  if(!m_shader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader, now with path support
  if(!m_shader->AddShader(GL_VERTEX_SHADER, v))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader, now with path support
  if(!m_shader->AddShader(GL_FRAGMENT_SHADER, f))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  // Connect the program
  if(!m_shader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

  // Add the menu shader
  m_menu_shader = new Shader();
  if (!m_menu_shader->Initialize())
  {
    printf("Menu Shader Failed to Initialize\n");
    return false;
  }

  if (!m_menu_shader->AddShader(GL_VERTEX_SHADER,"../assets/shaders/vertex_menu_pa7.txt"))
  {
    printf("Menu Vertex Shader failed to Initialize\n");
    return false;
  }

  if (!m_menu_shader->AddShader(GL_FRAGMENT_SHADER,"../assets/shaders/fragment_menu_pa7.txt"))
  {
    printf("Menu Fragment Shader failed to Initialize\n");
    return false;
  }

  // Connect the program
  if(!m_menu_shader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  //Flag that determines if the menu is drawn
  menu_draw = true;

  return true;
}

void Graphics::Update(unsigned int dt)
{
  // Update the objects
	for (int iter = 0; iter < (int) objlist.size(); iter++)
	{
      if (simulation_speed == 0) {
			objlist[iter].Update(0); //Halt dt if simulation speed is zero
		} else {
			objlist[iter].Update(dt*0.25*pow(2.0,simulation_speed-1.0)); //Otherwise update with a dt multiplier
		}
	}
  //Update the menu objects (one for each potential simulation speed state)
	for (int iter = 0; iter < 5; iter++)
	{
        menu_obj[iter]->Update(dt);
  }
}

void Graphics::Render()
{
  //clear the screen
  glClearColor(0.0, 0.0, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  //Main object rendering
  //---------------------
  // Locate the projection matrix in the shader
  m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_projectionMatrix not found\n");
  }

  // Locate the view matrix in the shader
  m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_viewMatrix not found\n");
  }

  // Locate the model matrix in the shader
  m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_modelMatrix not found\n");
  }

  // Start the generic (lighting) shader
  m_shader->Enable();

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

  // Render the objects
	for (int iter = 0; iter < (int) objlist.size(); iter++)
	{
		glUniformMatrix4fv(m_modelMatrix,1,GL_FALSE,glm::value_ptr(objlist[iter].GetModel()));
		objlist[iter].Render();
	}
  //--------------------
  //Menu objects are located in a separate "space" using a different camera, use their own view and projection matrix, etc.
  //Menu object loading
  //--------------------
  // Locate the projection matrix in the shader
  m_projectionMatrix = m_menu_shader->GetUniformLocation("projectionMatrix");
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_projectionMatrix not found\n");
  }

  // Locate the view matrix in the shader
  m_viewMatrix = m_menu_shader->GetUniformLocation("viewMatrix");
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_viewMatrix not found\n");
  }

  // Locate the model matrix in the shader
  m_modelMatrix = m_menu_shader->GetUniformLocation("modelMatrix");
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_modelMatrix not found\n");
  }

  // Start the menu shader
  m_menu_shader->Enable();

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_menu_camera->GetProjection())); 
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_menu_camera->GetView())); 

  glUniformMatrix4fv(m_modelMatrix,1,GL_FALSE,glm::value_ptr(menu_obj[simulation_speed]->GetModel()));
  //Draw the menu itself
  if (menu_draw && !meme_draw) {menu_obj[simulation_speed]->Render();}
  //Draw the meme
  if (meme_draw) {meme->Render();};

  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
  }
  //-----------------
}

std::string Graphics::ErrorString(GLenum error)
{
  if(error == GL_INVALID_ENUM)
  {
    return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
  }

  else if(error == GL_INVALID_VALUE)
  {
    return "GL_INVALID_VALUE: A numeric argument is out of range.";
  }

  else if(error == GL_INVALID_OPERATION)
  {
    return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
  }

  else if(error == GL_INVALID_FRAMEBUFFER_OPERATION)
  {
    return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
  }

  else if(error == GL_OUT_OF_MEMORY)
  {
    return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
  }
  else
  {
    return "None";
  }
}

