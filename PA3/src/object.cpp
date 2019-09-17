#include "object.h"
#include <cmath>

Object::Object(float s, float r, float r_m, float o_m, Object* p)
{  
  /*
    # Blender File for a Cube
    o Cube
    v 1.000000 -1.000000 -1.000000
    v 1.000000 -1.000000 1.000000
    v -1.000000 -1.000000 1.000000
    v -1.000000 -1.000000 -1.000000
    v 1.000000 1.000000 -0.999999
    v 0.999999 1.000000 1.000001
    v -1.000000 1.000000 1.000000
    v -1.000000 1.000000 -1.000000
    s off
    f 2 3 4
    f 8 7 6
    f 1 5 6
    f 2 6 7
    f 7 8 4
    f 1 4 8
    f 1 2 4
    f 5 8 6
    f 2 1 6
    f 3 2 7
    f 3 7 4
    f 5 1 8
  */

  Vertices = {
    {{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}},
    {{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
    {{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
    {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}},
    {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}},
    {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}},
    {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
    {{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}}
  };

  Indices = {
    2, 3, 4,
    8, 7, 6,
    1, 5, 6,
    2, 6, 7,
    7, 8, 4,
    1, 4, 8,
    1, 2, 4,
    5, 8, 6,
    2, 1, 6,
    3, 2, 7,
    3, 7, 4,
    5, 1, 8
  };

  // The index works at a 0th index
  for(unsigned int i = 0; i < Indices.size(); i++)
  {
    Indices[i] = Indices[i] - 1;
  }

  rotation_angle = 0.0f;
  orbit_angle = 0.0f;


  scale = s;
  radius = r;
  rotation_angle_mod = r_m;
  orbit_angle_mod = o_m;
  parent = p;

  rotation_angle_paused = 1.0f;
  orbit_angle_paused = 1.0f;

  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

void Object::Update(unsigned int dt)
{
  //Default to identity matrix
  model = glm::mat4(1.0f);
  if(parent != nullptr)
  {
    //Start with the parent's model matrix to bring in the position, if applicable
    //It also brings in the scale and rotation, so we need to account for that
    model = parent->GetModel();
  }

  //Scale object
  if(parent != nullptr)
  {
    //If the object has a parent, subtract the parent's scale, which is built into the model matrix
    model = glm::scale(model, glm::vec3(scale - (parent->GetScale()), scale - (parent->GetScale()), scale - (parent->GetScale())));
  }
  else
  {
    model = glm::scale(model, glm::vec3(scale, scale, scale));
  }

  //Rotation calculations
  if(parent != nullptr)
  {
    //If the object has a parent, subtract the parent's rotation, which is built into the model matrix
    rotation_angle += (rotation_angle_paused * dt * M_PI/1000 * rotation_angle_mod) - (parent->rotation_angle_paused * dt * M_PI/1000 * parent->rotation_angle_mod); //Rotation angle
  }
  else
  {
    rotation_angle += rotation_angle_paused * dt * M_PI/1000 * rotation_angle_mod;
  }
  
  //Orbit calculations
  orbit_angle += orbit_angle_paused * dt * M_PI/1000 * orbit_angle_mod; //Orbit angle

  //Orbit
  //Translate along the X and Z axes by 'radius' amount, relative to orbit angle
  model = glm::translate(model, glm::vec3(radius * sin(orbit_angle),1.0f, radius * cos(orbit_angle)));
  
  //Rotation
  model = glm::rotate(model, (rotation_angle), glm::vec3(0.0, 1.0, 0.0));
}

glm::mat4 Object::GetModel()
{
  return model;
}

float Object::GetScale()
{
  return scale;
}

float Object::GetRadius()
{
  return radius;
}

float Object::GetRotationAngle()
{
  return rotation_angle;
}

float Object::GetRotating()
{
  return rotation_angle_paused;
}

float Object::GetOrbitAngle()
{
  return orbit_angle;
}

float Object::GetOrbiting()
{
  return orbit_angle_paused;
}

Object* Object::GetParent()
{
  return parent;
}

void Object::Render()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

