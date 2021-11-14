#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>
#include <queue>

#include <Windows.h>
using namespace sf;

static const sf::Vector2i Pos(541, 256);

enum class Direct {
	Up,
	Down,
	Left,
	Right
};

class Block : public Sprite {
public:
	Block(void) {
		t.loadFromFile("./assets/block.png");
		this->setTexture(t);
	}
private:
	Texture t;
};

class Food {
public:
	Food() : rotation(0, 0) {
		food.setColor(Color(255, 165, 0));
	}
	void gen(void) {
		this->rotation.x = rand() % 36 * 15;
		this->rotation.y = rand() % 15 * 15;
		food.move(rotation);
	}
	Vector2f& getRotation(void) {
		return rotation;
	}
	operator Block&() {
		return food;
	}
private:
	Block food;
	Vector2f rotation;
};

class Worm {
public:
	Worm(Vector2f r = Vector2f(15.f, 15.f)) {
		rotation = r;

		Block tmp;
		tmp.setColor(Color(225, 37, 40));

		body.push_back(tmp);
		dir.push(Direct::Right);
	}

	void feed(void) {
		Block tmp;
		tmp.setColor(Color(225, 37, 40));

		body.push_back(tmp);
	}
	void move(RenderWindow& w, Food& f) {
		if (Keyboard::isKeyPressed(Keyboard::W)) {
			for (int i = 0; i < body.size(); ++i) {
				dir.push(Direct::Up);
			}
		}
		else if (Keyboard::isKeyPressed(Keyboard::A)) {
			for (int i = 0; i < body.size(); ++i) {
				dir.push(Direct::Left);
			}
		}
		else if (Keyboard::isKeyPressed(Keyboard::S)) {
			for (int i = 0; i < body.size(); ++i) {
				dir.push(Direct::Down);
			}
		}
		else if (Keyboard::isKeyPressed(Keyboard::D)) {
			for (int i = 0; i < body.size(); ++i) {
				dir.push(Direct::Right);
			}
		}
		else {
			for (int i = 0; i < body.size(); ++i) {
				dir.push(dir.front());
			}
		}
		
		switch (dir.front()) {
		case Direct::Up:
			rotation.y -= 15;
			break;
		case Direct::Down:
			rotation.y += 15;
			break;
		case Direct::Left:
			rotation.x -= 15;
			break;
		case Direct::Right:
			rotation.x += 15;
			break;
		default:
			break;
		}

		if (rotation.x > Pos.x) {
			rotation.x = 0;
		}
		if (rotation.x < 0) {
			rotation.x = Pos.x-1;
		}
		if (rotation.y > Pos.y) {
			rotation.y = 0;
		}
		if (rotation.y < 0) {
			rotation.y = Pos.y;
		}

		if (this->rotation == f.getRotation()) {
			feed();
		}

		for (int i = 0; i < body.size(); ++i) {
			body[i].setPosition(rotation);
			dir.pop();
		}
		for (int i = 0; i < body.size(); ++i) {
			w.draw(body[i]);
		}
		Sleep(170);
	}
	Vector2f& getRotation(void) {
		return rotation;
	}
private:
	Vector2f rotation; // head
	std::vector<Block> body;
	std::queue<Direct> dir;
};

void generate_map(RenderWindow& win) {
	float r[] = { win.getSize().x, win.getSize().y };

	Block b;
	for (auto x = 0.f; x <= r[0]; x += 15) {
		for (auto y = 0.f; y <= r[1]; y += 15) {
			win.draw(b);
			b.setPosition(Vector2f(x, y));
		}
	}
}

int main() {
	srand(static_cast<unsigned int>(time(nullptr)));

	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, HIDE_WINDOW);

	RenderWindow window(VideoMode(Pos.x, Pos.y), "App", Style::None);
	window.setFramerateLimit(60);

	Food food;
	food.gen();

	Worm worm;
	while (window.isOpen()) {
		Event e;
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed) {
				window.close();
			}
			if (Keyboard::isKeyPressed(Keyboard::Escape)) {
				window.close();
			}
		}
		window.clear(Color::White);
		generate_map(window);
		window.draw(food);

		// worm moving.
		worm.move(window, food);

		window.display();
	}
	return 0;
}