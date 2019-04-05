#ifndef _WORLD_HPP_
#define _WORLD_HPP_

const float PI = 3.141592653f;
float size = 20.f;
const int HEIGHT = sf::VideoMode::getDesktopMode().height;
const int WIDTH = sf::VideoMode::getDesktopMode().width;
float angleX, angleY; // Angle of rotate of camera

class WORLD
{
	const int worldSizeXZ, worldSizeY;
	bool world[300][50][300];
	bool *** worlds;
public:
	WORLD(int _worldSizeXZ = 1000, int _worldSizeY = 200) : worldSizeXZ(_worldSizeXZ), worldSizeY(_worldSizeY)
	{
		worlds = new bool ** [_worldSizeXZ];
		for (int i = 0; i < _worldSizeXZ; ++i)
		{
			worlds[i] = new bool *[_worldSizeY];
			for (int j = 0; j < _worldSizeY; ++j)
				worlds[i][j] = new bool[_worldSizeXZ];
		}
	}
	~WORLD() 
	{
		for (int i = 0; i < worldSizeXZ; ++i)
		{
			for (int j = 0; j < worldSizeY; ++j)
				delete[] worlds[i][j];
			delete[] worlds[i];
		}
		delete[] worlds;
	}

	bool check(int x, int y, int z)
	{
		if ((x < 0) || (x >= worldSizeXZ) ||
			(y < 0) || (y >= worldSizeY) ||
			(z < 0) || (z >= worldSizeXZ))
			return false;

		return world[x][y][z];
	}

	void setBox(int x, int y, int z, bool set)
	{
		if ((x < 0) || (x >= worldSizeXZ) ||
			(y < 0) || (y >= worldSizeY) ||
			(z < 0) || (z >= worldSizeXZ))
			return;

		world[x][y][z] = set;
	}

	// MODE:	0 - the super flat world
	//			1 - the meteors world
	//			2 - the normal world
	void worldGeneration(int mode)
	{
		switch (mode)
		{
		case 0:
			for (int x = 0; x < worldSizeXZ; ++x)
				for (int z = 0; z < worldSizeXZ; ++z)
					for (int y = 0; y < 4; ++y)
						world[x][y][z] = true;
			break;
		case 1:
			srand(static_cast<unsigned int>(time(nullptr)));
			{
				unsigned short int prev = 1, current = 20;
				for (int x = 0; x < worldSizeXZ; ++x)
					for (int z = 0; z < worldSizeXZ; ++z)
					{
						current = (current - prev) + rand() % current;
						for (int y = 0; y < current; ++y)
							world[x][y][z] = true;
						prev = current - 1;
						current++;
					}
			}
			break;
		case 2:
			sf::Image im;
			if (!im.loadFromFile("resources/heightmap.png"))
			{
				std::cerr << "Error! File can't load" << std::endl;
				return;
			}

			for (int x = 0; x < 256; ++x)
				for (int z = 0; z < 256; ++z)
				{
					int c = static_cast<int>(im.getPixel(x, z).r / 15.);
					for (int y = 0; y < c; ++y)
						world[x][y][z] = true;
				}
			break;
		}
	}
};

class WORLD world;

class Player
{
public:
	float x, y, z;
	float dx, dy, dz;
	float w, h, d;  // width, height, depth 
	bool onGround;
	float speed;

	Player(float x0, float y0, float z0) : x(x0), y(y0), z(z0)
	{
		dx = 0.f; dy = 0.f; dz = 0.f;
		w = 5.f; h = 20.f; d = 5.f; speed = 5.f;
		onGround = false;
	}

	void update(float time)
	{
		if (!onGround)
			dy -= 1.5f * time;
		onGround = false;

		x += dx * time;
		collision(dx, 0.f, 0.f);
		y += dy * time;
		collision(0.f, dy, 0.f);
		z += dz * time;
		collision(0.f, 0.f, dz);

		dx = dz = 0.f;
	}

	void collision(float Dx, float Dy, float Dz)
	{
		for (int X = static_cast<int>((x - w) / size); X < (x + w) / size; X++)
		{
			for (int Y = static_cast<int>((y - h) / size); Y < (y + h) / size; Y++)
			{
				for (int Z = static_cast<int>((z - d) / size); Z < (z + d) / size; Z++)
				{
					if (world.check(X, Y, Z))
					{
						if (Dx > 0.f)  x = X * size - w;
						if (Dx < 0.f)  x = X * size + size + w;
						if (Dy > 0.f)  y = Y * size - h;
						if (Dy < 0.f) { y = Y * size + size + h; onGround = true; dy = 0.f; }
						if (Dz > 0.f)  z = Z * size - d;
						if (Dz < 0.f)  z = Z * size + size + d;
					}
				}
			}
		}
	}

	void keyboard(void)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			if (onGround)
			{
				onGround = false;
				dy = 12.f;
			}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			dx = static_cast<float>(-sin(angleX / 180.f * PI)) * speed;
			dz = static_cast<float>(-cos(angleX / 180.f * PI)) * speed;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			dx = static_cast<float>(sin(angleX / 180.f * PI)) * speed;
			dz = static_cast<float>(cos(angleX / 180.f * PI)) * speed;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			dx = static_cast<float>(sin((angleX + 90.f) / 180.f * PI)) * speed;
			dz = static_cast<float>(cos((angleX + 90.f) / 180.f * PI)) * speed;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			dx = static_cast<float>(sin((angleX - 90.f) / 180.f * PI)) * speed;
			dz = static_cast<float>(cos((angleX - 90.f) / 180.f * PI)) * speed;
		}
	}
};

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

	sf::Texture startGameTexture, exitTexture, aboutTexture;
	if (!startGameTexture.loadFromFile("resources/menu/start.png"))
	{
		std::cerr << "Error! File (start.png) can't load" << std::endl;
		return;
	}
	if(!aboutTexture.loadFromFile("resources/menu/about.png"))
	{
		std::cerr << "Error! File (about.png) can't load" << std::endl;
		return;
	}
	if(!exitTexture.loadFromFile("resources/menu/exit.png"))
	{
		std::cerr << "Error! File (exit.png) can't load" << std::endl;
		return;
	}

	sf::Sprite menu1(startGameTexture), menu2(aboutTexture), menu3(exitTexture);
	menu1.setPosition(WIDTH / 2.f - 150, HEIGHT / 2.f - 60.f);
	menu2.setPosition(WIDTH / 2.f - 150, HEIGHT / 2.f);
	menu3.setPosition(WIDTH / 2.f - 150, HEIGHT / 2.f + 60.f);

	while (isMenu)
	{
		menuNum = 0;
		window.clear(sf::Color(129, 181, 221));

		if (sf::IntRect(static_cast<int>(WIDTH / 2.f) - 150, static_cast<int>(HEIGHT / 2.f) - 60, 300, 50).contains(sf::Mouse::getPosition(window)))
			menuNum = 1;
		if (sf::IntRect(static_cast<int>(WIDTH / 2.f) - 150, static_cast<int>(HEIGHT / 2.f), 300, 50).contains(sf::Mouse::getPosition(window)))
			menuNum = 2;
		if (sf::IntRect(static_cast<int>(WIDTH / 2.f) - 150, static_cast<int>(HEIGHT / 2.f) + 60, 300, 50).contains(sf::Mouse::getPosition(window)))
			menuNum = 3;

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
						window.close();
						isMenu = false;
					break;
				}
			}

			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
				window.close();
		}

		glClear(GL_DEPTH_BUFFER_BIT);

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

#endif _WORLD_HPP_