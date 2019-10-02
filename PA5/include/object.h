#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/color4.h>

struct Mesh
{
	std::string name;
	std::vector<Vertex> vertex_data;
	std::vector<unsigned int> indice_data;
};

struct Scene
{
	std::string supername;
	std::vector<Mesh> meshes;

	int search_mesh_by_name(std::string input) {
		for (int iter = 0; iter < (int) meshes.size(); iter++)
		{
			if (meshes[iter].name.compare(input) == 0) {
				return iter;
			}
		}
		return -1;
	}
};

class Object
{
  public:
    ~Object();
Object(std::string s_p, std::string m_n, float s, float r, float r_m, float o_m, Object* p);
    void Update(unsigned int dt);
    void Render();

    glm::mat4 GetModel();
    float GetScale();
    float GetRadius();
    float GetRotationAngle();
    float GetRotating();
    float GetOrbiting();
    float GetOrbitAngle();
    Object* GetParent();
    void SetParent(Object*);

    float rotation_angle;
    float orbit_angle;

    std::string scene_path;
    std::string mesh_name;
    float scale;
    float radius;
    float rotation_angle_mod;
    float orbit_angle_mod;
    Object *parent;
    
    float rotation_angle_paused;
    float orbit_angle_paused;

    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;

  private:
    glm::mat4 model;
    GLuint VB;
    GLuint IB;
};

#endif /* OBJECT_H */
