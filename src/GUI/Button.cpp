#include "Button.hpp"

gui::Button::Button()
	:Entity(__GUI_ID_BUTTON)
{
}

gui::Button::~Button()
{
}

void gui::Button::copy(Button& button)
{
	highlight = button.highlight;
	original = button.original;

	this->actionEvent = button.actionEvent;
	this->action = button.action;
	this->setPointCount(button.getPointCount());
	for (int i = 0; i < getPointCount(); i++) {
		this->setPoint(i, button.getPoint(i));
	}
	this->setPosition(button.getPosition());
	this->setRotation(button.getRotation());
	this->setScale(button.getScale());
	this->setOrigin(button.getOrigin());
}

void gui::Button::setHighlightFillColor(const sf::Color& color)
{
	highlight.fillColor = color;
	if (isSelected)setFillColor(color);
}

void gui::Button::setHighlightOutlineColor(const sf::Color& color)
{
	highlight.outlineColor = color;
	if (isSelected)setOutlineColor(color);
}

void gui::Button::setHighlightOutlineThickness(float thickness)
{
	highlight.outlineThickness = thickness;
	if (isSelected)setOutlineThickness(thickness);
}

void gui::Button::setOriginalFillColor(const sf::Color& color)
{
	original.fillColor = color;
	if (!isSelected)setFillColor(color);
}

void gui::Button::setOriginalOutlineColor(const sf::Color& color)
{
	original.outlineColor = color;
	if (!isSelected)setOutlineColor(color);
}

void gui::Button::setOriginalOutlineThickness(float thickness)
{
	original.outlineThickness = thickness;
	if (!isSelected)setOutlineThickness(thickness);
}

sf::Vector2f gui::Button::getSize() const
{
	return sf::Vector2f(getGlobalBounds().width, getGlobalBounds().height);
}

bool gui::Button::contains(const sf::Vector2f& mousePos) const
{
	return getGlobalBounds().contains(mousePos);
}

gui::Entity* gui::Button::isHit(const sf::Vector2f& mousePos)
{
	if (isActive()) {
		return this->contains(mousePos) ? this : nullptr;
	}
	return nullptr;
}

void gui::Button::activateSelection()
{
	isSelected = true;
	setFillColor(highlight.fillColor);
	setOutlineColor(highlight.outlineColor);
	setOutlineThickness(highlight.outlineThickness);
}

void gui::Button::deactivateSelection()
{
	isSelected = false;
	setFillColor(original.fillColor);
	setOutlineColor(original.outlineColor);
	setOutlineThickness(original.outlineThickness);
}

void gui::Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isActive()) {
		target.draw((sf::ConvexShape) * this, states);
	}
}