#include "sdl_game.h"
#include "chunk.h"

void flip_surface_vertically(SDL_Surface* surface)
{
	SDL_LockSurface(surface);
	int pitch = surface->pitch; // row size
	char* temp = new char[pitch]; // intermediate buffer
	char* pixels = (char*)surface->pixels;
	for (int i = 0; i < surface->h / 2; i++)
	{
		// get pointers to the two rows to swap
		char* row1 = pixels + i * pitch;
		char* row2 = pixels + (surface->h - i - 1) * pitch;
		// swap rows
		memcpy(temp, row1, pitch);
		memcpy(row1, row2, pitch);
		memcpy(row2, temp, pitch);
	}
	delete[] temp;
	SDL_UnlockSurface(surface);
}

GLuint load_texture(const char* path)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	SDL_Surface* TextureImage = IMG_Load(path);
	if (TextureImage)
	{
		flip_surface_vertically(TextureImage);
		int mode = TextureImage->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TextureImage->w,
			TextureImage->h, 0, mode, GL_UNSIGNED_BYTE, TextureImage->pixels);
	}
	else
	{
		std::cout << "Failed to load texture\n";
		return 0;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

void sun_moon(player plr, float alpha, GLuint textures)
{
	float x = plr.x, y = plr.y, z = plr.z, rot_x = plr.rot_x, rot_y = plr.rot_y;
	glDisable(GL_LIGHTING);
	glTranslatef(x, y, z);
	glRotatef(alpha, 0, 0, 1);
	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, textures);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(1280, -75, -75);
	glTexCoord2f(0.0, 1.0); glVertex3f(1280, 75, -75);
	glTexCoord2f(0.5, 1.0); glVertex3f(1280, 75, 75);
	glTexCoord2f(0.5, 0.0); glVertex3f(1280, -75, 75);
	glTexCoord2f(0.5, 0.0); glVertex3f(-1280, -75, -75);
	glTexCoord2f(0.5, 1.0); glVertex3f(-1280, 75, -75);
	glTexCoord2f(1.0, 1.0); glVertex3f(-1280, 75, 75);
	glTexCoord2f(1.0, 0.0); glVertex3f(-1280, -75, 75);
	glEnd();
	glEnable(GL_LIGHTING);
	float sun_pos[] = { 1, 0, 0, 0 };
	if (alpha >= 180) sun_pos[0] = 0;
	glLightfv(GL_LIGHT0, GL_POSITION, sun_pos);
	glRotatef(-alpha, 0, 0, 1);
	glTranslatef(-x, -y, -z);
}

void gui_inventory(player plr, GLuint esc)
{
	chunk ch;
	std::vector <vec2> uv = ch.v_data.add_texture(ch.blocks[plr.place].front_texture + 4);
	glBegin(GL_QUADS);
	glTexCoord2f(0.25, 0.75); glVertex3f(1.01, -0.5 - 0.0625, -0.0625);
	glTexCoord2f(0.25, 1.0); glVertex3f(1.01, -0.5 + 0.0625, -0.0625);
	glTexCoord2f(0.5, 1.0); glVertex3f(1.01, -0.5 + 0.0625, 0.0625);
	glTexCoord2f(0.5, 0.75); glVertex3f(1.01, -0.5 - 0.0625, 0.0625);
	glTexCoord2f(uv[0].x, uv[0].y); glVertex3f(1.005, -0.5 - 0.04, -0.04);
	glTexCoord2f(uv[1].x, uv[1].y); glVertex3f(1.005, -0.5 + 0.04, -0.04);
	glTexCoord2f(uv[3].x, uv[3].y); glVertex3f(1.005, -0.5 + 0.04, 0.04);
	glTexCoord2f(uv[2].x, uv[2].y); glVertex3f(1.005, -0.5 - 0.04, 0.04);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, esc);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(1.01,  0.9, -ASPECT);
	glTexCoord2f(0.0, 1.0); glVertex3f(1.01, 1, -ASPECT);
	glTexCoord2f(1.0, 1.0); glVertex3f(1.01, 1, -ASPECT + 0.15);
	glTexCoord2f(1.0, 0.0); glVertex3f(1.01, 0.9, -ASPECT + 0.15);
	glEnd();
}

void gui(player plr, GLuint textures, GLuint esc)
{
	float x = plr.x, y = plr.y, z = plr.z, rot_x = plr.rot_x, rot_y = plr.rot_y;
	glTranslatef(x, y, z);
	glRotatef(rot_x + 90, 0, 1, 0);
	glRotatef(rot_y, 0, 0, 1);
	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, textures);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.75); glVertex3f(1.01, -0.0625, -0.0625);
	glTexCoord2f(0.0, 1.0); glVertex3f(1.01, 0.0625, -0.0625);
	glTexCoord2f(0.25, 1.0); glVertex3f(1.01, 0.0625, 0.0625);
	glTexCoord2f(0.25, 0.75); glVertex3f(1.01, -0.0625, 0.0625);
	glEnd();
	gui_inventory(plr, esc);
	glRotatef(-rot_y, 0, 0, 1);
	glRotatef(-rot_x - 90, 0, 1, 0);
	glTranslatef(-x, -y, -z);
}