#ifndef GLOBAL_HEADER_
#define GLOBAL_HEADER_
	std::string replaceOccurrences(std::string& s, const std::string& toReplace, const std::string& replaceWith);
	std::vector<Object> object_vector;
	std::string cur_name;
	std::string cur_obj_path;
	std::string cur_parent;
	float cur_scale;
	float cur_rot_spd;
	float cur_orbit_rad;
	float cur_orbit_spd;
#endif
