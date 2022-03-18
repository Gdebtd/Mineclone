#include "sdl_game.h"
#include "chunk.h"

void menu(bool eng)
{
	enable(false);
	GLuint textures[] = { load_texture("Resources/background.png"), load_texture("Resources/name.png"),
		load_texture("Resources/eng_buttons.png"), load_texture("Resources/rus_buttons.png"), load_texture("Resources/loading.png") };
	GLuint rules_textures[] = { load_texture("Resources/eng_rules.png"), load_texture("Resources/rus_rules.png") };
	bool run = true, show_rules = false;
	while (run)
	{
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		draw_menu(textures, eng);
		if (show_rules) draw_rules(eng, rules_textures);
		menu_events(run, eng, show_rules, textures[4]);
		SDL_GL_SwapWindow(window);
	}
	quit();
}

void play(bool eng)
{
	chunk ch;
	using Duration = std::chrono::duration<float>;
	perlin_noise(time(0));
	enable(true);
	init_chunks();
	GLuint textures[] = { load_texture("Resources/atlas.png"), load_texture("Resources/light.png"), load_texture("Resources/gui.png"), load_texture("Resources/esc.png") };
	player plr = player{ 0, 0, 0, 0, 0, 0, 1.5, 3, 0, 10, 4, 2, false, false, 2 };
	spawn_player(plr, world_size / 2, world_size / 2);
	float alpha = 0, day_time = 0, last_render = 0, day_cnt = 0;
	bool run = true;
	while (run && day_cnt < 5)
	{
		float current_time = std::chrono::duration_cast<Duration>(
			std::chrono::high_resolution_clock::now().time_since_epoch()
			).count();
		if ((current_time - last_render) > (1.0 / (float)(FPS)))
		{
			if (plr.y / ch.v_data.b_s < -5)
			{
				spawn_player(plr, plr.x / ch.v_data.b_s, plr.z / ch.v_data.b_s);
				plr.y += ch.v_data.b_s * 5;
			}
			last_render = current_time;
			draw_game_frame(plr, day_time, day_cnt, alpha, textures[1], textures[0], textures[2], textures[3]);
			events(run, plr);
			SDL_GL_SwapWindow(window);
		}
	}
	enable(false);
}

void start(bool eng)
{
	ShowCursor(false);
	init();
	srand(time(0));
	Mix_Music* music[] = { Mix_LoadMUS("Resources/sweden.wav"), Mix_LoadMUS("Resources/subwoofer_lullaby.wav"), 
		Mix_LoadMUS("Resources/haggstrom.wav"), Mix_LoadMUS("Resources/living_mice.wav"), Mix_LoadMUS("Resources/danny.wav") };
	Mix_PlayMusic(music[rand() % 5], -1);
	menu(eng);
}