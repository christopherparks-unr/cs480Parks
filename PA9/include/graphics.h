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
	int mesh_index;
        std::string texture_path;
	std::string parent;
	float x_offset;
	float y_offset;
	float z_offset;
};


class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height, std::string v1, std::string f1, std::string v2, std::string f2);
    void Update(unsigned int dt);
    void Render();

	std::vector<Object> objlist;
	Object* object_search_by_name(std::string findme);
        bool use_vertex_lighting;
	Shader *m_shader;

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

    GLint m_lightposition_one;
    GLint m_materialshininess;
};

#endif /* GRAPHICS_H */
