#ifndef ENTYTI_H
#define ENTYTI_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265359
#endif //M_PI

const int bloc_size = 30;

const int width_window = 640;
const int height_window = 540;

class Entity{

public:
  Entity();
  virtual ~Entity();
  virtual void draw(sf::RenderTarget &target);
  virtual void update();
  virtual sf::Vector2f getMapPosition() const;

protected:
  sf::Vector2f map_pos;
  float time;
  float fps;
};

#endif //ENTYTI_H
