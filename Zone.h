#ifndef ZONE_H
#define ZONE_H

#include <iostream>
#include <SFML/Graphics.hpp>

struct Block2Id{
  std::string block;
  std::string id;
};

struct Block{
  std::string block;
  sf::Vector2f map_pos;
  //DEBUG VISUAL
  bool block_different;
};

enum ZoneType{ContainerZone, BlockZone, InvalidZone};

class Zone{

public:
  Zone(const sf::IntRect &rectzone, const sf::Vector2i &min_rect, int div);
  bool containsBlocks() const;
  bool containsThisBlock(const sf::Vector2f &pos) const;

  bool addBlock(const sf::Vector2f &pos, const std::string &block);
  std::vector<Block> getAllBlocks() const;

private:
  sf::IntRect rect;
  sf::Vector2i min_rect;
  int div;
  ZoneType type;

  std::vector<Zone> children;
  std::vector<Block> blocks;
  bool contains_block;
};

#endif //ZONE_H
