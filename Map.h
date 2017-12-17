#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <string>
#include "Entity.h"
#include <memory>
#include <map>
#include <string>
#include "Zone.h"



enum BLOCK_SIDE{LEFT, RIGHT, TOP, DOWN, RIEN};

struct CollideData{
  sf::Vector2f pos;
  BLOCK_SIDE side;
};

template<typename T>
std::vector<T> split(const T &s, char delim) {
    std::vector<T> elems;
    T tmp("");
    for (int i = 0; i < static_cast<int>(s.size()); i++){
      if (s.at(i) == delim){
        elems.push_back(tmp);
        tmp = "";
      }else{
        tmp += s[i];
      }
    }
    if (tmp != ""){
      elems.push_back(tmp);
    }
    return elems;
}

class Map{
public:
  Map();
  void drawMap(const sf::FloatRect &rect, sf::RenderWindow &target);
  sf::Vector2f getMapStartPos() const;
  sf::Vector2f getWindowStartPos() const;
  bool block_exist(const sf::Vector2f &block_pos) const;
  static int detPandD(const sf::Vector2f &point, const sf::Vector2f &d1, const sf::Vector2f &d2);
  static bool linesCollide(const sf::Vector2f &l1, const sf::Vector2f &l2, const sf::Vector2f &d1, const sf::Vector2f &d2);
  void poseBlock(const sf::Vector2f &eyes_pos, const sf::Vector2f &mousepos, const float &dist_poseblock);
  static float Points2Angle(const sf::Vector2f &p1, const sf::Vector2f &p2);
  void addBlockToMap(const sf::Vector2f &pos, const std::string &block);

  static float distance(const sf::Vector2f &p1, const sf::Vector2f &p2);
  static bool intersection(const sf::Vector2f &p0, const sf::Vector2f &p1, const sf::Vector2f &p2, const sf::Vector2f &p3, sf::Vector2f *pos);
  bool blockInThisArea(const sf::FloatRect &rect) const;

  CollideData getNearestPosCollide(const sf::Vector2f &l1, const sf::Vector2f &l2) const;
private:
  sf::FloatRect rect_map;
  sf::Vector2f player_startmappos;
  sf::Vector2f player_startwindowpos;
  std::vector<Block2Id> blocks;
  int loadBlocksIndex(const std::string &filepath);
  std::map<std::string, Block> map;
  std::vector<Zone> zones;
  std::vector<Block2Id> blockused;

  //VISUAL DEBUG
  bool draw_rectposeblock;
  sf::FloatRect rectBlocks;
  sf::Vertex line[2];
  sf::Vector2f eyes_pos;
  sf::Vector2f mousepos;
  bool draw_wanted_blocktopos;
  sf::Vector2f wanted_blocktopos;
};

#endif //MAP_H
