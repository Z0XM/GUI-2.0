#include "Renderer.hpp"
#include <iostream>

void Renderer::initWindow()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;

	window.create(sf::VideoMode(1200, 700), "GUI 2.0", sf::Style::Default, settings);
	window.setFramerateLimit(60);
}

Renderer::Renderer()
{
	this->initWindow();

	running = true;
	paused = false;

	font.loadFromFile("data/consola.ttf");

	frame.setWindow(window);

	button.setSize({ 80, 40 });
	button.setPosition(20, 5);
	button.setFillColor(sf::Color::Transparent);
	button.setOutlineColor(sf::Color(160, 32, 240));
	button.setOutlineThickness(3);
	button.setHighlightFillColor(sf::Color(160, 32, 240, 126));
	button.setHighlightOutlineColor(sf::Color(124, 163, 251));
	button.setHighlightOutlineThickness(5);
	button.setCharacterSize(22);
	button.setTextFillColor(sf::Color::White);
	button.setFont(font);
	button.setString("Button");

	text.setSize({ 80, 40 });
	text.setPosition(400, 5);
	text.setFillColor(sf::Color::Transparent);
	text.setOutlineColor(sf::Color(160, 32, 240));
	text.setOutlineThickness(3);
	text.setCharacterSize(22);
	text.setTextFillColor(sf::Color::White);
	text.setFont(font);
	text.setString("Button");
	page.addEntity(text);

	button2.copy(button);
	button2.setString("button2");
	button2.setPosition(120, 5);

	button3.copy(button);
	button3.setString("button3");
	button3.setPosition(210, 5);

	page.setSize({ this->getWinSize().x * 1.5f , 500 });
	page.setActiveRegion({ 300, 0, 500, 300});
	page.setPosition(0, getWinSize().y - 20 - 400);

	page.addEntity(button);
	page.addEntity(button2);
	page.addEntity(button3);
	frame.addEntity(page);

	button3.setOutlineColor(sf::Color::Red);
	button3.setHighlightFillColor(sf::Color(255, 0, 0, 126));
	button3.setHighlightOutlineColor(sf::Color(255, 165, 0));
	
	slider.setSize({ 80, 40 });
	slider.setPosition(300, 5);
	slider.setFillColor(sf::Color::Transparent);
	slider.setOutlineColor(sf::Color::Blue);
	slider.setOutlineThickness(3);
	slider.setTextFillColor(sf::Color::White);
	slider.setFont(font);
	slider.setCharacterSize(22);
	slider.setString("Speed");
	slider.setAction([this]() {
		slider.moveSliderBar(page.getMousePosition());
	});
	page.addEntity(slider);

	page.setScroll(gui::Scroll::BOTTOM);
	page.setScroll(gui::Scroll::RIGHT);
	page.setScroll(gui::Scroll::TOP);
	page.setScroll(gui::Scroll::LEFT);
	page.removeScroll(gui::Scroll::LEFT);
	page.setName(button2, "b");

	page.getByName("b")->setAction([] {std::cout << "hello"; });
	page.setPosition(50, page.getPosition().y);
}

Renderer::~Renderer()
{
}

bool Renderer::isRunning()
{
	return this->running;
}

void Renderer::update()
{
	this->pollEvents();

	if (!this->paused) {
		frame.update();
	}
}

void Renderer::pollEvents()
{
	sf::Event event;

	while (this->window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			this->running = false;

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)this->paused = !this->paused;

		if (!this->paused) {
			frame.pollEvents(event);
			if (event.type == sf::Event::MouseMoved) {
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
				
			}
			else if (event.type == sf::Event::KeyPressed) {

			}
		}
	}
}

void Renderer::render()
{
	this->window.clear();

	frame.draw();

	this->window.display();
}

sf::Vector2f Renderer::getMousePosition()
{
	return window.mapPixelToCoords(sf::Mouse::getPosition(this->window));
}

sf::Vector2f Renderer::getWinSize()
{
	return sf::Vector2f(this->window.getSize());
}