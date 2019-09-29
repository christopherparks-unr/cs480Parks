#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>

#ifndef GLOBAL_HEADER_
#define GLOBAL_HEADER_
	std::vector< const aiScene* > scene_vector;
	std::vector< std::vector< std::string > > object_vector;
	std::string cur_name;
	std::string cur_obj_path;
	std::string cur_parent;
	std::string cur_scale;
	std::string cur_rot_spd;
	std::string cur_orbit_rad;
	std::string cur_orbit_spd;

	std::string replaceOccurrences(std::string& s, const std::string& toReplace, const std::string& replaceWith);
#endif
