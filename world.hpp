#ifndef _WORLD_HPP_
#define _WORLD_HPP_
#pragma once
#include <SDKDDKVer.h> // The latest version of SDK

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <GL/GLU.h>
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")

extern const float PI = 3.141592653f;
extern const float size = 20.f;
extern const uint16_t HEIGHT = sf::VideoMode::getDesktopMode().height;
extern const uint16_t WIDTH = sf::VideoMode::getDesktopMode().width;
extern float angleX, angleY; // Angle of rotate of camera

class WORLD
{
public:
	WORLD(size_t _worldSizeX = 1000u, size_t _worldSizeY = 200u, size_t _worldSizeZ = 1000u);
	~WORLD();
	bool check(int, int, int) const;
	void setBox(int, int, int, bool);
	void worldGeneration(int);
private:
	const size_t worldSizeX, worldSizeY, worldSizeZ;
	bool world[300][50][300];
	bool *** worlds;
};

class Player
{
public:
	Player(float, float, float);
	~Player();
	void update(float, const WORLD &);
	void collision(float, float, float, const WORLD &);
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

#endif _WORLD_HPP_