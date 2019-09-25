#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"

class Object
{
  public:
    Object(std::string obj, float s, float r, float r_m, float o_m, Object* p);
    ~Object();
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

    float rotation_angle;
    float orbit_angle;

    float scale;
    float radius;
    float rotation_angle_mod;
    float orbit_angle_mod;
    Object *parent;
    
    float rotation_angle_paused;
    float orbit_angle_paused;

    std::vector<Vertex> Vertices;
    std::vector<int> Indices;

  private:
    glm::mat4 model;
    GLuint VB;
    GLuint IB;
};

#endif /* OBJECT_H */
