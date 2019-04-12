#include "stdafx.h"
#include "world.hpp"

int main(int argc, char** argv)
{
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT, 32), "NewCraft", sf::Style::Fullscreen, sf::ContextSettings(24));
	menu(window);
	//===
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClearDepth(1.);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90., 1., 1., 2000.);
	glEnable(GL_TEXTURE_2D);
	ShowCursor(false);

	world.worldGeneration(2);
	//=== Textures
	sf::Texture t;
	if (!t.loadFromFile("resources/cursor.png"))
	{
		std::cerr << "Error! File can't load" << std::endl;
		return 1;
	}
	sf::Sprite s(t);
	s.setOrigin(8.f, 8.f);
	s.setPosition(WIDTH / 2.f, HEIGHT / 2.f);

	srand(static_cast<unsigned int>(time(nullptr)));
	unsigned int skyBoxRand = 1 + rand() % 10;
	if (skyBoxRand > 3)
		skyBoxRand = 3;
	GLuint skybox[6];
	skybox[0] = LoadTexture("resources/skyBoxs/skybox" + std::to_string(skyBoxRand) + "/skybox_front.bmp");
	skybox[1] = LoadTexture("resources/skyBoxs/skybox" + std::to_string(skyBoxRand) + "/skybox_back.bmp");
	skybox[2] = LoadTexture("resources/skyBoxs/skybox" + std::to_string(skyBoxRand) + "/skybox_left.bmp");
	skybox[3] = LoadTexture("resources/skyBoxs/skybox" + std::to_string(skyBoxRand) + "/skybox_right.bmp");
	skybox[4] = LoadTexture("resources/skyBoxs/skybox" + std::to_string(skyBoxRand) + "/skybox_bottom.bmp");
	skybox[5] = LoadTexture("resources/skyBoxs/skybox" + std::to_string(skyBoxRand) + "/skybox_top.bmp");
	GLuint box[6];
	box[0] = LoadTexture("resources/grassBox/side.jpg");
	box[1] = LoadTexture("resources/grassBox/side.jpg");
	box[2] = LoadTexture("resources/grassBox/side.jpg");
	box[3] = LoadTexture("resources/grassBox/side.jpg");
	box[4] = LoadTexture("resources/grassBox/bottom.jpg");
	box[5] = LoadTexture("resources/grassBox/top.jpg");

	sf::Clock clock;
	bool mLeft = false, mRight = false;
	Player p(200.f, 200.f, 200.f);

	while (window.isOpen())
	{
		float time = static_cast<float>(clock.getElapsedTime().asMilliseconds());
		clock.restart();
		time /= 50.f;
		if (time > 3.f)
			time = 3.f;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
				menu(window);

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.key.code == sf::Mouse::Right)
					mRight = true;
				if (event.key.code == sf::Mouse::Left)
					mLeft = true;
			}
		}

		glClear(GL_DEPTH_BUFFER_BIT);

		p.keyboard();
		p.update(time);

		//=== Settings of the mouse
		POINT mousexy;
		GetCursorPos(&mousexy);
		double xt = window.getPosition().x + 400.;
		double yt = window.getPosition().y + 300.;

		angleX += (static_cast<int>(xt) - mousexy.x) / 4.f; // 4 Ч чувствительность 
		angleY += (static_cast<int>(yt) - mousexy.y) / 4.f;

		if (angleY < -89.0f)
			angleY = -89.0f;
		if (angleY > 89.0f)
			angleY = 89.0f;

		SetCursorPos(static_cast<int>(xt), static_cast<int>(yt));
		//===================

		if (mRight || mLeft)
		{
			float x = p.getX();
			float y = p.getY() + p.getH() / 2.f;
			float z = p.getZ();

			int X, Y, Z, oldX, oldY, oldZ;
			for (int dist = 0; dist < 120; ++dist) // радиус действи€
			{
				x += static_cast<float>(-sin(angleX / 180.f * PI));
				X = static_cast<int>(x / size);
				y += static_cast<float>(tan(angleY / 180.f * PI));
				Y = static_cast<int>(y / size);
				z += static_cast<float>(-cos(angleX / 180.f * PI));
				Z = static_cast<int>(z / size);

				if (world.check(X, Y, Z))
				{
					if (mLeft)
					{
						world.setBox(X, Y, Z, false);
						break;
					}
					else
					{
						world.setBox(oldX, oldY, oldZ, true);
						break;
					}
				}

				oldX = X; oldY = Y; oldZ = Z;
			}
		}

		mLeft = mRight = false;

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(p.getX(), p.getY() + p.getH() / 2, p.getZ(), p.getX() - sin(angleX / 180 * PI), p.getY() + p.getH() / 2 + (tan(angleY / 180 * PI)), p.getZ() - cos(angleX / 180 * PI), 0., 1., 0.);

		//=== Draw boxes
		int R = 15;
		int X = static_cast<int>(p.getX() / size);
		int Y = static_cast<int>(p.getY() / size);
		int Z = static_cast<int>(p.getZ() / size);

		for (int x = X - R; x < X + R; x++)
		{
			for (int y = 0; y < 25; y++)
			{
				for (int z = Z - R; z < Z + R; z++)
				{
					if (!world.check(x, y, z))
						continue;

					glTranslatef(size * x + size / 2.f, size * y + size / 2.f, size * z + size / 2.f);
					createBox(box, size / 2.f);
					glTranslatef(-size * x - size / 2.f, -size * y - size / 2.f, -size * z - size / 2.f);
				}
			}
		}

		glTranslatef(p.getX(), p.getY(), p.getZ());
		createBox(skybox, 1000.f);
		glTranslatef(-p.getX(), -p.getY(), -p.getZ());

		window.pushGLStates();
		window.draw(s);
		window.popGLStates();

		window.display();
	}

	return 0;
}