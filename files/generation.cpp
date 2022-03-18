#include "sdl_game.h"
#include "voxel_data.h"
#include "chunk.h"

chunk chunks[world_size_chunks][world_size_chunks];

void spawn_player(player & plr, int x, int z)
{
	chunk ch;
	float y = (noise[x][x] * (chunk_height - air_height) + ground_height + 3) * ch.v_data.b_s;
	plr.x = x * ch.v_data.b_s + 10;
	plr.z = z * ch.v_data.b_s + 10;
	plr.y = y;
}

void init_chunks()
{
	for (int i = 0; i < world_size_chunks; i++)
	{
		for (int j = 0; j < world_size_chunks; j++)
		{
			chunks[i][j].c_pos = vec3{ (float)i, 0, (float)j };
			chunks[i][j].populate_voxel_map();
			chunks[i][j].create_mesh_data();
		}
	}
}

void create_data_chunks(vec3 pos)
{
	chunk ch;
	int curr_x = (int)round(pos.x / (chunk_width * ch.v_data.b_s));
	int curr_z = (int)round(pos.z / (chunk_width * ch.v_data.b_s));
	int x_min = curr_x - 1 <= 0 ? 0 : curr_x - 1;
	int x_max = curr_x + 1 >= world_size_chunks ? world_size_chunks : curr_x + 1;
	int z_min = curr_z - 1 <= 0 ? 0 : curr_z - 1;
	int z_max = curr_z + 1 >= world_size_chunks ? world_size_chunks : curr_z + 1;
	for (int i = x_min; i < x_max; i++)
	{
		for (int j = z_min; j < z_max; j++)
		{
			chunks[i][j].create_mesh_data();
		}
	}
}

void create_chunks(vec3 pos)
{
	chunk ch;
	int curr_x = (int)round(pos.x / (chunk_width * ch.v_data.b_s));
	int curr_z = (int)round(pos.z / (chunk_width * ch.v_data.b_s));
	int x_min = curr_x - radius_chunks <= 0 ? 0 : curr_x - radius_chunks;
	int x_max = curr_x + radius_chunks >= world_size_chunks ? world_size_chunks : curr_x + radius_chunks;
	int z_min = curr_z - radius_chunks <= 0 ? 0 : curr_z - radius_chunks;
	int z_max = curr_z + radius_chunks >= world_size_chunks ? world_size_chunks : curr_z + radius_chunks;
	for (int i = x_min; i < x_max; i++)
	{
		for (int j = z_min; j < z_max; j++)
		{
			chunks[i][j].create_mesh();
		}
	}
}