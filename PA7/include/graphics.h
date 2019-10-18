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
	float scale;
	float radius;
	float rotation_angle_mod;
        float rotation_offset;
	float orbit_angle_mod;
        float orbit_offset;
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
    Camera *m_camera;
	
    Camera *m_menu_camera;
    Object* menu_obj[5];
    int simulation_speed;
    bool menu_draw;
    
	Object* meme;
	bool meme_draw;

  private:
    std::string ErrorString(GLenum error);

    
    Shader *m_shader;
    Shader *m_menu_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
};

#endif /* GRAPHICS_H */
