#include "Page.hpp"
#include "Scroll.hpp"

gui::Page::Page(const sf::Vector2f& size)
	:Entity(__GUI_ID_PAGE)
{
	maxSize = size;
	activeRegion = sf::FloatRect(0, 0, size.x, size.y);

	action = nullptr;
	actionEvent = ActionEvent::NONE;

	for (int i = 0; i < 4; i++) {
		connectedScroll[i] = nullptr;
	}

	fillColor = sf::Color::Transparent;
	outlineColor = sf::Color::Transparent;

	setHeader(false);
	header.setInactive();
}

gui::Page::~Page()
{
	for (int i = 0; i < 4; i++) {
		delete connectedScroll[i];
	}
}

void gui::Page::addEntity(Entity& entity)
{
	allEntities.push_back(&entity);
}

void gui::Page::removeEntity(const Entity& entity)
{
	std::string name = getName(entity.getID());
	if (name != "")Frame::removeName(name);
	for (int i = 0; i < allEntities.size(); i++) {
		if (allEntities[i]->getID() == entity.getID()) {
			allEntities.erase(allEntities.begin() + i);
			break;
		}
	}
}

void gui::Page::removeEntity(unsigned int id)
{
	for (int i = 0; i < allEntities.size(); i++) {
		if (allEntities[i]->getID() == id) {
			allEntities.erase(allEntities.begin() + i);
			break;
		}
	}
}

void gui::Page::setName(const Entity& entity, const std::string& name)
{
	Frame::setName(entity, name);
}

gui::Entity* gui::Page::getByID(unsigned int id) const
{
	for (int i = 0; i < allEntities.size(); i++) {
		if (allEntities[i]->getID() == id) {
			return allEntities[i];
		}
	}
	return nullptr;
}

gui::Entity* gui::Page::getByName(const std::string& name) const
{
	return getByID(Frame::getIDByName(name));
}

std::string gui::Page::getName(unsigned int id)
{
	return Frame::getName(id);
}

sf::Vector2f gui::Page::getMousePosition() const
{
	return getFrame()->getMousePosition() - activePos + sf::Vector2f(activeRegion.left, activeRegion.top);
}

void gui::Page::setFillColor(sf::Color color)
{
	fillColor = color;
}

void gui::Page::setOutlineColor(sf::Color color)
{
	outlineColor = color;
}

void gui::Page::setHeader(bool hasHeader, bool isMovable, bool isMinimisable)
{
	if (hasHeader) {
		header.setPointCount(4);
		header.setPoint(0, sf::Vector2f(0,0));
		header.setPoint(1, sf::Vector2f(activeRegion.width, 0));
		header.setPoint(2, sf::Vector2f(activeRegion.width, 15));
		header.setPoint(3, sf::Vector2f(0,15));
		header.setPosition(0, -15);
		header.setOriginalFillColor(sf::Color::White);
		header.setHighlightFillColor(sf::Color::White);
		header.actionEvent = ActionEvent::MOUSEHELD;
		if (isMovable)header.setAction([this] {move(getFrame()->getLastMouseOffset()); });
		else header.setAction(nullptr);
		header.setActive();

		collapse.setPointCount(4);
		collapse.setPoint(0, sf::Vector2f(0, 0));
		collapse.setPoint(1, sf::Vector2f(15, 0));
		collapse.setPoint(2, sf::Vector2f(15, 15));
		collapse.setPoint(3, sf::Vector2f(0, 15));
		collapse.setPosition(activeRegion.width - 15, -15);
		collapse.setOriginalFillColor(sf::Color::Red);
		collapse.setHighlightFillColor(sf::Color::Red);
		collapse.setHighlightOutlineColor(sf::Color::Red);
		collapse.setHighlightOutlineThickness(3);
		collapse.actionEvent = ActionEvent::RELEASE;
		if (isMinimisable) {
			collapse.setAction([this] { isActive() ? setInactive() : setActive(); });
			collapse.setActive();
		}
		else {
			collapse.setAction(nullptr);
			collapse.setInactive();
		}
	}
	else {
		header.setInactive();
		header.actionEvent = ActionEvent::NONE;
		header.setAction(nullptr);

		collapse.setInactive();
		collapse.actionEvent = ActionEvent::NONE;
		collapse.setAction(nullptr);
	}
}

