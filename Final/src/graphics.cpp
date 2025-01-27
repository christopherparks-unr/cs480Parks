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
			if (s[0] == '[') {
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
				make.texture_path = "";
				make.x_offset = 0.0;
				make.y_offset = 0.0;
				make.z_offset = 0.0;
				make.specular_shininess = 2.0;
				make.restitution = 0.3;

				make.shape_type = "object";
				make.lifetime = -1;
				make.mass = 0.0;
				make.radius = 1.0;
				make.x_phys = 1.0;
				make.y_phys = 1.0;
				make.z_phys = 1.0;

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
			} else if (s.find("x=") == 0) {
				s = s.substr(2,s.length());
				make.x_offset = std::stof(s);
			} else if (s.find("y=") == 0) {
				s = s.substr(2,s.length());
				make.y_offset = std::stof(s);
			} else if (s.find("z=") == 0) {
				s = s.substr(2,s.length());
				make.z_offset = std::stof(s);
			} else if (s.find("specular=") == 0) {
				s = s.substr(9,s.length());
				make.specular_shininess = std::stof(s);
			} else if (s.find("restitution=") == 0) {
				s = s.substr(12,s.length());
				make.restitution = std::stof(s);
			} else if (s.find("mass=") == 0) {
				s = s.substr(5,s.length());
				make.mass = std::stof(s);
			} else if (s.find("radius=") == 0) {
				s = s.substr(7,s.length());
				make.radius = std::stof(s);
			} else if (s.find("vecx=") == 0) {
				s = s.substr(5,s.length());
				make.x_phys = std::stof(s);
			} else if (s.find("vecy=") == 0) {
				s = s.substr(5,s.length());
				make.y_phys = std::stof(s);
			} else if (s.find("vecz=") == 0) {
				s = s.substr(5,s.length());
				make.z_phys = std::stof(s);
			} else if (s.find("lifetime=") == 0) {
				s = s.substr(9,s.length());
				make.lifetime = std::stoi(s);
			} else if (s.find("shape_type=") == 0) {
				make.shape_type = s.substr(11,s.length());
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
    ambient_lighting = 0.6;
    diffuse_lighting = 1.0;
    specular_lighting = 1.0;
    spotlight_angle = 0.5;
}

Graphics::~Graphics()
{

  genlist.clear();

  objlist.clear();

}

bool Graphics::Initialize(int width, int height, std::string v, std::string f, Physics* PhysStruct)
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


  //Create the objects
	objlist.clear();

  //Iterate through the list of object information, create the objects with their attributes, and push them into an object list
	for (int iter = 0; iter < (int) genlist.size(); iter++)
	{
    Object temp(genlist[iter].scene_path,
		genlist[iter].mesh_index,
                genlist[iter].texture_path,
		genlist[iter].x_offset,
		genlist[iter].y_offset,
		genlist[iter].z_offset,
		genlist[iter].specular_shininess,
		genlist[iter].restitution,
		genlist[iter].lifetime);

		objlist.push_back(temp);
	}

	for (int iter = 0; iter < (int) genlist.size(); iter++)
	{
		//std::cout << genlist[iter].name << std::endl;

		if (genlist[iter].shape_type.compare("box") == 0) {
			objlist[iter].rigidBody = PhysStruct->add_box(object_search_by_name(genlist[iter].name), genlist[iter].mass,
				btVector3(genlist[iter].x_phys,genlist[iter].y_phys,genlist[iter].z_phys));
		} else if (genlist[iter].shape_type.compare("sphere") == 0) {
			objlist[iter].rigidBody = PhysStruct->add_sphere(object_search_by_name(genlist[iter].name), genlist[iter].mass, genlist[iter].radius);
		} else if (genlist[iter].shape_type.compare("cylinder") == 0) {
			objlist[iter].rigidBody = PhysStruct->add_cylinder(object_search_by_name(genlist[iter].name), genlist[iter].mass,
				btVector3(genlist[iter].x_phys,genlist[iter].y_phys,genlist[iter].z_phys));
		} else {
			objlist[iter].rigidBody = PhysStruct->add_object(object_search_by_name(genlist[iter].name));
		}
	}
  
  // Set up the shaders
  m_shader = new Shader();
  if(!m_shader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!m_shader->AddShader(GL_VERTEX_SHADER, "../assets/shaders/vertex_pa9_p.txt"))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_shader->AddShader(GL_FRAGMENT_SHADER, "../assets/shaders/fragment_pa9_p.txt"))
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

  // alt shader
  m_alt_shader = new Shader();
  if (!m_alt_shader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!m_alt_shader->AddShader(GL_VERTEX_SHADER, "../assets/shaders/vertex_pa9_g.txt"))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_alt_shader->AddShader(GL_FRAGMENT_SHADER, "../assets/shaders/fragment_pa9_g.txt"))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  if(!m_alt_shader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

  shade_use = true;

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}

