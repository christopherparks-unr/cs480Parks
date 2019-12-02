#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <fstream>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"
#include "physics.h"

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
        float specular_shininess;
	float restitution;

	std::string shape_type;
	int lifetime;
	float x_phys;
	float y_phys;
	float z_phys;
	float radius;
	float mass;
};


class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height, std::string v, std::string f, Physics* ps);
    void Update(unsigned int dt);
    void Render(Physics* PhysStruct = nullptr);

	std::vector<Object> objlist;
	Object* object_search_by_name(std::string findme);

    bool shade_use;
    float spotlight_angle;
    float ambient_lighting;
    float diffuse_lighting;
    float specular_lighting;
    Camera *m_camera;


  private:
    std::string ErrorString(GLenum error);

    Shader *m_shader;
    Shader *m_alt_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

    GLint m_light_position;
    GLint m_mat_shininess;

    GLint m_spotlight_angle;
    GLint m_spotlight_dir;
    GLint m_ambient_lighting;
    GLint m_diffuse_lighting;
    GLint m_specular_lighting;
};

#endif /* GRAPHICS_H */