void gui::Page::scrollBy(int scrollPos, const sf::Vector2f& offset)
{
	Scroll::ScrollPosition comp;
	if (scrollPos == Scroll::LEFT)comp = Scroll::RIGHT;
	else if (scrollPos == Scroll::TOP)comp = Scroll::BOTTOM;
	else if (scrollPos == Scroll::RIGHT)comp = Scroll::LEFT;
	else if (scrollPos == Scroll::BOTTOM)comp = Scroll::TOP;

	sf::Vector2f newOffset;

	if (connectedScroll[scrollPos] != nullptr)newOffset = connectedScroll[scrollPos]->scrollBy(offset);
	if (connectedScroll[comp] != nullptr)newOffset = connectedScroll[comp]->scrollBy(newOffset);

	if (connectedScroll[scrollPos] == nullptr)scrollPos = comp;

	if (connectedScroll[scrollPos] != nullptr)
		moveActiveRegion({
		newOffset.x * getMaxSize().x / connectedScroll[scrollPos]->getSize().x,
		newOffset.y * getMaxSize().y / connectedScroll[scrollPos]->getSize().y
			});
}


void gui::setScrollAction(Page* page, int scrollPos)
{
	Scroll::ScrollPosition comp;
	if (scrollPos == Scroll::LEFT)comp = Scroll::RIGHT;
	else if (scrollPos == Scroll::TOP)comp = Scroll::BOTTOM;
	else if (scrollPos == Scroll::RIGHT)comp = Scroll::LEFT;
	else if (scrollPos == Scroll::BOTTOM)comp = Scroll::TOP;

	page->connectedScroll[scrollPos]->bar.setAction([page, scrollPos, comp]() {
		sf::Vector2f offset = page->connectedScroll[scrollPos]->scrollTo(page->getFrame()->getMousePosition() - page->getPosition());
		if (page->connectedScroll[comp] != nullptr)page->connectedScroll[comp]->scrollBy(offset);
		page->moveActiveRegion({
			offset.x * page->getMaxSize().x / page->connectedScroll[scrollPos]->rect.getSize().x,
			offset.y * page->getMaxSize().y / page->connectedScroll[scrollPos]->rect.getSize().y
			});
	});
	page->connectedScroll[scrollPos]->rect.setAction([page, scrollPos, comp]() {
		sf::Vector2f offset = page->connectedScroll[scrollPos]->scrollTo(page->getFrame()->getMousePosition() - page->getPosition());
		if (page->connectedScroll[comp] != nullptr)page->connectedScroll[comp]->scrollBy(offset);
		page->moveActiveRegion({
			offset.x * page->getMaxSize().x / page->connectedScroll[scrollPos]->rect.getSize().x,
			offset.y * page->getMaxSize().y / page->connectedScroll[scrollPos]->rect.getSize().y
			});
	});
}

void gui::Page::setScroll(int scrollPos)
{
	if (connectedScroll[scrollPos] == nullptr) connectedScroll[scrollPos] = new Scroll();

	setScrollAction(this, scrollPos);

	connectedScroll[scrollPos]->setPage(*this, (Scroll::ScrollPosition)scrollPos);
}

void gui::Page::removeScroll(int scrollPos)
{
	delete connectedScroll[scrollPos];
	connectedScroll[scrollPos] = nullptr;
}

void gui::Page::limitActiveRegion()
{
	activeRegion.left = std::max(0.f, activeRegion.left);
	activeRegion.left = std::min(maxSize.x, activeRegion.left);

	activeRegion.top = std::max(0.f, activeRegion.top);
	activeRegion.top = std::min(maxSize.y, activeRegion.top);

	activeRegion.width = std::max(0.f, activeRegion.width);
	if (activeRegion.left + activeRegion.width >= maxSize.x)activeRegion.width = maxSize.x - activeRegion.left;
	activeRegion.height = std::max(0.f, activeRegion.height);
	if (activeRegion.top + activeRegion.height >= maxSize.y)activeRegion.height = maxSize.y - activeRegion.top;
}

void gui::Page::setActiveRegion(const sf::FloatRect& region)
{
	activeRegion = region;
	limitActiveRegion();
}

sf::FloatRect gui::Page::getActiveRegion() const
{
	return activeRegion;
}

void gui::Page::moveActiveRegion(float offsetX, float offsetY)
{
	activeRegion.left += offsetX;
	activeRegion.top += offsetY;
	limitActiveRegion();
}

void gui::Page::moveActiveRegion(const sf::Vector2f& offset)
{
	activeRegion.left += offset.x;
	activeRegion.top += offset.y;
	limitActiveRegion();
}

void gui::Page::setMaxSize(const sf::Vector2f& size)
{
	maxSize = size;
}

