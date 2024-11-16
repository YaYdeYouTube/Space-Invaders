#pragma once
#include "spaceship.hpp"
#include "obstacle.hpp"
#include "alien.hpp"
#include "mysteryship.hpp"

class Game
{
public:
	Game();
	~Game();
	void Draw();
	void Update();
	void HandleInput();
	bool run;
	int lives;
	int score;
	int hightscore;
	int level;
private:
	void DeleteInactiveLasers();
	void MoveAliens();
	void MoveDownAliens(int distance);
	void AlienShootLaser();
	void ChekForColisions();
	void GameOver();
	void Reset();
	void InitGame();
	void CheckForHighscore();
	void SaveHighscoreToFile(int highscore);
	int LoadHighscoreFromFile();
	void SwitchLevel();
	Spaceship spaceship;
	MysteryShip mysteryship;
	std::vector<Obstacle> obstacles;
	std::vector<Obstacle> CreateObstacles();
	std::vector<Alien> CreateAliens();
	std::vector<Alien> aliens;
	int aliensDirection;
	std::vector<Laser> alienLasers;
	constexpr static float alienShootLaserInterval = 0.35;
	float timeLastAlienFired;
	float mysteryShipSpawnInterval;
	float timeLastSpawn;
};
