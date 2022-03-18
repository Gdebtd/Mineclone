#include "sdl_game.h"
#include "voxel_data.h"
#include "chunk.h"

voxel_data v_data;

void find_block(player plr, bool place, int place_ind, int dist)
{
	int x_ind = plr.x / (chunk_width * v_data.b_s), z_ind = plr.z / (chunk_width * v_data.b_s);
	int y = (int)(plr.y / v_data.b_s);
	int x = (int)(plr.x / v_data.b_s) % chunk_width, z = (int)(plr.z / v_data.b_s) % chunk_width;
	int i = 0;
	while (y >= 0 && y < chunk_height && x >= 0 && x < chunk_width && z >= 0 && z < chunk_width &&
		chunks[x_ind][z_ind].voxel_map[y][x][z] == 0 && i < dist)
	{
		plr.x -= sin(plr.rot_x / 180 * PI) * plr.speed;
		plr.y += tan(plr.rot_y / 180 * PI) * plr.speed;
		plr.z -= cos(plr.rot_x / 180 * PI) * plr.speed;
		x_ind = plr.x / (chunk_width * v_data.b_s);
		z_ind = plr.z / (chunk_width * v_data.b_s);
		y = (int)(plr.y / v_data.b_s);
		x = (int)(plr.x / v_data.b_s) % chunk_width;
		z = (int)(plr.z / v_data.b_s) % chunk_width;
		i++;
	}
	if (y >= 0 && y < chunk_height && x >= 0 && x < chunk_width && z >= 0 && z < chunk_width)
	{
		if (chunks[x_ind][z_ind].voxel_map[y][x][z] != 0)
		{
			if (place)
			{
				plr.x += sin(plr.rot_x / 180 * PI) * plr.speed;
				plr.y -= tan(plr.rot_y / 180 * PI) * plr.speed;
				plr.z += cos(plr.rot_x / 180 * PI) * plr.speed;
				x_ind = plr.x / (chunk_width * v_data.b_s);
				z_ind = plr.z / (chunk_width * v_data.b_s);
				y = (int)(plr.y / v_data.b_s);
				x = (int)(plr.x / v_data.b_s) % chunk_width;
				z = (int)(plr.z / v_data.b_s) % chunk_width;
				chunks[x_ind][z_ind].voxel_map[y][x][z] = place_ind;
				create_data_chunks(vec3{ plr.x, plr.y, plr.z });
			}
			else
			{
				if (chunks[x_ind][z_ind].voxel_map[y][x][z] != 1)
				{
					chunks[x_ind][z_ind].voxel_map[y][x][z] = 0;
					create_data_chunks(vec3{ plr.x, plr.y, plr.z });
				}
			}
		}
	}
}

void gravity(player & plr)
{
	int x_ind = plr.x / (chunk_width * v_data.b_s);
	int z_ind = plr.z / (chunk_width * v_data.b_s);
	int y = (int)(plr.y / v_data.b_s);
	int x = (int)(plr.x / v_data.b_s) % chunk_width;
	int z = (int)(plr.z / v_data.b_s) % chunk_width;
	if ((chunks[x_ind][z_ind].voxel_map[y < 0 ? chunk_height - 1 : y][x][z] == 0 &&
		chunks[x_ind][z_ind].voxel_map[y - 1 < 0 ? chunk_height - 1 : y - 1][x][z] == 0 &&
		chunks[x_ind][z_ind].voxel_map[y - 2 < 0 ? chunk_height - 1 : y - 2][x][z] == 0) ||
		y >= chunk_height || (plr.jump_speed > 0 && chunks[x_ind][z_ind].voxel_map[y + 1][x][z] == 0))
	{
		plr.y += plr.jump_speed;
	}
	if (plr.jump_speed > -plr.gravity)
		plr.jump_speed -= plr.gravity;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (plr.jump_speed <= 0)
		{
			if (y - 2 >= 0 && y < chunk_height &&
				chunks[x_ind][z_ind].voxel_map[y + 1][x][z] == 0 &&
				chunks[x_ind][z_ind].voxel_map[y - 2][x][z] != 0)
			{
				plr.jump_speed = plr.jump_height;
			}
		}
	}
}

