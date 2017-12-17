#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "Steve.h"
#include "EntityManager.h"
#include "Map.h"
#include <string>

float CubOutEasing(float t, float b, float c, float d);
float CubInEasing(float t, float b, float c, float d);

void displayPos(std::shared_ptr<Entity> &entity){
  std::cout << "pos : " << entity->getMapPosition().x << ", " << entity->getMapPosition().x << std::endl;
}

int main (int argc, char *argv[]){


  sf::RenderWindow fenetre(sf::VideoMode(width_window, height_window-(bloc_size*3)), "Mycraft 1.0");
  fenetre.setFramerateLimit(140);
  sf::Event event;

  std::shared_ptr<Map> map = std::make_shared<Map>(Map());

  std::shared_ptr<Steve> steve = std::make_shared<Steve>(Steve(map, sf::Vector2f(width_window/2, height_window/2) ));
  steve->changeArmAngle(static_cast<sf::Vector2f>(sf::Mouse::getPosition(fenetre)));

  EntityManager manager;
  manager.addEntity(steve);


  steve->setMapPos(sf::Vector2f(map->getMapStartPos().x, map->getMapStartPos().y-bloc_size));
  steve->setWindowPos(sf::Vector2f(map->getWindowStartPos().x, map->getWindowStartPos().y-(bloc_size*2)));
  std::cout << "getWindowStartPos.y : " << map->getWindowStartPos().y << std::endl;

  int fps = 0;
  int last_time = 0;
  sf::Clock clock_fps;

  sf::Font font;
  font.loadFromFile("res/fonts/BebasNeue.otf");

  sf::Text text_fps;
  text_fps.setFont(font);
  text_fps.setPosition(10, 10);
  text_fps.setCharacterSize(20);
  text_fps.setString("fps : "+std::to_string(fps));

  sf::FloatRect rect;
  sf::FloatRect rect_correctif;
  rect_correctif.width = 0;
  rect_correctif.height = 0;
  rect_correctif.left = 0;
  rect_correctif.top = 0;
  rect.width = fenetre.getSize().x;
  rect.height = fenetre.getSize().y;
  rect.left = steve->getMapPosition().x-(width_window/2);
  rect.top = steve->getMapPosition().y-(height_window/2);
  if (rect.top+rect.height>height_window){
    steve->setWindowPos(sf::Vector2f(steve->getWindowPosition().x, steve->getWindowPosition().y-(rect.top+rect.height-height_window)));
    rect_correctif.top -= rect.top+rect.height-height_window;
  }

  steve->draw(fenetre);

  //TEST
  sf::Vector2f test;
  if (Map::intersection(sf::Vector2f(2, 2), sf::Vector2f(2, 100), sf::Vector2f(2, 2), sf::Vector2f(100, 2), &test)){
    std::cout << "intersection : " << test.x << ", " << test.y << std::endl;
  }


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
        if (event.mouseWheelScroll.delta > 0){
          steve->sword_up();
        }else{
          steve->sword_down();
        }
      }else if(event.type == sf::Event::MouseButtonPressed){
        if (event.mouseButton.button == sf::Mouse::Left){
          steve->kick();
        }
        else if (event.mouseButton.button == sf::Mouse::Right){
          steve->pose_block(static_cast<sf::Vector2f>(sf::Mouse::getPosition(fenetre)));
        }
      }
    }

    manager.updateEntities();
    steve->changeArmAngle(static_cast<sf::Vector2f>(sf::Mouse::getPosition(fenetre)));

    fenetre.clear();
    rect.left = steve->getMapPosition().x-(width_window/2);
    rect.top = steve->getMapPosition().y-(height_window/2);
    rect.left += rect_correctif.left;
    rect.top += rect_correctif.top;
    rect.width += rect_correctif.width;
    rect.height += rect_correctif.height;
    map->drawMap(rect, fenetre);

    manager.drawEntities(fenetre);
    fps = (clock_fps.getElapsedTime().asMilliseconds()-last_time)+1;
    fps = (float)1000/fps;

    text_fps.setString("fps : "+std::to_string(fps));

    clock_fps.restart();

    fenetre.draw(text_fps);


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
