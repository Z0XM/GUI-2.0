#pragma once
#include "GUIFrame.hpp"


namespace gui
{
	class Button : public Entity, public sf::ConvexShape {
	public:
		Button();
		~Button();

		void copy(Button& button);

		void setHighlightFillColor(const sf::Color& color);
		void setHighlightOutlineColor(const sf::Color& color);
		void setHighlightOutlineThickness(float thickness);

		void setOriginalFillColor(const sf::Color& color);
		void setOriginalOutlineColor(const sf::Color& color);
		void setOriginalOutlineThickness(float thickness);

		sf::Vector2f getSize() const;

		bool contains(const sf::Vector2f& mousePos) const;

		Entity* isHit(const sf::Vector2f& mousePos);

		void activateSelection();
		void deactivateSelection();

		void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;

	private:
		struct {
			sf::Color fillColor, outlineColor;
			float outlineThickness;
		} highlight, original;
	};
}