#ifndef ENTYTI_H
#define ENTYTI_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265359;
#endif //M_PI

class Entity{

public:
  Entity();
  virtual ~Entity();
  virtual void draw(sf::RenderTarget &target);
  virtual void update();
  virtual sf::Vector2f getPosition() const;

protected:
  sf::Vector2f pos;
  float time;
  float fps;
};

#endif //ENTYTI_H
