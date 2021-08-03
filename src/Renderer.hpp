#pragma once
#include <SFML/Graphics.hpp>
#include "GUI.hpp"

class Renderer {
private:
	sf::RenderWindow window;
	bool running;
	bool paused;

	gui::Frame frame;
	sf::Font font;

	gui::Button button, button2, button3;
	gui::Page page;
	gui::Slider slider;
	gui::Textbox text;

	void initWindow();
public:
	Renderer();
	~Renderer();

	bool isRunning();

	void update();
	void pollEvents();
	void render();

	sf::Vector2f getMousePosition();
	sf::Vector2f getWinSize();
};
