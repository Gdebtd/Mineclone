#include "sdl_game.h"
#include "chunk.h"

void draw_game_frame(player & plr, float & day_time, float & day_cnt, float & alpha, GLuint sun_textures, GLuint textures, GLuint gui_textures, GLuint esc)
{
	float val = 0; // phases: 0, 0.125, 0.375, 0.5, 0.875, 1
	if (day_time <= 0.125) val = lerp(0.25, 1, day_time / 0.125);
	else if (day_time <= 0.375) val = 1.0;
	else if (day_time <= 0.5) val = lerp(1, 0, (day_time - 0.375) / 0.125);
	else if (day_time <= 0.875) val = 0;
	else if (day_time <= 1) val = lerp(0, 0.25, (day_time - 0.875) / 0.125);
	glClearColor(lerp(0, 0.5625, val), lerp(0, 0.6375, val), lerp(0, 1.00, val), 1); // set gb color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	player_controller(plr);
	sun_moon(plr, alpha, sun_textures); // sun
	glBindTexture(GL_TEXTURE_2D, textures);
	create_chunks(vec3{ plr.x, plr.y, plr.z });
	glDisable(GL_LIGHTING);
	gui(plr, gui_textures, esc); // gui
	day_time += 1.0 / (float)(DAY_TIME); // timer
	if (day_time >= 1)
	{
		day_time = 0;
		day_cnt++;
	}
	alpha += 360.0 / (float)(DAY_TIME);
	if (alpha > 360) alpha = 0;
}

void draw_loading(GLuint texture)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex2f(-0.35, -0.55);
	glTexCoord2f(0.0, 1.0); glVertex2f(-0.35, 0.55);
	glTexCoord2f(1.0, 1.0); glVertex2f(0.35, 0.55);
	glTexCoord2f(1.0, 0.0); glVertex2f(0.35, -0.55);
	glEnd();
}

void draw_buttons()
{
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.75); glVertex2f(-0.35, 0.2);
	glTexCoord2f(0.0, 1.0); glVertex2f(-0.35, 0.4);
	glTexCoord2f(1.0, 1.0); glVertex2f(0.35, 0.4);
	glTexCoord2f(1.0, 0.75); glVertex2f(0.35, 0.2);
	glTexCoord2f(0.0, 0.5); glVertex2f(-0.35, -0.1);
	glTexCoord2f(0.0, 0.75); glVertex2f(-0.35, 0.1);
	glTexCoord2f(1.0, 0.75); glVertex2f(0.35, 0.1);
	glTexCoord2f(1.0, 0.5); glVertex2f(0.35, -0.1);
	glTexCoord2f(0.0, 0.25); glVertex2f(-0.35, -0.4);
	glTexCoord2f(0.0, 0.5); glVertex2f(-0.35, -0.2);
	glTexCoord2f(1.0, 0.5); glVertex2f(0.35, -0.2);
	glTexCoord2f(1.0, 0.25); glVertex2f(0.35, -0.4);
	glTexCoord2f(0.0, 0.0); glVertex2f(-0.35, -0.7);
	glTexCoord2f(0.0, 0.25); glVertex2f(-0.35, -0.5);
	glTexCoord2f(1.0, 0.25); glVertex2f(0.35, -0.5);
	glTexCoord2f(1.0, 0.0); glVertex2f(0.35, -0.7);
	glEnd();
}

void draw_menu(GLuint textures[], bool eng)
{
	glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex2f(-1, -1);
	glTexCoord2f(0.0, 1.0); glVertex2f(-1, 1);
	glTexCoord2f(1.0, 1.0); glVertex2f(1, 1);
	glTexCoord2f(1.0, 0.0); glVertex2f(1, -1);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex2f(-0.35, 0.7);
	glTexCoord2f(0.0, 1.0); glVertex2f(-0.35, 0.9);
	glTexCoord2f(1.0, 1.0); glVertex2f(0.35, 0.9);
	glTexCoord2f(1.0, 0.0); glVertex2f(0.35, 0.7);
	glEnd();
	if (eng) glBindTexture(GL_TEXTURE_2D, textures[2]);
	else glBindTexture(GL_TEXTURE_2D, textures[3]);
	draw_buttons();
}

void draw_rules(bool eng, GLuint textures[])
{
	if (eng) glBindTexture(GL_TEXTURE_2D, textures[0]);
	else glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex2f(-0.5, -0.7);
	glTexCoord2f(0.0, 1.0); glVertex2f(-0.5, 0.7);
	glTexCoord2f(1.0, 1.0); glVertex2f(0.5, 0.7);
	glTexCoord2f(1.0, 0.0); glVertex2f(0.5, -0.7);
	glEnd();
}