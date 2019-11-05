#ifndef OBJECT_H
#define OBJECT_H

#include <fstream>

#include <vector>
#include "graphics_headers.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/color4.h>

#include <bullet/btBulletDynamicsCommon.h>

struct Mesh
{
	std::string name;
	std::vector<Vertex> vertex_data;
	std::vector<unsigned int> indice_data;

	unsigned char* image_data;
	int image_x,image_y,image_c;
};

struct Scene
{
	std::string supername;
	std::vector<Mesh> meshes;
/*
	int search_mesh_by_index(int input) {
		for (int iter = 0; iter < (int) meshes.size(); iter++)
		{
			if (meshes[iter].name.compare(input) == 0) {
				return iter;
			}
		}
		return -1;
	}*/
};



class Object
{
  public:
    ~Object();
Object(std::string s_p, int m_i, std::string t_p, float s, float r, float r_m, float r_o, float o_m, float o_o, Object* p);
    void Update(unsigned int dt);
    void Render();

    glm::mat4 GetModel();
    void SetModel(btScalar m[16]);
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
    int mesh_index;
    std::string texture_path;
    float scale;
    float radius;
    float rotation_angle_mod;
    float rotation_offset;
    float orbit_angle_mod;
    float orbit_offset;
    Object *parent;
    
    float rotation_angle_paused;
    float orbit_angle_paused;

    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;

    bool has_texture;

  private:
    glm::mat4 model;
    GLuint VB;
    GLuint IB;
    GLuint TX;
};

#endif /* OBJECT_H */