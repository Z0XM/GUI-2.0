#include "GUIFrame.hpp"
#include "Page.hpp"
#include "Scroll.hpp"
#include "Slider.hpp"
#include "Textbox.hpp"
#include "TextButton.hpp"
#include "Dropdown.hpp"

namespace gui {
	unsigned int Entity::item_count = 0;

	void Entity::registerVar(Entity* entity)
	{
		varRegister.push_back(entity);
	}

	Entity::Entity(unsigned int class_id)
	{
		id = (class_id << 24) + ++item_count;
		
		isSelected = false;
		action = nullptr;
		actionEvent = ActionEvent::PRESS;

		setActive();
	}
	Entity::~Entity()
	{
		
	}

	unsigned int Entity::getID() const
	{
		return id;
	}

	unsigned int getClassID(Entity& entity)
	{
		return entity.getID() >> 24;
	}

	Frame* Entity::getFrame() const
	{
		return currFrame;
	}

	void Entity::setActive()
	{
		active = true;
	}
	void Entity::setInactive()
	{
		active = false;
	}
	bool Entity::isActive() const
	{
		return active;
	}

	void Entity::callAction() const
	{
		action();
	}

	bool Entity::hasAction() const
	{
		return action != nullptr;
	}

	void Entity::setAction(std::function<void()> func)
	{
		action = func;
	}

	std::map<std::string, unsigned int> Frame::nameMap;

	Frame::Frame()
	{
		window = nullptr;
		mouseHoveringOn = nullptr;
		clicked = nullptr;
	}

	Frame::~Frame()
	{
		
	}

	void Frame::setWindow(sf::RenderWindow& window)
	{
		this->window = &window;
	}

	void setEntityFrame(Entity& entity, Frame* frame)
	{
		entity.currFrame = frame;
		for (int i = 0; i < entity.varRegister.size(); i++)
			entity.varRegister[i]->currFrame = frame;
	}

	void Frame::addEntity(Entity& entity)
	{
		entityMap.insert(std::make_pair(entity.getID(), &entity));
		setEntityFrame(entity, this);
	}

	void Frame::removeEntity(Entity& entity)
	{	
		setEntityFrame(entity, nullptr);
		std::string name = getName(entity.getID());
		if(name != "")removeName(name);
		entityMap.erase(entity.getID());
	}

	void Frame::removeEntity(unsigned int id)
	{
		entityMap.erase(id);
	}

	void Frame::setName(const Entity& entity, const std::string& name)
	{
		nameMap[name] = entity.getID();
	}

	void Frame::removeName(const std::string& name)
	{
		nameMap.erase(name);
	}

	Entity* Frame::getByID(unsigned int id) const
	{
		return entityMap.at(id);
	}

	Entity* Frame::getByName(const std::string& name) const
	{
		return entityMap.at(nameMap.at(name));
	}
	unsigned int Frame::getIDByName(const std::string& name)
	{
		return nameMap.at(name);
	}
	std::string Frame::getName(unsigned int id)
	{
		for (auto it = nameMap.begin(); it != nameMap.end(); it++) {
			if (it->second == id)return it->first;
		}
		return "";
	}
	sf::Vector2f Frame::getLastMouseOffset() const
	{
		return getMousePosition() - lastMousePos;
	}
	void Frame::update()
	{
		if (clicked != nullptr && clicked->actionEvent == Entity::ActionEvent::MOUSEHELD && clicked->hasAction()) {
			clicked->callAction();
		}
		else {
			Entity* currentMouseHoveringOn = nullptr;
			for (auto it = entityMap.begin(); it != entityMap.end() && currentMouseHoveringOn == nullptr; it++) {
				currentMouseHoveringOn = it->second->isHit(getMousePosition());
			}

			if (mouseHoveringOn != currentMouseHoveringOn)
			{
				if (mouseHoveringOn != nullptr)mouseHoveringOn->deactivateSelection();
				mouseHoveringOn = currentMouseHoveringOn;
				if (mouseHoveringOn != nullptr)mouseHoveringOn->activateSelection();
			}
		}

		lastMousePos = getMousePosition();
	}
	bool Frame::pollEvents(sf::Event e)
	{
		if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
			clicked = mouseHoveringOn;

			if (clicked != nullptr && clicked->actionEvent == Entity::ActionEvent::PRESS && clicked->hasAction())
				clicked->callAction();

			return true;
		}
		else if(e.type == sf::Event::MouseButtonReleased) {
			if (clicked != nullptr && clicked == mouseHoveringOn && e.mouseButton.button == sf::Mouse::Left)
			{
				if (clicked->actionEvent == Entity::ActionEvent::RELEASE && clicked->hasAction())
					clicked->callAction();
			}
			clicked = nullptr;

			return true;
		}
		else if (e.type == sf::Event::MouseWheelScrolled) {
			bool wasEventPolled = false;
			for (auto it = entityMap.begin(); it != entityMap.end() && !wasEventPolled; it++) {
				if (getClassID(*it->second) == __GUI_ID_PAGE && ((Page*)it->second)->contains(getMousePosition()))
					wasEventPolled = ((Page*)it->second)->pollEvents(e);
				else if (getClassID(*it->second) == __GUI_ID_DROPDOWN && ((Dropdown*)it->second)->containsExcludingHeader(getMousePosition()))
					wasEventPolled = ((Dropdown*)it->second)->pollEvents(e);
			}
		}

		return false;
	}
	void Frame::draw()
	{
		for (auto it = entityMap.begin(); it != entityMap.end(); it++) {
			it->second->draw(*window);
		}
	}
	sf::Vector2f Frame::getMousePosition() const
	{
		return window->mapPixelToCoords(sf::Mouse::getPosition(*window));
	}
}
