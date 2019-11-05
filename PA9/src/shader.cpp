#include "shader.h"
#include <fstream>
Shader::Shader()
{
  m_shaderProg_vertex = 0;
  m_shaderProg_fragment = 0;
}

Shader::~Shader()
{
  for (std::vector<GLuint>::iterator it = m_shaderObjList_vertex.begin() ; it != m_shaderObjList_vertex.end() ; it++)
  {
    glDeleteShader(*it);
  }

  for (std::vector<GLuint>::iterator it = m_shaderObjList_fragment.begin() ; it != m_shaderObjList_fragment.end() ; it++)
  {
    glDeleteShader(*it);
  }

  if (m_shaderProg_vertex != 0)
  {
    glDeleteProgram(m_shaderProg_vertex);
    m_shaderProg_vertex = 0;
  }
  if (m_shaderProg_fragment != 0)
  {
    glDeleteProgram(m_shaderProg_fragment);
    m_shaderProg_fragment = 0;
  }
}

bool Shader::Initialize()
{
  m_shaderProg_vertex = glCreateProgram();
  m_shaderProg_fragment = glCreateProgram();

  if (m_shaderProg_vertex == 0 || m_shaderProg_fragment == 0) 
  {
    std::cerr << "Error creating shader program\n";
    return false;
  }

  return true;
}

// Use this method to add shaders to the program. When finished - call finalize()
bool Shader::AddShader(GLenum ShaderType, std::string path, bool vertex_lighting)
{ 
  
  std::string s;
  //Buffer string when reading from file
  std::string line;
  //Input stream class for reading from file
  std::ifstream shader_info;
  //Open the shader file, path determined by function call in graphics.cpp
  shader_info.open(path);

  //Read in file, then close
  while(getline(shader_info,line))
  {
    s += line + "\n";
  }
  shader_info.close();
  GLuint ShaderObj = glCreateShader(ShaderType);

  if (ShaderObj == 0) 
  {
    std::cerr << "Error creating shader type " << ShaderType << std::endl;
    return false;
  }

  // Save the shader object - will be deleted in the destructor
  if(vertex_lighting)
  {
    m_shaderObjList_vertex.push_back(ShaderObj);
  }
  else
  {
    m_shaderObjList_fragment.push_back(ShaderObj);
  }

  const GLchar* p[1];
  p[0] = s.c_str();
  GLint Lengths[1] = { (GLint)s.size() };

  glShaderSource(ShaderObj, 1, p, Lengths);

  glCompileShader(ShaderObj);

  GLint success;
  glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

  if (!success) 
  {
    GLchar InfoLog[1024];
    glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
    std::cerr << "Error compiling: " << InfoLog << std::endl;
    return false;
  }
if(vertex_lighting)
{
  glAttachShader(m_shaderProg_vertex, ShaderObj);
}
else
{
  glAttachShader(m_shaderProg_fragment, ShaderObj);
}
  

  return true;
}


// After all the shaders have been added to the program call this function
// to link and validate the program.
bool Shader::Finalize(bool vertex_lighting)
{

  if(vertex_lighting)
  {
    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    glLinkProgram(m_shaderProg_vertex);

    glGetProgramiv(m_shaderProg_vertex, GL_LINK_STATUS, &Success);
    if (Success == 0)
    {
      glGetProgramInfoLog(m_shaderProg_vertex, sizeof(ErrorLog), NULL, ErrorLog);
      std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
      return false;
    }

    glValidateProgram(m_shaderProg_vertex);
    glGetProgramiv(m_shaderProg_vertex, GL_VALIDATE_STATUS, &Success);
    if (!Success)
    {
      glGetProgramInfoLog(m_shaderProg_vertex, sizeof(ErrorLog), NULL, ErrorLog);
      std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
      return false;
    }

    // Delete the intermediate shader objects that have been added to the program
    for (std::vector<GLuint>::iterator it = m_shaderObjList_vertex.begin(); it != m_shaderObjList_vertex.end(); it++)
    {
      glDeleteShader(*it);
    }

    m_shaderObjList_vertex.clear();

    return true;
  }
  else
  {
    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    glLinkProgram(m_shaderProg_fragment);

    glGetProgramiv(m_shaderProg_fragment, GL_LINK_STATUS, &Success);
    if (Success == 0)
    {
      glGetProgramInfoLog(m_shaderProg_fragment, sizeof(ErrorLog), NULL, ErrorLog);
      std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
      return false;
    }

    glValidateProgram(m_shaderProg_fragment);
    glGetProgramiv(m_shaderProg_fragment, GL_VALIDATE_STATUS, &Success);
    if (!Success)
    {
      glGetProgramInfoLog(m_shaderProg_fragment, sizeof(ErrorLog), NULL, ErrorLog);
      std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
      return false;
    }

    // Delete the intermediate shader objects that have been added to the program
    for (std::vector<GLuint>::iterator it = m_shaderObjList_fragment.begin(); it != m_shaderObjList_fragment.end(); it++)
    {
      glDeleteShader(*it);
    }

    m_shaderObjList_fragment.clear();

    return true;
  }
}


void Shader::Enable(bool vertex_lighting)
{
  if(vertex_lighting)
  {
    glUseProgram(m_shaderProg_vertex);
  }
  else
  {
    glUseProgram(m_shaderProg_fragment);
  }
}


GLint Shader::GetUniformLocation(const char* pUniformName, bool vertex_lighting)
{
  GLuint Location;
  if(vertex_lighting)
  {
    Location = glGetUniformLocation(m_shaderProg_vertex, pUniformName);
  }
  else
  {
    Location = glGetUniformLocation(m_shaderProg_fragment, pUniformName);
  }
    
  if (Location == INVALID_UNIFORM_LOCATION) {
      fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
  }

    return Location;
}