void move(player & plr)
{
	int y = (int)(plr.y / v_data.b_s), x = (int)(plr.x / v_data.b_s) % chunk_width, z = (int)(plr.z / v_data.b_s) % chunk_width;
	if (GetAsyncKeyState('W') & 0x8000)
	{
		z = (int)((plr.z - cos(plr.rot_x / 180 * PI) * plr.speed) / v_data.b_s) % chunk_width;
		if (y >= chunk_height ||
			chunks[(int)(plr.x / (chunk_width * v_data.b_s))][(int)((plr.z - cos(plr.rot_x / 180 * PI) * plr.speed) / (chunk_width * v_data.b_s))].voxel_map[y][x][z] == 0 &&
			chunks[(int)(plr.x / (chunk_width * v_data.b_s))][(int)((plr.z - cos(plr.rot_x / 180 * PI) * plr.speed) / (chunk_width * v_data.b_s))].voxel_map[y - 1][x][z] == 0)
			plr.z -= cos(plr.rot_x / 180 * PI) * plr.speed;
		x = (int)((plr.x - sin(plr.rot_x / 180 * PI) * plr.speed) / v_data.b_s) % chunk_width;
		if (y >= chunk_height ||
			chunks[(int)((plr.x - sin(plr.rot_x / 180 * PI) * plr.speed) / (chunk_width * v_data.b_s))][(int)(plr.z / (chunk_width * v_data.b_s))].voxel_map[y][x][z] == 0 &&
			chunks[(int)((plr.x - sin(plr.rot_x / 180 * PI) * plr.speed) / (chunk_width * v_data.b_s))][(int)(plr.z / (chunk_width * v_data.b_s))].voxel_map[y - 1][x][z] == 0)
			plr.x -= sin(plr.rot_x / 180 * PI) * plr.speed;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		int z = (int)((plr.z + cos(plr.rot_x / 180 * PI) * plr.speed) / v_data.b_s) % chunk_width;
		if (y >= chunk_height ||
			chunks[(int)(plr.x / (chunk_width * v_data.b_s))][(int)((plr.z + cos(plr.rot_x / 180 * PI) * plr.speed) / (chunk_width * v_data.b_s))].voxel_map[y][x][z] == 0 &&
			chunks[(int)(plr.x / (chunk_width * v_data.b_s))][(int)((plr.z + cos(plr.rot_x / 180 * PI) * plr.speed) / (chunk_width * v_data.b_s))].voxel_map[y - 1][x][z] == 0)
			plr.z += cos(plr.rot_x / 180 * PI) * plr.speed;
		x = (int)((plr.x + sin(plr.rot_x / 180 * PI) * plr.speed) / v_data.b_s) % chunk_width;
		if (y >= chunk_height ||
			chunks[(int)((plr.x + sin(plr.rot_x / 180 * PI) * plr.speed) / (chunk_width * v_data.b_s))][(int)(plr.z / (chunk_width * v_data.b_s))].voxel_map[y][x][z] == 0 &&
			chunks[(int)((plr.x + sin(plr.rot_x / 180 * PI) * plr.speed) / (chunk_width * v_data.b_s))][(int)(plr.z / (chunk_width * v_data.b_s))].voxel_map[y - 1][x][z] == 0)
			plr.x += sin(plr.rot_x / 180 * PI) * plr.speed;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		int z = (int)((plr.z + cos(plr.rot_x / 180 * PI + PI / 2) * plr.speed) / v_data.b_s) % chunk_width;
		if (y >= chunk_height ||
			chunks[(int)(plr.x / (chunk_width * v_data.b_s))][(int)((plr.z + cos(plr.rot_x / 180 * PI + PI / 2) * plr.speed) / (chunk_width * v_data.b_s))].voxel_map[y][x][z] == 0 &&
			chunks[(int)(plr.x / (chunk_width * v_data.b_s))][(int)((plr.z + cos(plr.rot_x / 180 * PI + PI / 2) * plr.speed) / (chunk_width * v_data.b_s))].voxel_map[y - 1][x][z] == 0)
			plr.z += cos(plr.rot_x / 180 * PI + PI / 2) * plr.speed;
		x = (int)((plr.x + sin(plr.rot_x / 180 * PI + PI) * plr.speed) / v_data.b_s) % chunk_width;
		if (y >= chunk_height ||
			chunks[(int)((plr.x + sin(plr.rot_x / 180 * PI + PI / 2) * plr.speed) / (chunk_width * v_data.b_s))][(int)(plr.z / (chunk_width * v_data.b_s))].voxel_map[y][x][z] == 0 &&
			chunks[(int)((plr.x + sin(plr.rot_x / 180 * PI + PI / 2) * plr.speed) / (chunk_width * v_data.b_s))][(int)(plr.z / (chunk_width * v_data.b_s))].voxel_map[y - 1][x][z] == 0)
			plr.x += sin(plr.rot_x / 180 * PI + PI / 2) * plr.speed;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		int z = (int)((plr.z - cos(plr.rot_x / 180 * PI + PI / 2) * plr.speed) / v_data.b_s) % chunk_width;
		if (y >= chunk_height ||
			chunks[(int)(plr.x / (chunk_width * v_data.b_s))][(int)((plr.z - cos(plr.rot_x / 180 * PI + PI / 2) * plr.speed) / (chunk_width * v_data.b_s))].voxel_map[y][x][z] == 0 &&
			chunks[(int)(plr.x / (chunk_width * v_data.b_s))][(int)((plr.z - cos(plr.rot_x / 180 * PI + PI / 2) * plr.speed) / (chunk_width * v_data.b_s))].voxel_map[y - 1][x][z] == 0)
			plr.z -= cos(plr.rot_x / 180 * PI + PI / 2) * plr.speed;
		x = (int)((plr.x - sin(plr.rot_x / 180 * PI + PI) * plr.speed) / v_data.b_s) % chunk_width;
		if (y >= chunk_height ||
			chunks[(int)((plr.x - sin(plr.rot_x / 180 * PI + PI / 2) * plr.speed) / (chunk_width * v_data.b_s))][(int)(plr.z / (chunk_width * v_data.b_s))].voxel_map[y][x][z] == 0 &&
			chunks[(int)((plr.x - sin(plr.rot_x / 180 * PI + PI / 2) * plr.speed) / (chunk_width * v_data.b_s))][(int)(plr.z / (chunk_width * v_data.b_s))].voxel_map[y - 1][x][z] == 0)
			plr.x -= sin(plr.rot_x / 180 * PI + PI / 2) * plr.speed;
	}
}

