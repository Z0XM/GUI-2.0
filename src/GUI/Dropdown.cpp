#include "Dropdown.hpp"
#include "Scroll.hpp"

gui::Dropdown::Dropdown(const sf::Vector2f& headerSize) :
	Entity(__GUI_ID_DROPDOWN)
{
	this->itemCount = 0;
	this->itemLimit = 3;
	setHeaderSize(headerSize);
	setHeader();

	itemPage.setFillColor(sf::Color::Transparent);

	registerVar(&itemPage);
}

gui::Dropdown::Dropdown(const sf::Vector2f& headerSize, float itemHeight, int itemLimit):
	Entity(__GUI_ID_DROPDOWN)
{
	this->itemCount = 0;
	this->itemLimit = itemLimit;
	setHeaderSize(headerSize);
	setHeader();

	itemPage.setFillColor(sf::Color::Transparent);
	
	registerVar(&itemPage);
}

gui::Dropdown::~Dropdown()
{
}

void gui::Dropdown::copy(Dropdown& drop)
{
	this->action = drop.action;
	this->actionEvent = drop.actionEvent;
	this->collapse = drop.collapse;
	this->header = drop.header;
	this->itemSize = drop.itemSize;
	this->itemLimit = drop.itemLimit;
}

void gui::Dropdown::setHeader()
{
	header.setFillColor(sf::Color::White);
	header.setHighlightFillColor(sf::Color::White);
	header.actionEvent = ActionEvent::MOUSEHELD;
	header.setAction([this] {move(getFrame()->getLastMouseOffset()); });

	collapse.setOriginalFillColor(sf::Color::Red);
	collapse.setHighlightFillColor(sf::Color::Red);
	collapse.setHighlightOutlineColor(sf::Color::Red);
	collapse.setHighlightOutlineThickness(1);
	collapse.actionEvent = ActionEvent::RELEASE;
	collapse.setAction([this] { 
		if (itemPage.isActive()) {
			itemPage.setInactive();
			collapse.rotate(90);
		}
		else {
			itemPage.setActive();
			collapse.rotate(-90);
		}
		});
}

void gui::Dropdown::setHeaderSize(const sf::Vector2f& headerSize)
{
	header.setSize(headerSize);

	float triWidth = header.getSize().y * 0.75f;

	collapse.setPointCount(3);
	collapse.setPoint(0, sf::Vector2f(0, 0));
	collapse.setPoint(1, sf::Vector2f(triWidth, 0));
	collapse.setPoint(2, sf::Vector2f(triWidth * 0.5, triWidth * 0.5f));
	collapse.setOrigin(triWidth * 0.5, triWidth * 0.25);
	collapse.setPosition(header.getPosition().x + header.getSize().x - header.getSize().y * 0.5f,header.getPosition().y + header.getSize().y * 0.5f);

	this->itemSize = sf::Vector2f(headerSize.x - 30, itemSize.y);

	itemPage.setMaxSize({ headerSize.x, itemPage.getMaxSize().y });
	itemPage.setActiveRegion({ itemPage.getActiveRegion().left, itemPage.getActiveRegion().top, headerSize.x, itemPage.getActiveRegion().height });
}

void gui::Dropdown::setItemLimit(int limit)
{
	itemLimit = limit;

	itemPage.setActiveRegion({ itemPage.getActiveRegion().left, itemPage.getActiveRegion().top, itemPage.getActiveRegion().width, std::min(itemLimit, itemCount) * (itemSize.y + 10) + 10 });
}

void gui::Dropdown::setItemHeight(float height)
{
	itemSize.y = height;

	itemPage.setMaxSize({ header.getSize().x, itemCount * (itemSize.y + 10) + 10 });
	itemPage.setActiveRegion({ itemPage.getActiveRegion().left, itemPage.getActiveRegion().top, itemPage.getActiveRegion().width, std::min(itemLimit, itemCount) * (itemSize.y + 10) + 10 });
}

void gui::Dropdown::setTitle(const std::string& title, sf::Font& font, int charSize)
{
	header.setFont(font);
	header.setString(title);
	header.setCharacterSize(charSize);
	header.setTextFillColor(sf::Color::Black);
	header.setStyle(sf::Text::Underlined | sf::Text::Bold);
}

void gui::Dropdown::setFillColor(sf::Color color)
{
	itemPage.setFillColor(color);
}

void gui::Dropdown::setOutineColor(sf::Color color)
{
	itemPage.setOutlineColor(color);
}

sf::Vector2f gui::Dropdown::getSize() const
{
	return header.getSize() + sf::Vector2f(0, itemPage.getMaxSize().y);
}

sf::Vector2f gui::Dropdown::getMaxSize() const
{
	return header.getSize() + sf::Vector2f(0, itemPage.getMaxSize().y);
}

void gui::Dropdown::setPosition(float x, float y)
{
	header.setPosition(x, y);
	collapse.setPosition(header.getPosition().x + header.getSize().x - header.getSize().y * 0.5f, header.getPosition().y + header.getSize().y * 0.5f);

	itemPage.setPosition(x, y + header.getSize().y);
}

