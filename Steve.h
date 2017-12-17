#ifndef STEVE_H
#define STEVE_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "Map.h"
#include <memory>

struct SorterCollision{
  float dist;
  CollideData collision;
};

class Steve : public Entity
{
public:
  Steve(const std::shared_ptr<Map> &map, const sf::Vector2f &window_pos = {0, 0});
  virtual ~Steve();
  virtual void draw(sf::RenderTarget &target);
  virtual void update();
  virtual sf::Vector2f getMapPosition() const;
  virtual sf::Vector2f getWindowPosition() const;
  bool isJumping();
  bool blockUnderFeets();

  //Controls
  void setright(bool val);
  void setleft(bool val);
  void setjump(bool val);
  void changeArmAngle(const sf::Vector2f &mousepos);
  void sword_down();
  void sword_up();
  void kick();
  void setMapPos(const sf::Vector2f &pos);
  void setWindowPos(const sf::Vector2f &pos);
  void pose_block(const sf::Vector2f &mousepos);
  sf::Vector2f pos_bloclooking();
  void checkCollision(const sf::Vector2f &prec_pos, const sf::Vector2f &wanted_pos);

protected:
  sf::Vector2f pos;

private:
  sf::Vector2f window_pos;
  bool right;
  bool left;
  bool jump;
  bool jumping;
  float jumpy;
  float vec_jump;
  float vec_fall;
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

  std::shared_ptr<Map> map;

  sf::FloatRect sprite_rect;

  float block_posdist;

};

#endif //STEVE_H
