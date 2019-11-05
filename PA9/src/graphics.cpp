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
        make.x_offset = 0.0;
        make.y_offset = 0.0;
        make.z_offset = 0.0;

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
			} else if(s.find("x=") == 0) {
        s = s.substr(2,s.length());
        make.x_offset = std::stof(s);
      } else if(s.find("y=") == 0) {
        s = s.substr(2,s.length());
        make.y_offset = std::stof(s);
      } else if(s.find("z=") == 0) {
        s = s.substr(2,s.length());
        make.z_offset = std::stof(s);
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
  use_vertex_lighting = false;
}

Graphics::~Graphics()
{

  genlist.clear();

  objlist.clear();

}

bool Graphics::Initialize(int width, int height, std::string v1, std::string f1, std::string v2, std::string f2)
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
    Object temp(genlist[iter].scene_path,genlist[iter].mesh_index,
                genlist[iter].texture_path,
                genlist[iter].x_offset,
                genlist[iter].y_offset,
                genlist[iter].z_offset
		);

		objlist.push_back(temp);
	}


  // Set up the shaders
  m_shader = new Shader();
  if(!m_shader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader, now with path support
  if(!m_shader->AddShader(GL_VERTEX_SHADER, v1, true))
  {
    printf("Vertex Shader 1 failed to Initialize\n");
    return false;
  }

  // Add the fragment shader, now with path support
  if(!m_shader->AddShader(GL_FRAGMENT_SHADER, f1, true))
  {
    printf("Fragment Shader 1 failed to Initialize\n");
    return false;
  }

  // Add the vertex shader, now with path support
  if(!m_shader->AddShader(GL_VERTEX_SHADER, v2, false))
  {
    printf("Vertex Shader 2 failed to Initialize\n");
    return false;
  }

  // Add the fragment shader, now with path support
  if(!m_shader->AddShader(GL_FRAGMENT_SHADER, f2, false))
  {
    printf("Fragment Shader 2 failed to Initialize\n");
    return false;
  }

  // Connect the program
  if(!m_shader->Finalize(true))
  {
    printf("Program to Finalize 1\n");
    return false;
  }

  if(!m_shader->Finalize(false))
  {
    printf("Program to Finalize 2\n");
    return false;
  }

  // Locate the projection matrix in the shader
  m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix", use_vertex_lighting);
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrix = m_shader->GetUniformLocation("viewMatrix", use_vertex_lighting);
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  m_modelMatrix = m_shader->GetUniformLocation("modelMatrix", use_vertex_lighting);
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_modelMatrix not found\n");
    return false;
  }

  m_lightposition_one = m_shader->GetUniformLocation("light_position", use_vertex_lighting);
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_lightposition_one not found\n");
  }

  m_materialshininess = m_shader->GetUniformLocation("material_shininess", use_vertex_lighting);
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_lightposition_one not found\n");
  }

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}

void Graphics::Update(unsigned int dt)
{
  
}

void Graphics::Render()
{
  //clear the screen
  glClearColor(0.0, 0.2, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Start the correct program
  m_shader->Enable(use_vertex_lighting);

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

  float lightboi[4];
  lightboi[0] = 5.0;
  lightboi[1] = -5.0;
  lightboi[2] = 0.0;
  lightboi[3] = 0.0;
  glUniform4fv( m_lightposition_one, 1, lightboi );

  float matshiny = 10.0;
  glUniform1fv( m_materialshininess, 1, &matshiny );

  // Render the objects
	for (int iter = 0; iter < (int) objlist.size(); iter++)
	{
		glUniformMatrix4fv(m_modelMatrix,1,GL_FALSE,glm::value_ptr(objlist[iter].GetModel()));
		objlist[iter].Render();
	}

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