void rotate(player & plr)
{
	if (!(GetAsyncKeyState(VK_ESCAPE) & 0x8000))
	{
		POINT m_pos;
		GetCursorPos(&m_pos);
		int x, y, w, h;
		SDL_GetWindowPosition(window, &x, &y);
		SDL_GetWindowSize(window, &w, &h);
		int xc = x + w / 2;
		int yc = y + h / 2;
		plr.rot_x += (xc - m_pos.x) / plr.sensitivity;
		plr.rot_y += (yc - m_pos.y) / plr.sensitivity;
		if (plr.rot_y < -89.0) plr.rot_y = -89.0;
		if (plr.rot_y > 89.0) plr.rot_y = 89.0;
		SetCursorPos(xc, yc);
	}
}

void player_controller(player & plr)
{
	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		plr.speed = plr.run_speed;
	else
		plr.speed = plr.walk_speed;
	move(plr);
	rotate(plr);
	gravity(plr);
	if (plr.l_mouse)
	{
		find_block(plr, false, 0, 40);
	}
	else if (plr.r_mouse)
	{
		find_block(plr, true, plr.place, 40);
	}
	plr.l_mouse = false;
	plr.r_mouse = false;
	gluLookAt(plr.x, plr.y, plr.z, plr.x - sin(plr.rot_x / 180 * PI), plr.y + tan(plr.rot_y / 180 * PI), plr.z - cos(plr.rot_x / 180 * PI), 0, 1, 0);
}