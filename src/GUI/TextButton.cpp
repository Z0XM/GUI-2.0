#pragma once
#include "TextButton.hpp"

gui::TextButton::TextButton(const sf::Vector2f& size)
	:Textbox(size, __GUI_ID_TEXTBUTTON)
{
	action = nullptr;
	actionEvent = ActionEvent::RELEASE;
}

gui::TextButton::~TextButton()
{
}

void gui::TextButton::copy(const TextButton& button)
{
	box = button.box;
	text = button.text;
	alignment = button.alignment;

	action = nullptr;
	actionEvent = ActionEvent::RELEASE;
	original = button.original;
	highlight = button.highlight;
}

void gui::TextButton::setHighlightFillColor(const sf::Color& color)
{
	highlight.fillColor = color;
	if (isSelected)box.setFillColor(color);
}

void gui::TextButton::setHighlightOutlineColor(const sf::Color& color)
{
	highlight.outlineColor = color;
	if (isSelected)box.setOutlineColor(color);
}

void gui::TextButton::setHighlightOutlineThickness(float thickness)
{
	highlight.outlineThickness = thickness;
	if (isSelected)box.setOutlineThickness(thickness);
}

void gui::TextButton::setFillColor(const sf::Color& color)
{
	original.fillColor = color;
	if (!isSelected)box.setFillColor(color);
}

void gui::TextButton::setOutlineColor(const sf::Color& color)
{
	original.outlineColor = color;
	if (!isSelected)box.setOutlineColor(color);
}

void gui::TextButton::setOutlineThickness(float thickness)
{
	original.outlineThickness = thickness;
	if (!isSelected)box.setOutlineThickness(thickness);
}

bool gui::TextButton::contains(const sf::Vector2f& mousePos) const
{
	return box.getGlobalBounds().contains(mousePos);
}

gui::Entity* gui::TextButton::isHit(const sf::Vector2f& mousePos)
{
	if (isActive()) {
		return this->contains(mousePos) ? this : nullptr;
	}
	return nullptr;
}

void gui::TextButton::activateSelection()
{
	isSelected = true;
	box.setFillColor(highlight.fillColor);
	box.setOutlineColor(highlight.outlineColor);
	box.setOutlineThickness(highlight.outlineThickness);
}
void gui::TextButton::deactivateSelection()
{
	isSelected = false;
	box.setFillColor(original.fillColor);
	box.setOutlineColor(original.outlineColor);
	box.setOutlineThickness(original.outlineThickness);
}

