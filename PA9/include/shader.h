#ifndef SHADER_H
#define SHADER_H

#include <vector>

#include "graphics_headers.h"

class Shader
{
  public:
    Shader();
    ~Shader();
    bool Initialize();
    void Enable(bool vertex_lighting);
    bool AddShader(GLenum ShaderType, std::string path, bool vertex_lighting);
    bool Finalize(bool vertex_lighting);
    GLint GetUniformLocation(const char* pUniformName, bool vertex_lighting);

    GLuint m_shaderProg_vertex;
    GLuint m_shaderProg_fragment;
    std::vector<GLuint> m_shaderObjList_vertex;
    std::vector<GLuint> m_shaderObjList_fragment;

  private:
    
};

#endif  /* SHADER_H */
