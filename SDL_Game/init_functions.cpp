#include "sdl_game.h"
#include "chunk.h"

SDL_Window* window;
SDL_GLContext context;
SDL_Event event;

void init()
{
	int width = WIDTH;
	// init SDL, IMG, window, context
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	window = SDL_CreateWindow("Mineclone 1.0a", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
	context = SDL_GL_CreateContext(window);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	// set OpenGL attributes
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(
		SDL_GL_CONTEXT_PROFILE_MASK,
		SDL_GL_CONTEXT_PROFILE_CORE);
}

void enable(bool game)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (game) gluPerspective(90.f, ASPECT, 1.f, 2000.f);
	else glOrtho(-1, 1, -1, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (game)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glClearDepth(1.f);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_COLOR_MATERIAL);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glDisable(GL_COLOR_MATERIAL);
	}
}
void quit()
{
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}

void events(bool & run, player & plr)
{
	chunk ch;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT: run = false;
		case SDL_KEYUP: if (event.key.keysym.sym == SDLK_ESCAPE) run = false;
		case SDL_MOUSEBUTTONUP:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
				plr.l_mouse = true;
				plr.r_mouse = false;
				break;
			case SDL_BUTTON_RIGHT:
				plr.l_mouse = false;
				plr.r_mouse = true;
				break;
			}
		case SDL_MOUSEWHEEL:
			if (event.wheel.y < 11)
			{
				if (event.wheel.y > 0)
				{
					plr.place++;
					if (plr.place >= ch.blocks.size())
						plr.place = 2;
				}
				else
				{
					plr.place--;
					if (plr.place <= 1)
						plr.place = ch.blocks.size() - 1;
				}
			}
		}
	}
}

void menu_events(bool & run, bool & eng, bool & show_rules, GLuint texture)
{
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT: run = false;
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case SDLK_1:
				draw_loading(texture);
				SDL_GL_SwapWindow(window);
				play(eng);
				break;
			case SDLK_2:
				show_rules = !show_rules;
				break;
			case SDLK_3:
				eng = !eng;
				break;
			case SDLK_4:
				run = false;
				break;
			}
		}
	}
}