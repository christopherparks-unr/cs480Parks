#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <fstream>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"

struct gen_object
{
	std::string name;
	std::string scene_path;
	std::string mesh_name;
	std::string parent;
	float scale;
	float radius;
	float rotation_angle_mod;
	float orbit_angle_mod;
};


class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height, std::string v, std::string f);
    void Update(unsigned int dt);
    void Render();

	std::vector<Object> objlist;
	Object* object_search_by_name(std::string findme);

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
};

#endif /* GRAPHICS_H */
