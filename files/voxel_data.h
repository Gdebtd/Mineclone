#pragma once
#include "sdl_game.h"

#define radius_chunks 1
#define world_size_chunks 10
#define world_size world_size_chunks * chunk_width
#define chunk_width 16
#define chunk_height 75
#define ground_height 10
#define air_height 15

class voxel_data
{
public:
	float b_s = 20.0f; // block size
	int texture_size_in_blocks = 4;
	float tile_size = 1.f / texture_size_in_blocks;

	vec3 voxel_verts[8] = {
		vec3{0.0, 0.0, 0.0},
		vec3{b_s, 0.0, 0.0},
		vec3{b_s, b_s, 0.0},
		vec3{0.0, b_s, 0.0}, 
		vec3{0.0, 0.0, b_s},
		vec3{b_s, 0.0, b_s},
		vec3{b_s, b_s, b_s},
		vec3{0.0, b_s, b_s}
	};

	vec3 face_normals[6] = {
		vec3{0.0, 0.0, -1.0},
		vec3{0.0, 0.0, 1.0},
		vec3{0.0, 1.0, 0.0},
		vec3{0.0, -1.0, 0.0},
		vec3{-1.0, 0.0, 0.0},
		vec3{1.0, 0.0, 0.0}
	};

	vec3 face_checks[6] = {
		vec3{0.0, 0.0, -b_s},
		vec3{0.0, 0.0, b_s},
		vec3{0.0, b_s, 0.0},
		vec3{0.0, -b_s, 0.0},
		vec3{-b_s, 0.0, 0.0},
		vec3{b_s, 0.0, 0.0}
	};

	int voxel_tris[6][4] = {
		// 0 1 2 2 1 3
		{0, 3, 1, 2}, // back
		{5, 6, 4, 7}, // front
		{3, 7, 2, 6}, // top
		{1, 5, 0, 4}, // bottom
		{4, 7, 0, 3}, // left
		{1, 2, 5, 6} // right
	};

	vec2 voxel_uvs[4] = {
		vec2{0.0f, 0.0f},
		vec2{0.0f, 1.0f},
		vec2{1.0f, 0.0f},
		vec2{1.0f, 1.0f}
	};

	std::vector <vec2> add_texture(int textureID)
	{
		float y = textureID / texture_size_in_blocks;
		float x = textureID - (y * texture_size_in_blocks);
		x *= tile_size;
		y *= tile_size;
		y = 1.f - y - tile_size;
		std::vector <vec2> res(4);
		res[0] = vec2{ x, y };
		res[1] = vec2{ x, y + tile_size };
		res[2] = vec2{ x + tile_size, y };
		res[3] = vec2{ x + tile_size, y + tile_size };
		return res;
	}
};