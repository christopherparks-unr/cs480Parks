#include "object.h"
#include <cmath>

std::vector<Scene> my_scenelist;

//Imports the file from the scene_path argument and creates a vector of meshes from it
Scene scene_from_assimp(std::string filename)
{
	Assimp::Importer importer;
	const aiScene *readScene = importer.ReadFile(filename,aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

	Scene make;
	make.meshes.clear();

	std::string otherstring(filename);
	make.supername = otherstring;
	
	Mesh submake;
	Vertex read;

  //Iterate from the meshes in the scene
	for (int iter = 0; iter < (int) readScene->mNumMeshes; iter++)
	{
		std::string newstring(readScene->mMeshes[iter]->mName.C_Str());
		submake.name = newstring;

		submake.vertex_data.clear();

    //Get vertex info from the mesh
		for (int verts = 0; verts < (int) (readScene->mMeshes[iter])->mNumVertices; verts++)
		{
			read.vertex[0] = (readScene->mMeshes[iter])->mVertices[verts].x;
			read.vertex[1] = (readScene->mMeshes[iter])->mVertices[verts].y;
			read.vertex[2] = (readScene->mMeshes[iter])->mVertices[verts].z;
			read.color[0] = 1.0;
			read.color[1] = 1.0;
			read.color[2] = 1.0;

			submake.vertex_data.push_back(read);
		}

		submake.indice_data.clear();

    //Get index info from the mesh
		for (int faces = 0; faces < (int) (readScene->mMeshes[iter])->mNumFaces; faces++)
		{
			//ASSUMES triangle conversion worked

			submake.indice_data.push_back( (readScene->mMeshes[iter])->mFaces[faces].mIndices[0] );
			submake.indice_data.push_back( (readScene->mMeshes[iter])->mFaces[faces].mIndices[1] );
			submake.indice_data.push_back( (readScene->mMeshes[iter])->mFaces[faces].mIndices[2] );
		}

		make.meshes.push_back(submake);
	}
  //Return the Scene struct, which has a name and a vector of Mesh structs, which contains the vertex and index information plus a name
	return make;
}

//Finds the scene object from the list of scenes (searched by path)
Scene find_scene_from_path(std::string input)
{
  //If the scene has already been loaded, use that instead of loading it again
	for (int iter = 0; iter < (int) my_scenelist.size(); iter++)
	{
		if (my_scenelist[iter].supername.compare(input) == 0) {
			return my_scenelist[iter];
		}
	}
  //Else load the scene in from the scene_path
	Scene make = scene_from_assimp(input);
  //my_scenelist is the list of Scenes
	my_scenelist.push_back(make);
	return make;
}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

Object::Object(std::string s_p, std::string m_n, float s, float r, float r_m, float o_m, Object* p)
{
	Scene from = find_scene_from_path(s_p);

	int which = from.search_mesh_by_name(m_n);

	if (which == -1) {
		printf("Invalid mesh file selected!\n");
	}

	Vertices = from.meshes[which].vertex_data;
	Indices = from.meshes[which].indice_data;

	printf("Successfully loaded %s with %i non-unique vertices and %i non-unique indices\n", m_n.c_str(), (int)Vertices.size(), (int)Indices.size());

  rotation_angle = 0.0f;
  orbit_angle = 0.0f;

  scene_path = s_p;
  mesh_name = m_n;
  scale = s;
  radius = r;
  rotation_angle_mod = r_m;
  orbit_angle_mod = o_m;
  parent = p;

  model = glm::mat4(1.0f);

  rotation_angle_paused = 1.0f;
  orbit_angle_paused = 1.0f;

  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
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
    model = glm::scale(model, glm::vec3(scale / (parent->GetScale()), scale / (parent->GetScale()), scale / (parent->GetScale())));
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

void Object::SetParent(Object* what)
{
  parent = what;
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

