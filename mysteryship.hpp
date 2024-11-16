#pragma once
#include "raylib.h"

class MysteryShip
{
public:
	MysteryShip();
	~MysteryShip();
	void Draw();
	void Update();
	void Spawn();
	Rectangle getRect();
	bool alive;
private:
	Vector2 position;
	Texture2D image;
	int speed;
};