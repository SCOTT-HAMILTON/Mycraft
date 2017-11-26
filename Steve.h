#ifndef STEVE_H
#define STEVE_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Entity.h"

class Steve : public Entity
{
public:
  Steve(const sf::Vector2f &pos = {0, 0});
  virtual ~Steve();
  virtual void draw(sf::RenderTarget &target);
  virtual void update();
  virtual sf::Vector2f getPosition() const;

  //Controls
  void setright(bool val);
  void setleft(bool val);
  void setjump(bool val);
  void changeArmAngle(const sf::Vector2f &mousepos);
  void sword_down();
  void sword_up();
  void kick();

protected:
  sf::Vector2f pos;

private:
  bool right;
  bool left;
  bool jump;
  bool jumping;
  float jumpy;
  float vec_jump;
  float gravity;
  float arm_length;
  bool kicking;
  bool kick_gived;

  sf::Texture text_walk0;
  sf::Texture text_walk1;
  sf::Texture text_arm;
  sf::Texture text_sword;

  int sprite;
  int angle_arm;
  int angle_sword;

  sf::Clock jumpclock;
  sf::Clock kickclock;

};

#endif //STEVE_H