void Graphics::Update(unsigned int dt)
{
  // Update the objects
	for (int iter = 0; iter < (int) objlist.size(); iter++)
	{
		objlist[iter].Update(dt);
	}
}

void Graphics::Render()
{
  //clear the screen
  glClearColor(0.02, 0.06, 0.12, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Shader* use;

  // Start the correct program
  if (shade_use) {
    m_shader->Enable();
    use = m_shader;
  } else {
    m_alt_shader->Enable();
    use = m_alt_shader;
  }

  // Locate the projection matrix in the shader
  m_projectionMatrix = use->GetUniformLocation("projectionMatrix");
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_projectionMatrix not found\n");
  }

  // Locate the view matrix in the shader
  m_viewMatrix = use->GetUniformLocation("viewMatrix");
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_viewMatrix not found\n");
  }

  // Locate the model matrix in the shader
  m_modelMatrix = use->GetUniformLocation("modelMatrix");
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_modelMatrix not found\n");
  }

  m_light_position = use->GetUniformLocation("light_position");
  if (m_light_position == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_light_position_one not found\n");
  }

  m_light_color = use->GetUniformLocation("light_color");
  if (m_light_color == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_light_color not found\n");
  }

  m_mat_shininess = use->GetUniformLocation("mat_shininess");
  if (m_mat_shininess == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_mat_shininess not found\n");
  }

  m_spotlight_angle = use->GetUniformLocation("spotlight_angle");
  if (m_spotlight_angle == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_spotlight_angle not found\n");
  }

  m_ambient_lighting = use->GetUniformLocation("ambient_lighting");
  if (m_ambient_lighting == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_ambient_lighting not found\n");
  }

  m_diffuse_lighting = use->GetUniformLocation("diffuse_lighting");
  if (m_diffuse_lighting == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_diffuse_lighting not found\n");
  }

  m_specular_lighting = use->GetUniformLocation("specular_lighting");
  if (m_specular_lighting == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_specular_lighting not found\n");
  }

  m_camera_position = use->GetUniformLocation("camera_positoin");
  if (m_camera_position == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_camera_position not found\n");
  }

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

  float light_pos[12], light_color[16];
  for (int y = 0; y < 12; y++) {
    light_pos[y] = 0.0;
  }
  for (int y = 0; y < 16; y++) {
    light_color[y] = 1.0;
  }
  //
  light_pos[0] = -160.0;
  light_pos[1] = 85.0;
  light_pos[2] = -100.0;

  light_pos[3] = 160.0;
  light_pos[4] = 85.0;
  light_pos[5] = 100.0;

  light_pos[6] = -150.0;
  light_pos[7] = 30.0;
  light_pos[8] = -20.0;

  light_pos[9] = 0.0;
  light_pos[10] = 50.0;
  light_pos[11] = 30.0;

  //0-3 [Other Upper]
  light_color[0] = 0.5;
  //4-7 [Upper]
  light_color[4] = 0.2;
  light_color[5] = 0.6;
  light_color[6] = 0.8;
  //8-11 [Pachinko]
  light_color[9] = 0.3;
  light_color[10] = 0.5;
  //12-15 [Center]
  light_color[13] = 0.9;
  light_color[14] = 0.3;

  glUniform3fv( m_light_position, 4, light_pos );
  glUniform4fv( m_light_color, 4, light_color );

  glUniform3fv( m_light_position, 1, light_pos );
  glUniform1fv( m_spotlight_angle, 1, &spotlight_angle );

  glUniform1fv( m_ambient_lighting, 1, &ambient_lighting );
  glUniform1fv( m_diffuse_lighting, 1, &diffuse_lighting );
  glUniform1fv( m_specular_lighting, 1, &specular_lighting );

  float camera_position[3];
  camera_position[0] = m_camera->eye_store[0];
  camera_position[1] = m_camera->eye_store[1];
  camera_position[2] = m_camera->eye_store[2];

  glUniform3fv( m_camera_position, 1, camera_position );

  // Render the objects
	for (int iter = 0; iter < (int) objlist.size(); iter++)
	{
		if (objlist[iter].texture_path.compare("../assets/scenes/invis.png") == 0) {continue;}

		glUniform1fv( m_mat_shininess, 1, &(objlist[iter].specular_shininess));
		glUniformMatrix4fv(m_modelMatrix,1,GL_FALSE,glm::value_ptr(objlist[iter].GetModel()));
		objlist[iter].Render();
	}
//Menu camera



  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
  }
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

