#include "game.hpp"
#include <iostream>
#include "fstream"

Game::Game() 
{
	InitGame();
}

Game::~Game() {
	Alien::UnLoadImages();
}

void Game::Update()
{
	if (run)
	{
		double currentTime = GetTime();
		if (currentTime - timeLastSpawn > mysteryShipSpawnInterval)
		{
			mysteryship.Spawn();
			timeLastSpawn = GetTime();
			mysteryShipSpawnInterval = GetRandomValue(10, 20);
		}

		for (auto& laser : spaceship.lasers)
		{
			laser.Update();
		}

		DeleteInactiveLasers();

		MoveAliens();
		for (auto& laser : alienLasers)
		{
			laser.Update();
		}

		AlienShootLaser();

		mysteryship.Update();

		ChekForColisions();
		SwitchLevel();
	}
}

void Game::Draw()
{
	spaceship.Draw();
	mysteryship.Draw();

	for (auto& laser : spaceship.lasers)
	{
		laser.Draw();
	}

	for (auto& obstacle : obstacles)
	{
		obstacle.Draw();
	}

	for (auto& alien : aliens)
	{
		alien.Draw();
	}

	for (auto& laser : alienLasers)
	{
		laser.Draw();
	}
}

void Game::HandleInput()
{
	if (run)
	{
		if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
		{
			spaceship.MoveLeft();
		}
		else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
		{
			spaceship.MoveRight();
		}
		else if (IsKeyDown(KEY_SPACE))
		{
			spaceship.FireLaser();
		}
	}
	else
	{
		if (IsKeyDown(KEY_ENTER))
		{
			Reset();
			InitGame();
		}
	}
}

