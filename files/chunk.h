#pragma once
#include "sdl_game.h"
#include "voxel_data.h"

class chunk
{
public:
	voxel_data v_data;
	vec3 c_pos; // chunk pos
	std::vector <vec3> vertices;
	std::vector <vec2> uvs;
	std::vector <vec3> normals;
	int voxel_map[chunk_height][chunk_width][chunk_width];

	std::vector <block_type> blocks = {
		block_type{"air", false, 0, 0, 0, 0, 0, 0},
		block_type{"barrier", false, 0, 0, 0, 0, 0, 0},
		block_type{"bedrock", true, 0, 0, 0, 0, 0, 0},
		block_type{"stone", true, 1, 1, 1, 1, 1, 1},
		block_type{"dirt", true, 2, 2, 2, 2, 2, 2},
		block_type{"grass", true, 3, 3, 4, 2, 3, 3},
		block_type{"sand", true, 5, 5, 5, 5, 5, 5},
		block_type{"sandstone", true, 6, 6, 6, 6, 6, 6},
		block_type{"cobblestone", true, 7, 7, 7, 7, 7, 7},
		block_type{"gravel", true, 8, 8, 8, 8, 8, 8}
	};

	void populate_voxel_map()
	{
		for (int y = 0; y < chunk_height; y++)
		{
			for (int x = 0; x < chunk_width; x++)
			{
				for (int z = 0; z < chunk_width; z++)
				{
					float _x = c_pos.x * chunk_width + x;
					float _y = c_pos.y * chunk_height + y;
					float _z = c_pos.z * chunk_width + z;
					voxel_map[y][x][z] = get_voxel(vec3{ _x, _y, _z });
				}
			}
		}
	}

	void create_mesh_data()
	{
		vertices = std::vector <vec3>(0);
		uvs = std::vector <vec2>(0);
		normals = std::vector <vec3>(0);
		for (int y = 0; y < chunk_height; y++)
		{
			for (int x = 0; x < chunk_width; x++)
			{
				for (int z = 0; z < chunk_width; z++)
				{
					add_voxel_data_chunk(vec3{ x * v_data.b_s, y * v_data.b_s, z * v_data.b_s });
				}
			}
		}

	}

	bool check_voxel(vec3 pos)
	{
		int x = (int)pos.x;
		int y = (int)pos.y;
		int z = (int)pos.z;
		if (x < 0 || x > chunk_width - 1 || y < 0 || y > chunk_height - 1 || z < 0 || z > chunk_width - 1)
		{
			return blocks[voxel_from_map(vec3{ x + c_pos.x * chunk_width, y + c_pos.y * chunk_height, z + c_pos.z * chunk_width })].is_solid;
		}
		return blocks[voxel_map[y][x][z]].is_solid;
	}

	void add_voxel_data_chunk(vec3 pos)
	{
		for (int p = 0; p < 6; p++)
		{
			float x = (pos.x + v_data.face_checks[p].x) / v_data.b_s;
			float y = (pos.y + v_data.face_checks[p].y) / v_data.b_s;
			float z = (pos.z + v_data.face_checks[p].z) / v_data.b_s;
			if (!check_voxel(vec3{ x, y, z }))
			{
				int blockID = voxel_map[(int)(pos.y / v_data.b_s)][(int)(pos.x / v_data.b_s)][(int)(pos.z / v_data.b_s)];
				if (blockID != 0 && blockID != 1)
				{
					for (int i = 0; i < 4; i++)
					{
						int triangle_index = v_data.voxel_tris[p][i];
						x = pos.x + v_data.voxel_verts[triangle_index].x + c_pos.x * chunk_width * v_data.b_s;
						y = pos.y + v_data.voxel_verts[triangle_index].y + c_pos.y * chunk_height * v_data.b_s;
						z = pos.z + v_data.voxel_verts[triangle_index].z + c_pos.z * chunk_width * v_data.b_s;
						vertices.push_back(vec3{ x, y, z });
						normals.push_back(v_data.face_normals[p]);
					}
					std::vector <vec2> tex_uv = v_data.add_texture(blocks[blockID].get_textureID(p));
					uvs.push_back(tex_uv[0]);
					uvs.push_back(tex_uv[1]);
					uvs.push_back(tex_uv[2]);
					uvs.push_back(tex_uv[3]);
				}
			}
		}
	}

	void create_mesh()
	{
		int cnt = 0;
		glColor3f(1, 1, 1);
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < vertices.size(); i++)
		{
			if (cnt == 4) cnt = 0;
			if (cnt == 3)
			{
				glNormal3f(normals[i - 1].x, normals[i - 1].y, normals[i - 1].z);
				glTexCoord2f(uvs[i - 1].x, uvs[i - 1].y);
				glVertex3f(vertices[i - 1].x, vertices[i - 1].y, vertices[i - 1].z);
				glNormal3f(normals[i - 2].x, normals[i - 2].y, normals[i - 2].z);
				glTexCoord2f(uvs[i - 2].x, uvs[i - 2].y);
				glVertex3f(vertices[i - 2].x, vertices[i - 2].y, vertices[i - 2].z);
			}
			glNormal3f(normals[i].x, normals[i].y, normals[i].z);
			glTexCoord2f(uvs[i].x, uvs[i].y);
			glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
			cnt++;
		}
		glEnd();
	}
};

extern chunk chunks[world_size_chunks][world_size_chunks];