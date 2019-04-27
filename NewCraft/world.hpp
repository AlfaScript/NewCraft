#pragma once
#ifndef WORLD_HPP
#define WORLD_HPP

class WORLD
{
public:
	static WORLD * getInstance(void);
	static void DestroyInstance(void);
	bool check(uint16_t, uint16_t, uint16_t) const;
	void setBox(int, int, int, bool);
	void worldGeneration(uint16_t);

private:
	WORLD() 
	{ 
		for (size_t x = 0; x < worldSizeX; ++x)
			for (size_t y = 0; y < worldSizeY; ++y)
				for (size_t z = 0; z < worldSizeZ; ++z)
					world[x][y][z] = false;
	}
	~WORLD() { }
	WORLD(const WORLD &) = delete;
	WORLD & operator=(const WORLD &) = delete;
	WORLD(WORLD && other) noexcept = delete;

private:
	static WORLD * p_instance;
	bool world[300][50][300];
	const size_t worldSizeX = 300u, worldSizeY = 50u, worldSizeZ = 300u;
};

class Player
{
public:
	Player(float, float, float);
	~Player();
	void update(float);
	void collision(float, float, float);
	void keyboard(void);
	float getX(void) const;
	float getY(void) const;
	float getZ(void) const;
	float getH(void) const;
private:
	float x, y, z;
	float dx, dy, dz;
	float w, h, d;  // width, height, depth 
	bool onGround;
	float speed;
};

GLuint LoadTexture(sf::String);
void createBox(GLuint[], float);
void menu(sf::RenderWindow &);

const enum WORLDTYPE : uint16_t { SUPERFLAT, METEORS, NORMAL };
const float PI = 3.141592653f;
const float size = 20.f;
const uint16_t HEIGHT = sf::VideoMode::getDesktopMode().height;
const uint16_t WIDTH = sf::VideoMode::getDesktopMode().width;

extern float angleX, angleY; // Angle of rotate of camera

#endif WORLD_HPP