void Game::DeleteInactiveLasers() 
{
	for (auto it = spaceship.lasers.begin(); it != spaceship.lasers.end();)
	{
		if (!it->active)
		{
			it = spaceship.lasers.erase(it);
		}
		else
		{
			++it;
		}
	}

	for (auto it = alienLasers.begin(); it != alienLasers.end();)
	{
		if (!it->active)
		{
			it = alienLasers.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void Game::MoveAliens()
{
	for (auto& alien : aliens)
	{
		if (alien.position.x + alien.alienImages[alien.type - 1].width > GetScreenWidth() - 25)
		{
			aliensDirection = -1;
			MoveDownAliens(2);
		}
		else if (alien.position.x < 25)
		{
			aliensDirection = 1;
			MoveDownAliens(2);
		}
		alien.Update(aliensDirection);
	}
}

void Game::MoveDownAliens(int distance)
{
	for (auto& alien : aliens)
	{
		alien.position.y += distance;
	}
}

void Game::AlienShootLaser()
{
	double currentTime = GetTime();
	if (currentTime - timeLastAlienFired >= alienShootLaserInterval && !aliens.empty())
	{
		int randomIndex = GetRandomValue(0, aliens.size() - 1);
		Alien& alien = aliens[randomIndex];
		alienLasers.push_back(Laser({ alien.position.x + alien.alienImages[alien.type - 1].width / 2, alien.position.y + alien.alienImages[alien.type - 1].height }, 6));
		timeLastAlienFired = GetTime();
	}
}

void Game::ChekForColisions()
{
	// Spaceship lasers
	for (auto& laser : spaceship.lasers)
	{
		auto it = aliens.begin();
		while (it != aliens.end())
		{
			if (CheckCollisionRecs(it->getRect(), laser.getRect()))
			{
				if (it->type = 1)
				{
					score += 100;
				}
				else if (it->type = 2)
				{
					score += 200;
				}
				else if (it->type = 3)
				{
					score += 300;
				}
				CheckForHighscore();

				it = aliens.erase(it);
				laser.active = false;
			}
			else {
				it++;
			}
		}

		for (auto& obstacle : obstacles)
		{
			auto it = obstacle.blocks.begin();
			while (it != obstacle.blocks.end())
			{
				if (CheckCollisionRecs(it->getRect(), laser.getRect()))
				{
					it = obstacle.blocks.erase(it);
					laser.active = false;
				}
				else {
					it++;
				}
			}
		}

		if (CheckCollisionRecs(mysteryship.getRect(), laser.getRect()))
		{
			score += 500;
			mysteryship.alive = false;
			laser.active = false;

			CheckForHighscore();
		}
	}

	// Aliens lasers
	for (auto& laser : alienLasers)
	{
		if (CheckCollisionRecs(laser.getRect(), spaceship.getRect()))
		{
			laser.active = false;
			lives--;
			if (lives == 0)
			{
				GameOver();
			}
		}
		
		for (auto& obstacle : obstacles)
		{
			auto it = obstacle.blocks.begin();
			while (it != obstacle.blocks.end())
			{
				if (CheckCollisionRecs(laser.getRect(), it->getRect()))
				{
					it = obstacle.blocks.erase(it);
					laser.active = false;
				}
				else
				{
					it++;
				}
			}
		}
	}

	// Aliens
	for (auto& alien : aliens)
	{
		for (auto& obstacle : obstacles)
		{
			auto it = obstacle.blocks.begin();
			while (it != obstacle.blocks.end())
			{
				if (CheckCollisionRecs(it->getRect(), alien.getRect()))
				{
					it = obstacle.blocks.erase(it);
				}
				else {
					it++;
				}
			}
		}

		if (CheckCollisionRecs(alien.getRect(), spaceship.getRect()))
		{
			GameOver();
		}
	}
}

void Game::GameOver()
{
	run = false;
}

void Game::Reset()
{
	spaceship.Reset();
	aliens.clear();
	alienLasers.clear();
	obstacles.clear();
}

void Game::InitGame()
{
	obstacles = CreateObstacles();
	aliens = CreateAliens();
	aliensDirection = 1;
	timeLastAlienFired = 0.0;
	timeLastSpawn = 0.0;
	mysteryShipSpawnInterval = GetRandomValue(10, 20);
	lives = 3;
	run = true;
	score = 0;
	hightscore = LoadHighscoreFromFile();
	level = 1;
}

void Game::CheckForHighscore()
{
	if (score > hightscore)
	{
		hightscore = score;
		SaveHighscoreToFile(hightscore);
	}
}

void Game::SaveHighscoreToFile(int highscore)
{
	std::ofstream highscoreFile("highscore.txt");
	if (highscoreFile.is_open())
	{
		highscoreFile << highscore;
		highscoreFile.close();
	}
	else
	{
		std::cerr << "Failed to save save highscore file" << std::endl;
	}
}

int Game::LoadHighscoreFromFile()
{
	int loadedHighscore = 0;
	std::ifstream highscoreFile("highscore.txt");
	if (highscoreFile.is_open())
	{
		highscoreFile >> loadedHighscore;
		highscoreFile.close();
	}
	else
	{
		std::cerr << "Failed to save save highscore file" << std::endl;
	}
	return loadedHighscore;
}

void Game::SwitchLevel()
{
	if (aliens.empty())
	{
		level++;
		aliens = CreateAliens();
	}
}

std::vector<Obstacle> Game::CreateObstacles()
{
	int obstacleWidth = Obstacle::grid[0].size() * 3;
	float gap = (GetScreenWidth() - (4 * obstacleWidth)) / 5;

	for (int i = 0; i < 4; ++i)
	{
		float offsetX = (i + 1) * gap + i * obstacleWidth;
		obstacles.push_back(Obstacle({ offsetX, float(GetScreenHeight() - 200)}));
	}
	return obstacles;
}

std::vector<Alien> Game::CreateAliens()
{
	std::vector<Alien> Aliens;
	for (int row = 0; row < 5; ++row)
	{
		int alienType;
		if (row == 0)
		{
			alienType = 3;
		}
		else if (row == 1 || row == 2)
		{
			alienType = 2;
		}
		else
		{
			alienType = 1;
		}

		for (int column = 0; column < 11; ++column)
		{
			float x = 75 + column * 55;
			float y = 110 + row * 55;
			Aliens.push_back(Alien(alienType, {x, y}));
		}
	}
	return Aliens;
}
