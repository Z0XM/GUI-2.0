#pragma once

#include "GUIFrame.hpp"
#include "Textbox.hpp"
#include "TextButton.hpp"
#include "Button.hpp"
#include "Page.hpp"
#include <vector>

namespace gui {
	class Dropdown : public Entity {
	private:
		std::vector<unsigned int> itemIDs;
		int itemLimit, itemCount;
		sf::Vector2f itemSize;

		TextButton header;
		Button collapse;

		Page itemPage;

		void setHeader();
	public:
		Dropdown(const sf::Vector2f& headerSize = sf::Vector2f(0,0));
		Dropdown(const sf::Vector2f& headerSize, float itemHeight, int itemLimit = 3);
		~Dropdown();

		void copy(Dropdown& dropdown);

		void setHeaderSize(const sf::Vector2f& headerSize);
		void setItemLimit(int limit);
		void setItemHeight(float height);

		void setTitle(const std::string& title, sf::Font& font, int charSize);
		void setFillColor(sf::Color color);
		void setOutineColor(sf::Color color);

		sf::Vector2f getMaxSize() const;
		sf::Vector2f getSize() const;

		void setPosition(float x, float y);
		void setPosition(const sf::Vector2f& position);
		sf::Vector2f getPosition() const;

		void move(float offsetX, float offsetY);
		void move(const sf::Vector2f& offset);

		void addItem(Textbox& textbox);
		void insertItem(int where, Textbox& textbox);
		void eraseItem(int where);

		bool contains(const sf::Vector2f& mousePos);
		bool containsExcludingHeader(const sf::Vector2f& mousePos);
		Entity* isHit(const sf::Vector2f& mousePos);

		bool pollEvents(sf::Event event);
		void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;

		void setAction(std::function<void()> func) = delete;

		void activateSelection();
		void deactivateSelection();
	};
}