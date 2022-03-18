#include "sdl_game.h"

float noise[noise_size][noise_size];

float lerp(float a, float b, float value)
{
	return b * value + a * (1 - value);
}

void scale_noise()
{
	float min = 1, max = 0;
	for (int i = 1; i < noise_size - 1; i++)
	{
		for (int j = 1; j < noise_size - 1; j++)
		{
			if (noise[i][j] < min)
				min = noise[i][j];
			if (noise[i][j] > max)
				max = noise[i][j];
		}
	}
	for (int i = 1; i < noise_size - 1; i++)
	{
		for (int j = 1; j < noise_size - 1; j++)
		{
			noise[i][j] -= min;
		}
	}
	for (int i = 1; i < noise_size - 1; i++)
	{
		for (int j = 1; j < noise_size - 1; j++)
		{
			noise[i][j] = lerp(0, 1, noise[i][j] / (max - min));
		}
	}
}

void perlin_noise(unsigned int seed)
{
	srand(seed);
	for (int i = 0; i < noise_size; i++)
	{
		for (int j = 0; j < noise_size; j++)
		{
			noise[i][j] = (rand() % 101) / 100.0;
		}
	}
	for (int n = 0; n < 10; n++)
	{
		for (int i = 1; i < noise_size - 1; i++)
		{
			for (int j = 1; j < noise_size - 1; j++)
			{
				float up = noise[i - 1][j - 1] + noise[i - 1][j] + noise[i - 1][j + 1];
				float mid = noise[i][j - 1] + noise[i][j] + noise[i][j + 1];
				float down = noise[i + 1][j - 1] + noise[i + 1][j] + noise[i + 1][j + 1];
				noise[i][j] = (up + mid + down) / 9;
			}
		}
	}
	scale_noise();
}