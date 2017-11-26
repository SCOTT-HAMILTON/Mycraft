#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "Steve.h"
#include "EntityManager.h"

float CubOutEasing(float t, float b, float c, float d);
float CubInEasing(float t, float b, float c, float d);

void displayPos(std::shared_ptr<Entity> &entity){
  std::cout << "pos : " << entity->getPosition().x << ", " << entity->getPosition().x << std::endl;
}

int main (int argc, char *argv[]){
  sf::RenderWindow fenetre(sf::VideoMode(640, 540), "Mycraft 1.0");
  fenetre.setFramerateLimit(60);
  sf::Event event;

  std::shared_ptr<Steve> steve = std::make_shared<Steve>(Steve(sf::Vector2f(300, 500)));
  steve->changeArmAngle(static_cast<sf::Vector2f>(sf::Mouse::getPosition(fenetre)));

  EntityManager manager;
  manager.addEntity(steve);

  while (fenetre.isOpen()){
    while (fenetre.pollEvent(event)){
      if (event.type == sf::Event::Closed)fenetre.close();
      else if (event.type == sf::Event::KeyPressed){
        if (event.key.code == sf::Keyboard::Q)steve->setleft(true);
        else if (event.key.code == sf::Keyboard::D)steve->setright(true);
        else if (event.key.code == sf::Keyboard::Space)steve->setjump(true);
      }
      else if (event.type == sf::Event::KeyReleased){
        if (event.key.code == sf::Keyboard::Q)steve->setleft(false);
        else if (event.key.code == sf::Keyboard::D)steve->setright(false);
        else if (event.key.code == sf::Keyboard::Space)steve->setjump(false);
      }
      else if(event.type == sf::Event::MouseWheelScrolled){
        std::cout << "wheel : " << event.mouseWheelScroll.delta << std::endl;
        if (event.mouseWheelScroll.delta > 0){
          steve->sword_up();
        }else{
          steve->sword_down();
        }
      }else if(event.type == sf::Event::MouseButtonPressed){
        if (event.mouseButton.button == sf::Mouse::Left){
          steve->kick();
        }
      }
    }


    fenetre.clear();

    manager.updateEntities();
    steve->changeArmAngle(static_cast<sf::Vector2f>(sf::Mouse::getPosition(fenetre)));
    manager.drawEntities(fenetre);

    fenetre.display();
  }

  return 0;
}

float CubOutEasing(float t, float b, float c, float d) {
	t /= d;
	t--;
	return c*(t*t*t + 1) + b;
};


float CubInEasing(float t, float b, float c, float d) {
	t /= d;
	return c*t*t*t + b;
};
