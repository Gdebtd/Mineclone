#include "sdl_game.h"
#include "chunk.h"

int voxel_from_map(vec3 pos)
{
	int x = pos.x;
	int y = pos.y;
	int z = pos.z;
	if (!(x >= 0 && x < world_size && y >= 0 && y < chunk_height && z >= 0 && z < world_size))
		return 0;
	int _x = x / chunk_width;
	int _z = z / chunk_width;
	x %= chunk_width;
	z %= chunk_width;
	return chunks[_x][_z].voxel_map[y][x][z];
}

int get_voxel_from_height(int x, int y, int z, int height, int block[4][3])
{
	if (noise[x][z] >= 0 && noise[x][z] <= 0.25)
	{
		if (y == height) return block[0][0];
		else if (y < height && y > height - 4) return block[0][1];
		else return block[0][2];
	}
	else if (noise[x][z] >= 0.25 && noise[x][z] <= 0.45)
	{
		if (y == height) return block[1][0];
		else if (y < height && y > height - 4) return block[1][1];
		else return block[1][2];
	}
	else if (noise[x][z] >= 0.45 && noise[x][z] <= 0.6)
	{
		if (y == height) return block[2][0];
		else if (y < height && y > height - 4) return block[2][1];
		else return block[2][2];
	}
	else if (noise[x][z] >= 0.6 && noise[x][z] <= 1)
	{
		if (y == height) return block[3][0];
		else if (y < height && y > height - 4) return block[3][1];
		else return block[3][2];
	}
}

int get_voxel(vec3 pos)
{
	int y = (int)pos.y, x = (int)pos.x, z = (int)pos.z;
	if (!(x >= 0 && x < world_size && y >= 0 && y < chunk_height && z >= 0 && z < world_size))
		return 0;
	if (x == 0 || x == world_size - 1 || z == 0 || z == world_size - 1)
		return 1;
	if (y == 0)
		return 2;
	float perlin = noise[x][z] * (chunk_height - air_height) + ground_height;
	if (y > (int)perlin)
	{
		return 0;
	}
	else
	{
		int block[4][3] = { { 9, 9, 3 }, { 6, 6, 7 }, { 5, 4, 3 }, { 3, 3, 3 } };
		return get_voxel_from_height(x, y, z, perlin, block);
	}
}