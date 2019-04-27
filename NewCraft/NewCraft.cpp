#include "stdafx.h"

float angleX = 0.f, angleY = 0.f;
WORLD * WORLD::p_instance = nullptr;

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

	//TODO: write menu for choose the mode of world generatation
	WORLD::getInstance()->worldGeneration(NORMAL);

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

	std::random_device r;
	std::default_random_engine e1(r());
	std::uniform_int_distribution<uint16_t> uniform_dist(1, 10);
	uint16_t skyBoxRand = uniform_dist(e1);
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

		angleX += (static_cast<int>(xt) - mousexy.x) / 4.f; // 4 - sensetive
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

			uint16_t X, Y, Z, oldX, oldY, oldZ;
			for (uint16_t dist = 0; dist < 120; ++dist) // distance of power
			{
				x += static_cast<float>(-sin(angleX / 180.f * PI));
				X = static_cast<uint16_t>(x / size);
				y += static_cast<float>(tan(angleY / 180.f * PI));
				Y = static_cast<uint16_t>(y / size);
				z += static_cast<float>(-cos(angleX / 180.f * PI));
				Z = static_cast<uint16_t>(z / size);

				if (WORLD::getInstance()->check(X, Y, Z))
				{
					if (mLeft)
					{
						WORLD::getInstance()->setBox(X, Y, Z, false);
						break;
					}
					else
					{
						WORLD::getInstance()->setBox(oldX, oldY, oldZ, true);
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
		uint16_t radius = 15;
		int16_t X = static_cast<int16_t>(p.getX() / size),
			Y = static_cast<int16_t>(p.getY() / size),
			Z = static_cast<int16_t>(p.getZ() / size);
		
		for (int16_t x = X - radius; x < X + radius; ++x)
			for (int16_t y = Y - radius; y < Y + radius; ++y)
				for (int16_t z = Z - radius; z < Z + radius; ++z)
				{
					if (!WORLD::getInstance()->check(x, y, z))
						continue;

					glTranslatef(size * x + size / 2.f, size * y + size / 2.f, size * z + size / 2.f);
					createBox(box, size / 2.f);
					glTranslatef(-size * x - size / 2.f, -size * y - size / 2.f, -size * z - size / 2.f);
				}

		glTranslatef(p.getX(), p.getY(), p.getZ());
		createBox(skybox, 1000.f);
		glTranslatef(-p.getX(), -p.getY(), -p.getZ());

		window.pushGLStates();
		window.draw(s);
		window.popGLStates();

		window.display();
	}

	WORLD::DestroyInstance();
	return 0;
}