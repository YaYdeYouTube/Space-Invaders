#include <raylib.h>
#include "game.hpp"
#include <string>

std::string FormatWithLeadingZeros(int number, int width)
{
	std::string numberText = std::to_string(number);
	int leadingZeros = width - numberText.length();
	return numberText = std::string(leadingZeros, '0') + numberText;
}

int main()
{
	Color grey = { 29, 29, 27, 255 };
	Color yellow = { 243, 216, 63, 255 };
	int offset = 50;
	int windowWidth = 800;
	int windowHeight = 550;

	InitWindow(windowWidth + offset, windowHeight + offset * 2, "C++ Space Invader");

	Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);
	Texture2D spaceshipImage = LoadTexture("Graphics/spaceship.png");

	SetTargetFPS(60);

	Game game;

	//Game loop
	while (!WindowShouldClose())
	{
		game.HandleInput();
		game.Update();

		BeginDrawing();
		ClearBackground(grey);
		DrawRectangleRoundedLines({ 10, 10, 830, 630 }, 0.18f, 20, 2, yellow);
		DrawLineEx({25, 580}, {825, 580}, 3, yellow);
		if (game.run)
		{
			std::string level = FormatWithLeadingZeros(game.level, 2);
			DrawTextEx(font, "LEVEL ", {620, 590}, 34, 2, yellow);
			DrawTextEx(font, level.c_str(), { 715, 590 }, 34, 2, yellow);
		}
		else
		{
			DrawTextEx(font, "GAME OVER", { 620, 590 }, 34, 2, yellow);
		}
		float x = 50.0;
		for (int i = 0; i < game.lives; i++)
		{
			DrawTextureV(spaceshipImage, { x, 595 }, WHITE);
			x += 50.0;
		}
		DrawTextEx(font, "SCORE", { 50, 15 }, 34, 2, yellow);
		std::string scoreText = FormatWithLeadingZeros(game.score, 6);
		DrawTextEx(font, scoreText.c_str(), { 50, 40 }, 34, 2, yellow);
		
		DrawTextEx(font, "HIGH-SCORE", { 570, 15 }, 34, 2, yellow);
		std::string highscoreText = FormatWithLeadingZeros(game.hightscore, 6);
		DrawTextEx(font, highscoreText.c_str(), { 655, 40 }, 34, 2, yellow);

		game.Draw();

		EndDrawing();
	}

	CloseWindow();
}