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
};



class Object
{
  public:
    ~Object();
    Object();
Object(std::string s_p, int m_i, std::string t_p, float x, float y, float z, float sp_sh, float rest, int lt);
    void Update(unsigned int dt);
    void Render();

    glm::mat4 GetModel();
    void SetModel(btScalar m[16]);
    void SetModel(glm::mat4 with);

    float rotation_angle;
    float orbit_angle;

    std::string scene_path;
    int mesh_index;
    std::string texture_path;
    float x_offset;
    float y_offset;
    float z_offset;
    float specular_shininess;
    float restitution;
    int lifetime;

    btRigidBody* rigidBody;

    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;

    bool has_texture;
    glm::mat4 model;

  private:
    GLuint VB;
    GLuint IB;
    GLuint TX;
};

#endif /* OBJECT_H */
