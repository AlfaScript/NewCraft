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

<<<<<<< HEAD
GLuint LoadTexture(sf::String);
void createBox(GLuint[], float);
void menu(sf::RenderWindow &);
=======
GLuint LoadTexture(sf::String name)
{
	sf::Image image;
	if (!image.loadFromFile(name))
	{
		std::cerr << "Error! File can't load" << std::endl;
        return EXIT_FAILURE;
	}

	image.flipVertically();

	GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image.getSize().x, image.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F); // 0x812F = CLAMP_TO_EDGE
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);

	return texture;
}

void createBox(GLuint box[], float size)
{
	glBindTexture(GL_TEXTURE_2D, box[0]);
	glBegin(GL_QUADS); // back
		glTexCoord2f(0.f, 0.f);   glVertex3f(-size, -size, -size);
		glTexCoord2f(1.f, 0.f);   glVertex3f( size, -size, -size);
		glTexCoord2f(1.f, 1.f);   glVertex3f( size,  size, -size);
		glTexCoord2f(0.f, 1.f);   glVertex3f(-size,  size, -size);
	glEnd();
		
	glBindTexture(GL_TEXTURE_2D, box[1]);
	glBegin(GL_QUADS); // front
		glTexCoord2f(0.f, 0.f); glVertex3f(size, -size, size);
		glTexCoord2f(1.f, 0.f); glVertex3f(-size,  -size, size);
		glTexCoord2f(1.f, 1.f); glVertex3f( -size,  size, size);
		glTexCoord2f(0.f, 1.f); glVertex3f( size, size, size);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, box[2]);
	glBegin(GL_QUADS); // left
		glTexCoord2f(0.f, 0.f); glVertex3f(-size, -size,  size);
		glTexCoord2f(1.f, 0.f); glVertex3f(-size, -size, -size);
		glTexCoord2f(1.f, 1.f); glVertex3f(-size,  size, -size);
		glTexCoord2f(0.f, 1.f); glVertex3f(-size,  size,  size);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, box[3]);
	glBegin(GL_QUADS); // right
		glTexCoord2f(0.f, 0.f); glVertex3f(size, -size, -size);
		glTexCoord2f(1.f, 0.f); glVertex3f(size, -size, size);
		glTexCoord2f(1.f, 1.f); glVertex3f(size, size,  size);
		glTexCoord2f(0.f, 1.f); glVertex3f(size, size,  -size);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, box[4]);
	glBegin(GL_QUADS); // bottom
		glTexCoord2f(0.f, 0.f); glVertex3f(-size, -size,  size);
		glTexCoord2f(1.f, 0.f); glVertex3f(size, -size, size);
		glTexCoord2f(1.f, 1.f); glVertex3f( size, -size, -size);
		glTexCoord2f(0.f, 1.f); glVertex3f( -size, -size,  -size);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, box[5]);
	glBegin(GL_QUADS); // top  		
		glTexCoord2f(0.f, 0.f); glVertex3f(-size, size,  -size);
		glTexCoord2f(1.f, 0.f); glVertex3f(size, size, -size);
		glTexCoord2f(1.f, 1.f); glVertex3f( size, size, size);
		glTexCoord2f(0.f, 1.f); glVertex3f( -size, size,  size);	     
	glEnd();
}

