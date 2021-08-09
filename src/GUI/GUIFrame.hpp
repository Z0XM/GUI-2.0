#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <functional>
#include <vector>

constexpr int __GUI_ID_TEXTBOX =    1;
constexpr int __GUI_ID_BUTTON =     2;
constexpr int __GUI_ID_TEXTBUTTON = 3;
constexpr int __GUI_ID_SLIDER =     4;
constexpr int __GUI_ID_SCROLL =     5;
constexpr int __GUI_ID_DROPDOWN =   6;
constexpr int __GUI_ID_PAGE =       7;

namespace gui {
	class Frame;
	class Entity {
	private:
		unsigned int id;
		Frame* currFrame;

		bool active;
		static unsigned int item_count;
	protected:
		bool isSelected;
		std::function<void()> action;
		std::vector<Entity*> varRegister;

		void registerVar(Entity* entity);
	public:
		Entity(unsigned int class_id);
		virtual ~Entity();

		unsigned int getID() const;

		Frame* getFrame() const;

		virtual Entity* isHit(const sf::Vector2f& mousePos) = 0;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const = 0;

		virtual void activateSelection() = 0;
		virtual void deactivateSelection() = 0;

		void setActive();
		void setInactive();
		bool isActive() const;

		void callAction() const;
		bool hasAction() const;
		void setAction(std::function<void()> func);

		enum class ActionEvent{NONE, RELEASE, PRESS, MOUSEHELD } actionEvent;

		friend void setEntityFrame(Entity& entity, Frame* frame);
	};

	class Frame {
	private:
		std::map<unsigned int, Entity*> entityMap;
		static std::map<std::string, unsigned int> nameMap;

		sf::RenderWindow* window;

		Entity* mouseHoveringOn, *clicked;
		sf::Vector2f lastMousePos;
	public:
		Frame();
		~Frame();

		void setWindow(sf::RenderWindow& window);

		void addEntity(Entity& entity);
		void removeEntity(Entity& entity);
		void removeEntity(unsigned int id);
		static void setName(const Entity& entity, const std::string& name);
		static void removeName(const std::string& name);
		
		Entity* getByID(unsigned int id) const;
		Entity* getByName(const std::string& name) const;
		static unsigned int getIDByName(const std::string& name);
		static std::string getName(unsigned int id);

		sf::Vector2f getLastMouseOffset() const;

		void update();
		bool pollEvents(sf::Event event);
		void draw();

		sf::Vector2f getMousePosition() const;
	};
}