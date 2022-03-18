#pragma once

#include <Windows.h>
#include <time.h>
#include <chrono>
#include <random>
#include <iostream>
#include <vector>

#include <SDL.h>
#include "GL/GL.h"
#include "GL/GLU.h"
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#define GL_CLAMP_TO_EDGE 0x812F

#define WIDTH 1280
#define HEIGHT 720
#define ASPECT 1.0 * WIDTH / HEIGHT

#define FPS 30
#define DAY_TIME_SEC 60
#define DAY_TIME DAY_TIME_SEC * FPS

extern SDL_Window* window;
extern SDL_GLContext context;

#define PI 3.14159265

#define noise_size 256
extern float noise[noise_size][noise_size];

struct vec2
{
	float x;
	float y;
};

struct vec3
{
	float x;
	float y;
	float z;
};

struct player
{
	float x;
	float y;
	float z;
	float rot_x;
	float rot_y;
	float speed;
	float walk_speed;
	float run_speed;
	float jump_speed;
	float jump_height;
	float gravity;
	float sensitivity;
	bool r_mouse;
	bool l_mouse;
	int place;
};

struct block_type
{
public:
	std::string name;
	bool is_solid;
	int back_texture;
	int front_texture;
	int top_texture;
	int bottom_texture;
	int left_texture;
	int right_texture;

	int get_textureID(int faceIndex)
	{
		switch (faceIndex)
		{
		case 0:
			return back_texture;
		case 1:
			return front_texture;
		case 2:
			return top_texture;
		case 3:
			return bottom_texture;
		case 4:
			return left_texture;
		case 5:
			return right_texture;
		}
	}
};

// player controller
void player_controller(player & plr);
// generation
void perlin_noise(unsigned int seed);
void spawn_player(player & plr, int x, int z);
void init_chunks();
void create_data_chunks(vec3 pos);
void create_chunks(vec3 pos);
int get_voxel(vec3 pos);
int voxel_from_map(vec3 pos);
float lerp(float a, float b, float value);
// drawing
GLuint load_texture(const char* path);
void sun_moon(player plr, float alpha, GLuint textures);
void gui(player plr, GLuint textures, GLuint esc);
void draw_game_frame(player & plr, float & day_time, float & day_cnt, float & alpha, GLuint sun_textures, GLuint textures, GLuint gui_textures, GLuint esc);
// main
void init();
void enable(bool game);
void quit();
void events(bool & run, player & plr);
void menu_events(bool & run, bool & eng, bool & show_rules, GLuint texture);
void draw_menu(GLuint textures[], bool eng);
void draw_rules(bool eng, GLuint textures[]);
void draw_loading(GLuint texture);
void menu(bool eng);
void play(bool eng);
void start(bool eng);