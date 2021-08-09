#include "Scroll.hpp"
#include <algorithm>

#include "Page.hpp"

gui::Scroll::Scroll()
	:Entity(__GUI_ID_SCROLL)
{
	rect.setPointCount(4);
	rect.setOriginalFillColor(sf::Color(41, 41, 41));
	rect.setOriginalOutlineColor(sf::Color::Transparent);
	rect.setHighlightFillColor(sf::Color(60, 60, 60));
	rect.setHighlightOutlineColor(sf::Color::Transparent);
	
	bar.setPointCount(4);
	bar.setOriginalFillColor(sf::Color(150, 150, 150, 70));
	bar.setOriginalOutlineColor(sf::Color::Transparent);
	bar.setHighlightFillColor(sf::Color(200, 200, 200, 70));
	bar.setHighlightOutlineColor(sf::Color::Transparent);

	rect.actionEvent = ActionEvent::PRESS;
	bar.actionEvent = ActionEvent::MOUSEHELD;

	scrollPos = RIGHT;
}

gui::Scroll::~Scroll()
{
}

const sf::Vector2f& gui::Scroll::getSize() const
{
	return rect.getSize();
}

sf::Vector2f gui::Scroll::getPosition() const
{
	return rect.getPosition();
}

void gui::Scroll::setPage(Page& page, ScrollPosition scrollPos)
{
	this->scrollPos = scrollPos;
	auto bound = page.getActiveRegion();
	switch (scrollPos)
	{
	case TOP: case LEFT: rect.setPosition(0, 0); break;
	case RIGHT: rect.setPosition(bound.width - 10, 0); break;
	case BOTTOM: rect.setPosition(0, bound.height - 10); break;
	}
	if (scrollPos == LEFT || scrollPos == RIGHT) {
		rect.setPoint(0, sf::Vector2f(0, 0));
		rect.setPoint(1, sf::Vector2f(10, 0));
		rect.setPoint(2, sf::Vector2f(10, bound.height));
		rect.setPoint(3, sf::Vector2f(0, bound.height));

		sf::Vector2f barSize(sf::Vector2f(rect.getSize().x, rect.getSize().y * rect.getSize().y / page.getMaxSize().y));
		bar.setPoint(0, sf::Vector2f(0, 0));
		bar.setPoint(1, sf::Vector2f(barSize.x, 0));
		bar.setPoint(2, barSize);
		bar.setPoint(3, sf::Vector2f(0, barSize.y));

		bar.setPosition(rect.getPosition() + sf::Vector2f(0, bound.top * rect.getSize().y / page.getMaxSize().y));
	}
	else if (scrollPos == BOTTOM || scrollPos == TOP) {
		rect.setPoint(0, sf::Vector2f(0, 0));
		rect.setPoint(1, sf::Vector2f(bound.width, 0));
		rect.setPoint(2, sf::Vector2f(bound.width, 10));
		rect.setPoint(3, sf::Vector2f(0, 10));

		sf::Vector2f barSize(sf::Vector2f(rect.getSize().x * rect.getSize().x / page.getMaxSize().x, rect.getSize().y));
		bar.setPoint(0, sf::Vector2f(0, 0));
		bar.setPoint(1, sf::Vector2f(barSize.x, 0));
		bar.setPoint(2, barSize);
		bar.setPoint(3, sf::Vector2f(0, barSize.y));

		bar.setPosition(rect.getPosition() + sf::Vector2f(bound.left * rect.getSize().x / page.getMaxSize().x, 0));
	}
}

void gui::Scroll::move(float offsetX, float offsetY)
{
	bar.move(offsetX, offsetY);
	rect.move(offsetX, offsetY);
}

void gui::Scroll::move(const sf::Vector2f& offset)
{
	bar.move(offset);
	rect.move(offset);
}

sf::Vector2f gui::Scroll::scrollTo(const sf::Vector2f& mousePos)
{
	if (scrollPos == TOP || scrollPos == BOTTOM) {
		float oldX = bar.getPosition().x;
		float newX = mousePos.x - bar.getSize().x * 0.5f;
		newX = std::max(rect.getPosition().x, newX);
		newX = std::min(rect.getPosition().x + rect.getSize().x - bar.getSize().x, newX);

		scrollBy({ newX - oldX, 0 });
		return { newX - oldX, 0 };
	}
	else {
		float oldY = bar.getPosition().y;
		float newY = mousePos.y - bar.getSize().y * 0.5f;
		newY = std::max(rect.getPosition().y, newY);
		newY = std::min(rect.getPosition().y + rect.getSize().y - bar.getSize().y, newY);

		scrollBy({ 0, newY - oldY });
		return { 0, newY - oldY };
	}
}

sf::Vector2f gui::Scroll::scrollBy(const sf::Vector2f& offset)
{

	bar.move(offset);

	if (bar.getPosition().x < rect.getPosition().x || bar.getPosition().x + bar.getSize().x > rect.getPosition().x + rect.getSize().x
		|| bar.getPosition().y < rect.getPosition().y || bar.getPosition().y + bar.getSize().y > rect.getPosition().y + rect.getSize().y)
		bar.move(-offset);

	else return offset;

	return { 0,0 };
}

gui::Entity* gui::Scroll::isHit(const sf::Vector2f& mousePos)
{
	if (isActive()) {
		return bar.getGlobalBounds().contains(mousePos) ? &bar : (rect.getGlobalBounds().contains(mousePos) ? &rect : nullptr);
	}
	return nullptr;
}

void gui::Scroll::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isActive()) {
		rect.draw(target);
		bar.draw(target);
	}
}

void gui::Scroll::activateSelection()
{
	isSelected = true;
}

void gui::Scroll::deactivateSelection()
{
	isSelected = false;
}