void menu(sf::RenderWindow & window) 
{
	ShowCursor(true);
	bool isMenu = true;
	unsigned short int menuNum = 0;

	sf::Font font;
	font.loadFromFile("resources/menu/majorforce.ttf");
	sf::Text menu1("START GAME", font, 50), menu2("ABOUT", font, 50), menu3("EXIT", font, 50);
	menu1.setPosition(WIDTH / 6.f - 100, HEIGHT / 2.f - 60.f);
	menu1.setOutlineThickness(1);
	menu2.setPosition(WIDTH / 6.f - 100, HEIGHT / 2.f);
	menu2.setOutlineThickness(1);
	menu3.setPosition(WIDTH / 6.f - 100, HEIGHT / 2.f + 60.f);
	menu3.setOutlineThickness(1);
	
	sf::Texture background;
	if(!background.loadFromFile("resources/menu/backgrounds/firstBackground.png"))
	{
		std::cerr << "Error! File (firstBackground.png) can't load" << std::endl;
		return;
	}
	sf::Sprite back(background);
	back.setColor(sf::Color(255, 255, 255, 200));
	int i = 1;
	sf::Clock clock;
	sf::Texture backPhotoTex;
	if (!backPhotoTex.loadFromFile("resources/menu/backgrounds/background1.png"))
	{
		std::cerr << "Error! File (background1.png) can't load" << std::endl;
		return;
	}
	sf::Texture buttonBackground;
	if (!buttonBackground.loadFromFile("resources/menu/backgrounds/buttonBackground.png"))
	{
		std::cerr << "Error! File (buttonBackground.png) can't load" << std::endl;
		return;
	}
	sf::Sprite backPhoto(backPhotoTex), buttonPhoto(buttonBackground);

	while (isMenu)
	{
		menuNum = 0;
		window.clear();
		menu1.setFillColor(sf::Color(255, 207, 133));
		menu2.setFillColor(sf::Color(255, 207, 133));
		menu3.setFillColor(sf::Color(255, 207, 133));
		menu1.setStyle(sf::Text::Regular);
		menu2.setStyle(sf::Text::Regular);
		menu3.setStyle(sf::Text::Regular);
		//==================================
		if (clock.getElapsedTime().asSeconds() > 5.f)
		{
			if (i > 3)
				i = 1;
			if (!backPhotoTex.loadFromFile("resources/menu/backgrounds/background" + std::to_string(i) + ".png"))
			{
				std::cerr << "Error! File (background" + std::to_string(i) + ".png) can't load" << std::endl;
				return;
			}
			backPhoto.setTexture(backPhotoTex);
			++i;
			clock.restart();
		}
		//=================================

		if (sf::IntRect(static_cast<int>(WIDTH / 6.f) - 100, static_cast<int>(HEIGHT / 2.f) - 60, 300, 50).contains(sf::Mouse::getPosition(window)))
		{
			buttonPhoto.setPosition(WIDTH / 6.f - 125, HEIGHT / 2.f - 60);
			buttonPhoto.setScale(1.25, 1.f);
			menu1.setFillColor(sf::Color(169, 233, 251));
			menu1.setStyle(sf::Text::Bold);
			menuNum = 1;
		}
		else if (sf::IntRect(static_cast<int>(WIDTH / 6.f) - 100, static_cast<int>(HEIGHT / 2.f), 300, 50).contains(sf::Mouse::getPosition(window)))
		{
			buttonPhoto.setPosition(WIDTH / 6.f - 125, HEIGHT / 2.f);
			buttonPhoto.setScale(0.8f, 1.f);
			menu2.setFillColor(sf::Color(169, 233, 251));
			menu2.setStyle(sf::Text::Bold);
			menuNum = 2;
		}
		else if (sf::IntRect(static_cast<int>(WIDTH / 6.f) - 100, static_cast<int>(HEIGHT / 2.f) + 60, 300, 50).contains(sf::Mouse::getPosition(window)))
		{
			buttonPhoto.setPosition(WIDTH / 6.f - 125, HEIGHT / 2.f + 60);
			buttonPhoto.setScale(0.8f, 1.f);
			menu3.setFillColor(sf::Color(169, 233, 251));
			menu3.setStyle(sf::Text::Bold);
			menuNum = 3;
		}
		else
		{
			buttonPhoto.setPosition(-300.f, -300.f);
			menuNum = 0;
		}

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.key.code == sf::Mouse::Left)
			{
				switch (menuNum)
				{
				case 1:
					isMenu = false;
					break;
				case 2:
					// TODO: create picture "about";

					// window.pushGLStates();
					// window.draw(about);
					// window.popGLStates();
					// window.display();
					// while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape));
					break;
				case 3:
					isMenu = false;
					window.close();
					break;
				}
			}

			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
			{
				isMenu = false;
				window.close();
			}
		}

		glClear(GL_DEPTH_BUFFER_BIT);

		window.pushGLStates();
		window.draw(backPhoto);
		window.popGLStates();

		window.pushGLStates();
		window.draw(back);
		window.popGLStates();

		if (menuNum)
		{
			buttonPhoto.setColor(sf::Color(255, 255, 255, 150));
			window.pushGLStates();
			window.draw(buttonPhoto);
			window.popGLStates();
		}

		window.pushGLStates();
		window.draw(menu1);
		window.popGLStates();

		window.pushGLStates();
		window.draw(menu2);
		window.popGLStates();

		window.pushGLStates();
		window.draw(menu3);
		window.popGLStates();

		window.display();
	}

	glClear(GL_DEPTH_BUFFER_BIT);
	window.clear();
	ShowCursor(false);
}
>>>>>>> parent of d70a925... Update menu

#endif _WORLD_HPP_