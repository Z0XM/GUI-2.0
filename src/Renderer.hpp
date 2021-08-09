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

	gui::Button rectB;
	gui::TextButton button, button2, button3, dbutton1, dbutton2, dbutton3, dbutton4, dbutton5, dbutton6;
	gui::Page page;
	gui::Slider slider;
	gui::Textbox text;
	gui::Dropdown drop;

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