void gui::Dropdown::setPosition(const sf::Vector2f& position)
{
	setPosition(position.x, position.y);
}

sf::Vector2f gui::Dropdown::getPosition() const
{
	return header.getPosition();
}

void gui::Dropdown::move(float offsetX, float offsetY)
{
	header.move(offsetX, offsetY);
	collapse.move(offsetX, offsetY);
	itemPage.move(offsetX, offsetY);
}

void gui::Dropdown::move(const sf::Vector2f& offset)
{
	move(offset.x, offset.y);
}

void gui::Dropdown::addItem(Textbox& textbox)
{
	textbox.setPosition(10, itemCount++ * (itemSize.y + 10) + 10);
	textbox.setSize(itemSize);
	itemPage.addEntity(textbox);

	itemPage.setMaxSize({ header.getSize().x, itemCount * (itemSize.y + 10) + 10 });
	itemPage.setActiveRegion({ itemPage.getActiveRegion().left, itemPage.getActiveRegion().top, itemPage.getActiveRegion().width, std::min(itemLimit, itemCount) * (itemSize.y + 10) + 10 });
	itemPage.setScroll(Scroll::ScrollPosition::RIGHT);

	itemIDs.push_back(textbox.getID());
}

void gui::Dropdown::insertItem(int where, Textbox& textbox)
{
	textbox.setPosition(10, where * (itemSize.y + 10) + 10);
	textbox.setSize(itemSize);
	itemPage.addEntity(textbox);

	for (int i = where; i < itemCount; i++) {
		((Textbox*)itemPage.getByID(itemIDs[i]))->move(0, itemSize.y + 10);
	}

	++itemCount;

	itemPage.setMaxSize({ header.getSize().x, itemCount * (itemSize.y + 10) + 10 });
	itemPage.setActiveRegion({ itemPage.getActiveRegion().left, itemPage.getActiveRegion().top, itemPage.getActiveRegion().width, std::min(itemLimit, itemCount) * (itemSize.y + 10) + 10 });
	itemPage.setScroll(Scroll::ScrollPosition::RIGHT);

	itemIDs.insert(itemIDs.begin() + where, textbox.getID());
}

void gui::Dropdown::eraseItem(int where)
{
	itemPage.removeEntity(itemIDs[where]);

	itemIDs.erase(itemIDs.begin() + where);

	--itemCount;

	for (int i = where; i < itemCount; i++) {
		((Textbox*)itemPage.getByID(itemIDs[i]))->move(0, - itemSize.y - 10);
	}

	itemPage.setMaxSize({ header.getSize().x, itemCount * (itemSize.y + 10) + 10 });
	itemPage.setActiveRegion({ itemPage.getActiveRegion().left, itemPage.getActiveRegion().top, itemPage.getActiveRegion().width, std::min(itemLimit, itemCount) * (itemSize.y + 10) + 10 });
	itemPage.setScroll(Scroll::ScrollPosition::RIGHT);
}

bool gui::Dropdown::contains(const sf::Vector2f& mousePos)
{
	sf::FloatRect rect;
	rect.left = getPosition().x;
	rect.top = getPosition().y;
	rect.width = getSize().x;
	rect.height = getSize().y;

	return rect.contains(mousePos);
}

bool gui::Dropdown::containsExcludingHeader(const sf::Vector2f& mousePos)
{
	sf::FloatRect rect;
	rect.left = getPosition().x;
	rect.top = getPosition().y + header.getSize().y;
	rect.width = getSize().x;
	rect.height = getSize().y;

	return rect.contains(mousePos);
}

gui::Entity* gui::Dropdown::isHit(const sf::Vector2f& mousePos)
{
	if (isActive()) {
		Entity* entity = nullptr;

		entity = collapse.isHit(mousePos);
		if (entity != nullptr) return entity;

		entity = header.isHit(mousePos);
		if (entity != nullptr) return entity;

		if (itemPage.isActive()) {
			entity = itemPage.isHit(mousePos);
			if (entity != nullptr) return entity;
		}

		if (contains(mousePos))return this;
	}

	return nullptr;
}

bool gui::Dropdown::pollEvents(sf::Event e)
{
	if (e.type == sf::Event::MouseWheelScrolled) {
		if (e.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel) {
			this->itemPage.scrollBy(Scroll::ScrollPosition::TOP, sf::Vector2f(-5 * e.mouseWheelScroll.delta, 0));
			this->itemPage.scrollBy(Scroll::ScrollPosition::BOTTOM, sf::Vector2f(-5 * e.mouseWheelScroll.delta, 0));
		}
		else {
			this->itemPage.scrollBy(Scroll::ScrollPosition::LEFT, sf::Vector2f(0, -5 * e.mouseWheelScroll.delta));
			this->itemPage.scrollBy(Scroll::ScrollPosition::RIGHT, sf::Vector2f(0, -5 * e.mouseWheelScroll.delta));
		}
		return true;
	}
	return false;
}

void gui::Dropdown::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	header.draw(target);
	collapse.draw(target);
	itemPage.draw(target);
}

void gui::Dropdown::activateSelection()
{
	isSelected = true;
}

void gui::Dropdown::deactivateSelection()
{
	isSelected = false;
}