sf::Vector2f gui::Page::getMaxSize() const
{
	return maxSize;
}


sf::Vector2f gui::Page::getSize() const
{
	return { activeRegion.width, activeRegion.height };
}

void gui::Page::setPosition(float x, float y)
{
	activePos.x = x;
	activePos.y = y;
}

void gui::Page::setPosition(const sf::Vector2f& position)
{
	activePos = position;
}

sf::Vector2f gui::Page::getPosition() const
{
	return activePos;
}

void gui::Page::move(float offsetX, float offsetY)
{
	activePos.x += offsetX;
	activePos.y += offsetY;
}

void gui::Page::move(const sf::Vector2f& offset)
{
	activePos += offset;
}

bool gui::Page::contains(const sf::Vector2f& mousePos)
{
	sf::FloatRect rect = getActiveRegion();
	rect.left = activePos.x; rect.top = activePos.y;

	return rect.contains(mousePos);
}

gui::Entity* gui::Page::isHit(const sf::Vector2f& mousePos)
{
	Entity* entity = nullptr;

	if (header.isActive()) {
		entity = header.isHit(mousePos - activePos);
		if (entity != nullptr)return entity;

		entity = collapse.isHit(mousePos - activePos);
		if (entity != nullptr)return entity;
	}


	if (isActive()) {
		sf::Vector2f mPos = mousePos - activePos;
		if (mPos.x < 0 || mPos.x >= activeRegion.width || mPos.y < 0 || mPos.y >= activeRegion.height)return nullptr;
		mPos += sf::Vector2f(activeRegion.left, activeRegion.top);

		for (int i = 0; i < 4 && entity == nullptr; i++) {
			if (connectedScroll[i] != nullptr)entity = connectedScroll[i]->isHit(mousePos - activePos);
		}

		for (auto it = allEntities.begin(); it != allEntities.end() && entity == nullptr; it++) {
			entity = (*it)->isHit(mPos);
		}
		
		

		if (entity == nullptr && contains(mousePos))entity = this;

		return entity;
	}
	return nullptr;
}

void gui::Page::activateSelection()
{
	isSelected = true;
}

void gui::Page::deactivateSelection()
{
	isSelected = false;
}

bool gui::Page::pollEvents(sf::Event e)
{
	if (e.type == sf::Event::MouseWheelScrolled) {
		if (e.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel) {
			this->scrollBy(Scroll::ScrollPosition::TOP, sf::Vector2f(-5 * e.mouseWheelScroll.delta, 0));
			this->scrollBy(Scroll::ScrollPosition::BOTTOM, sf::Vector2f(-5 * e.mouseWheelScroll.delta, 0));
		}
		else {
			this->scrollBy(Scroll::ScrollPosition::LEFT, sf::Vector2f(0, -5 * e.mouseWheelScroll.delta));
			this->scrollBy(Scroll::ScrollPosition::RIGHT, sf::Vector2f(0, -5 * e.mouseWheelScroll.delta));
		}
		return true;
	}
	return false;
}

void gui::Page::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isActive()) {
		sf::Vertex v[4] = {
			{{activePos.x, activePos.y}, outlineColor},
			{{activePos.x + activeRegion.width, activePos.y}, outlineColor},
			{{activePos.x + activeRegion.width, activePos.y + activeRegion.height}, outlineColor},
			{{activePos.x, activePos.y + activeRegion.height}, outlineColor}
		};

		target.draw(v, 4, sf::LinesStrip);

		v[0].color = v[1].color = v[2].color = v[3].color = fillColor;

		target.draw(v, 4, sf::Quads);

		if (maxSize.x && maxSize.y) {
			sf::RenderTexture rt;
			rt.create(maxSize.x, maxSize.y);
			rt.clear(sf::Color::Transparent);

			for (auto it = allEntities.begin(); it != allEntities.end(); it++) {
				(*it)->draw(rt);
			}
			rt.display();
			sf::Sprite s;
			s.setTexture(rt.getTexture());
			s.setTextureRect(sf::IntRect(activeRegion));
			s.setPosition(activePos);
			target.draw(s);
		}
		
		for (int i = 0; i < 4; i++) {
			if (connectedScroll[i] == nullptr)continue;
			connectedScroll[i]->move(activePos);
			connectedScroll[i]->draw(target);
			connectedScroll[i]->move(-activePos);
		}
	}

	if (header.isActive()) {
		header.move(activePos);
		header.draw(target);
		header.move(-activePos);

		collapse.move(activePos);
		collapse.draw(target);
		collapse.move(-activePos);
	}
}
