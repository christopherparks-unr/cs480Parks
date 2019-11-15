#include "object.h"
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <bullet/btBulletDynamicsCommon.h>

std::vector<Scene> my_scenelist;

std::string load_MTL(std::string filename, int meshIndex)
{
  std::string true_filename;
  true_filename = filename.substr(0,filename.length()-3);
  true_filename += "mtl";
  
	std::string asset_file = filename;
	for (int iter = (int) filename.length()-1; iter >= 0; iter--) {
		if (filename[iter] == '/') {
			break;
		}
		asset_file.pop_back();
	}

  std::filebuf fb;
	std::string empty;
	empty.clear();
  if (!fb.open(true_filename,std::ios_base::in)) {
    return "../assets/scenes/default.jpg";
  }
  
  std::string line;
  std::string line_extract;

	bool within = false;

  std::istream reader(&fb);
  int mesh_num = -1;
  while (reader) {

    line.clear();
    char get;
    while (reader.get(get)) {
      if (get == '\n') {
				break;
      } else {
				line.push_back(get);
      }
    }

    if (line[0] == '\t') {line = line.substr(1,std::string::npos);}
    
		if (line.find("newmtl ") == 0)
                {
                  mesh_num++;
                  if(meshIndex == mesh_num)
                  {
                    within = true;
                  }

		}

    if (within && line.find("map_Kd ") == 0) {
			line_extract = asset_file+line.substr(7,std::string::npos);
  		fb.close();
			return line_extract;
    }
  }

  fb.close();
  return "../assets/scenes/default.jpg";
}

//Imports the file from the scene_path argument and creates a vector of meshes from it
Scene scene_from_assimp(std::string filename, int meshIndex, std::string textureOverride)
{
	Assimp::Importer importer;
	const aiScene *readScene = importer.ReadFile(filename,aiProcess_Triangulate | aiProcess_GenSmoothNormals); // | aiProcess_JoinIdenticalVertices

	Scene createdScene;
	createdScene.meshes.clear();
        createdScene.supername = filename;
	
	Mesh wantedMesh;
	Vertex read;

  //Iterate from the meshes in the scene
        
	for (int iter = 0; iter < (int) readScene->mNumMeshes; iter++)
	{
		wantedMesh.name = readScene->mMeshes[iter]->mName.C_Str();
		
                if(textureOverride.length() == 0 || iter != meshIndex)
                {
		  std::string texturepath = load_MTL(filename,iter);

		  if (texturepath.length() != 0) {
			  wantedMesh.image_data = stbi_load(texturepath.c_str(),&(wantedMesh.image_x),
						&(wantedMesh.image_y),&(wantedMesh.image_c),4);
		  } else {
			  wantedMesh.image_data = NULL;
		  }
                }
                else
                {
                  wantedMesh.image_data = stbi_load(textureOverride.c_str(),&(wantedMesh.image_x),
						&(wantedMesh.image_y),&(wantedMesh.image_c),4);
                }

		wantedMesh.vertex_data.clear();

    //Get vertex info from the mesh
		for (int verts = 0; verts < (int) (readScene->mMeshes[iter])->mNumVertices; verts++)
		{
			read.vertex[0] = (readScene->mMeshes[iter])->mVertices[verts].x;
			read.vertex[1] = (readScene->mMeshes[iter])->mVertices[verts].y;
			read.vertex[2] = (readScene->mMeshes[iter])->mVertices[verts].z;
			if ((readScene->mMeshes[iter])->HasTextureCoords(0)) {
				read.coord[0] = (readScene->mMeshes[iter])->mTextureCoords[0][verts].x;
				read.coord[1] = (readScene->mMeshes[iter])->mTextureCoords[0][verts].y;
			}
			read.norm[0] = (readScene->mMeshes[iter])->mNormals[verts].x;
			read.norm[1] = (readScene->mMeshes[iter])->mNormals[verts].y;
			read.norm[2] = (readScene->mMeshes[iter])->mNormals[verts].z;

			wantedMesh.vertex_data.push_back(read);
		}

		wantedMesh.indice_data.clear();

    //Get index info from the mesh
		for (int faces = 0; faces < (int) (readScene->mMeshes[iter])->mNumFaces; faces++)
		{
			//ASSUMES triangle conversion worked

			wantedMesh.indice_data.push_back( (readScene->mMeshes[iter])->mFaces[faces].mIndices[0] );
			wantedMesh.indice_data.push_back( (readScene->mMeshes[iter])->mFaces[faces].mIndices[1] );
			wantedMesh.indice_data.push_back( (readScene->mMeshes[iter])->mFaces[faces].mIndices[2] );
		}

		createdScene.meshes.push_back(wantedMesh);
	}
  //Return the Scene struct, which has a name and a vector of Mesh structs, which contains the vertex and index information plus a name
	return createdScene;
}

//Finds the scene object from the list of scenes (searched by path)
Scene find_scene_from_path(std::string input, int mesh_index, std::string textureOverride)
{

	Scene make = scene_from_assimp(input, mesh_index, textureOverride);
  //my_scenelist is the list of Scenes
	my_scenelist.push_back(make);
	return make;
}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
  my_scenelist.clear();
}

Object::Object(std::string s_p, int m_i, std::string t_p, float x, float y, float z, float sp_sh, float rest)
{
  scene_path = s_p;
  mesh_index = m_i;
  texture_path = t_p;
  x_offset = x;
  y_offset = y;
  z_offset = z;
  specular_shininess = sp_sh;
  restitution = rest;


	Scene currentScene = find_scene_from_path(scene_path, mesh_index, texture_path);

	//int which = currentScene.search_mesh_by_index(mesh_index);

	if (mesh_index < 0 || mesh_index >= (int)currentScene.meshes.size()) {
		//printf("Invalid mesh file selected!\n");
	}

	Vertices = currentScene.meshes[mesh_index].vertex_data;
	Indices = currentScene.meshes[mesh_index].indice_data;

	//printf("Successfully loaded %s with mesh number %i. It has %i non-unique vertices and %i non-unique indices\n", scene_path.c_str(), mesh_index, (int)Vertices.size(), (int)Indices.size());

  model = glm::mat4(1.0f);

  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

	has_texture = false;

	if (currentScene.meshes[mesh_index].image_data != NULL) {	
		has_texture = true;
		glGenTextures(1, &TX);
		glBindTexture(GL_TEXTURE_2D,TX);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,currentScene.meshes[mesh_index].image_x,currentScene.meshes[mesh_index].image_y,
									0,GL_RGBA,GL_UNSIGNED_BYTE,currentScene.meshes[mesh_index].image_data);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	}
	
}

void Object::Update(unsigned int dt)
{
  
}

glm::mat4 Object::GetModel()
{
  return model;
}

void Object::SetModel(btScalar m[16])
{
  //glm::mat4 fixer;
  for (int iter = 0; iter < 16; iter++) {
    model[iter/4][iter%4] = m[iter];
    //fixer[iter/4][iter%4] = 0.0;
  }

}

void Object::SetModel(glm::mat4 with)
{
	model = with;
}

void Object::Render()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,TX);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,norm));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,coord));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
}

