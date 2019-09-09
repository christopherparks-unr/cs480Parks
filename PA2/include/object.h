#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"

class Object
{
  public:
    Object(float r);
    ~Object();
    void Update(unsigned int dt);
    void Render();

    glm::mat4 GetModel();

    float rotation_angle;
    float orbit_angle;
    float rotation_angle_mod;
    float orbit_angle_mod;
    float radius;
    
    float rotation_angle_paused;
    float orbit_angle_paused;

  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
};

#endif /* OBJECT_H */
