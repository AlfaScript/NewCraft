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

class Player
{
public:
	Player(float x0, float y0, float z0);
	~Player();
	void update(float time);
	void collision(float Dx, float Dy, float Dz);
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

class WORLD
{
public:
	WORLD(size_t _worldSizeX = 1000u, size_t _worldSizeY = 200u, size_t _worldSizeZ = 1000u);
	~WORLD();
	bool check(int x, int y, int z);
	void setBox(int x, int y, int z, bool set);
	void worldGeneration(int mode);
private:
	const size_t worldSizeX, worldSizeY, worldSizeZ;
	bool world[300][50][300];
	bool *** worlds;
};

GLuint LoadTexture(sf::String);
void createBox(GLuint[], float);
void menu(sf::RenderWindow &);

const float PI = 3.141592653f;
const float size = 20.f;
const uint16_t HEIGHT = sf::VideoMode::getDesktopMode().height;
const uint16_t WIDTH = sf::VideoMode::getDesktopMode().width;
float angleX, angleY; // Angle of rotate of camera
class WORLD world;

#endif _WORLD_HPP_