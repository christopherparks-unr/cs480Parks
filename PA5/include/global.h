#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>

#include "graphics_headers.h"

#ifndef GLOBAL_HEADER_
#define GLOBAL_HEADER_

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

struct Attr
{
	std::string scene_path;
	std::string mesh_name;
	float scale;
	float rot_spd;
	float orbit_rad;
	float orbit_spd;
	int parent_index;
};

	std::vector< Scene > scene_vector;
	std::vector< std::vector< std::string > > object_vector;

	std::string cur_name;
	std::string cur_obj_path;
	std::string cur_parent;
	std::string cur_scale;
	std::string cur_rot_spd;
	std::string cur_orbit_rad;
	std::string cur_orbit_spd;

	std::string replaceOccurrences(std::string& s, const std::string& toReplace, const std::string& replaceWith);
	Scene scene_from_assimp(std::string filename);
#endif
