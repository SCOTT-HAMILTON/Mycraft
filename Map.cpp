#include "Map.h"
#include <fstream>
#include <cmath>

Map::Map(){
  //VISUAL DEBUGGING
  line[0] = sf::Vertex(sf::Vector2f(0, 0), sf::Color::Green);
  line[1] = sf::Vertex(sf::Vector2f(0, 0), sf::Color::Green);
  draw_rectposeblock = false;
  draw_wanted_blocktopos = false;

  std::cout << "a New MAp constructed! !!!!!" << std::endl;

  sf::Vector2i min_rect = {bloc_size, bloc_size};
  int div = 3;

  for (int i = 0; i < 70; i++){
    for (int j = 0; j < 70; j++){
      sf::Vector2i scale = {min_rect.x*div*div, min_rect.y*div*div};
      zones.emplace_back(std::move(Zone( sf::IntRect(j*scale.x, i*scale.y, scale.x, scale.y), min_rect, div )));
    }
  }

  loadBlocksIndex("res/blocks/blocks_index.txt");
  int min_x = width_window/2, max_x = width_window/2;
  player_startmappos.y = height_window/2;
  int pos_ymax = height_window;
  int pos_ymin = 0;
  if (blocks.size()==0){
    std::cout << "Blocks association file corrupted !" << std::endl;
  }else{
    int nb_couch = 3;
    for (auto y = 0; y < nb_couch; y++){
      std::vector<Block> tmp;
      for (auto x = 0; x < 22; x++){
        sf::Vector2f pos;
        pos.x = x*bloc_size;
        pos.y = height_window-(bloc_size*(nb_couch-y) );
        if(pos.x < min_x)min_x = pos.x;
        if (pos.x > max_x)max_x = pos.x;
        if (pos.y < pos_ymax){
          pos_ymax = pos.y;
        }
        if (pos.y > pos_ymin){
          pos_ymin = pos.y;
        }
        std::string key( std::to_string(pos.x)+"|"+std::to_string(pos.y) );
        map[key] = {blocks[0].block, pos, false};
        std::cout << "pos : " << pos.x << ", " << pos.y << std::endl;
        for (int i = 0; i < zones.size(); i++){
          if (zones[i].addBlock(pos, blocks[0].block)){
            break;
          }
        }

      }
    }
  }

  rect_map.height = pos_ymin-pos_ymax;
  rect_map.width = max_x-min_x;
  rect_map.left = min_x;
  rect_map.top = pos_ymax;

  player_startmappos.x = ((max_x-min_x)/2)+min_x;
  player_startmappos.y = pos_ymax;
  if (player_startmappos.y<height_window){
    /*while (!(map[(int)player_startmappos.y/bloc_size-1][player_startmappos.x/bloc_size].block != "")){
      player_startmappos.y -= bloc_size;
    }*/
  }

  player_startwindowpos.x = width_window/2;
  player_startwindowpos.y = height_window/2;
  if (pos_ymax-bloc_size>=0){
    player_startwindowpos.y = pos_ymax;
  }

  blockused.push_back(blocks[0]);
}

void Map::drawMap(const sf::FloatRect &rect, sf::RenderWindow &target){
  int start_x = ((int)rect.left-((int)rect.left%bloc_size))/bloc_size;
  int start_y = ((int)rect.top-((int)rect.top%bloc_size))/bloc_size;
  if (start_x<0)start_x = 0;
  if (start_y<0)start_y = 0;
  for (std::vector<Zone>::iterator itzones = zones.begin(); itzones != zones.end(); itzones++){
    if (itzones->containsBlocks()){
      std::vector<Block> blocks = itzones->getAllBlocks();

      for (std::vector<Block>::iterator it = blocks.begin(); it != blocks.end(); it++){
        sf::FloatRect rectblock;
        rectblock.left = it->map_pos.x;
        rectblock.top = it->map_pos.y;
        rectblock.width = bloc_size;
        rectblock.height = bloc_size;
        if (rect.intersects(rectblock) && it->block != ""){
          sf::Vector2f pos = {rectblock.left-rect.left, rectblock.top-rect.top};
          sf::Sprite sp;
          sf::Texture textsp;

          textsp.loadFromFile("res/blocks/"+it->block);
          sp.setTexture(textsp);
          sp.setPosition(pos);
          if (it->block_different){
            sp.setColor(sf::Color(255, 255, 255, 100));
          }
          target.draw(sp);
        }
      }

    }
  }

  if (draw_rectposeblock){
    sf::RectangleShape block(sf::Vector2f(rectBlocks.width, rectBlocks.height));
    block.setFillColor(sf::Color::Yellow);
    block.setPosition(rectBlocks.left-rect.left, rectBlocks.top-rect.top);
    target.draw(block);

    line[0].position = sf::Vector2f(eyes_pos.x-rect.left, eyes_pos.y-rect.top);

    line[1].position = sf::Vector2f(mousepos.x-rect.left, mousepos.y-rect.top);

    target.draw(line, 2, sf::Lines);
  }
  if (draw_wanted_blocktopos){
    sf::RectangleShape block(sf::Vector2f(bloc_size, bloc_size));
    block.setFillColor(sf::Color::Blue);
    block.setPosition(wanted_blocktopos.x-rect.left, wanted_blocktopos.y-rect.top);
    target.draw(block);
  }
}

sf::Vector2f Map::getMapStartPos() const{
  return player_startmappos;
}

sf::Vector2f Map::getWindowStartPos() const{
  return player_startwindowpos;
}


int Map::loadBlocksIndex(const std::string &filepath){
  std::ifstream file(filepath);
  if (file.good()){
    char buff[1024];

    while (!file.eof()){
      file.getline(buff, 1000);
      std::string tmp(buff);
      if (tmp == ""){
        break;
      }
      std::vector<std::string> tab = split(tmp, ':');
      if (tab.size()!=2){
        break;
      }
      blocks.push_back({tab[0], tab[1]});
    }
    return 0;
  }else{
    std::cout << "Error Block ID association file doesn't exist." << std::endl;
    return 1;
  }
}

bool Map::block_exist(const sf::Vector2f &block_pos) const{
  sf::Vector2f tmppos;
  tmppos.x = (int)block_pos.x-((int)block_pos.x%bloc_size);
  tmppos.y = (int)block_pos.y-((int)block_pos.y%bloc_size);

  for (int i = 0; i < zones.size(); i++){
    if (zones[i].containsThisBlock(tmppos))return true;
  }
  return false;
}

int Map::detPandD(const sf::Vector2f &point, const sf::Vector2f &d1, const sf::Vector2f &d2){
  sf::Vector2f D, T;
  D.x = d2.x-d1.x;
  D.y = d2.y-d1.y;
  T.x = point.x-d1.x;
  T.y = point.y-d1.y;
  float d = (D.x*T.y)-(D.y*T.x);
  if (d<0){
    return -1;
  }else if (d>0){
    return 1;
  }
  return 0;
}

bool Map::linesCollide(const sf::Vector2f &l1, const sf::Vector2f &l2, const sf::Vector2f &d1, const sf::Vector2f &d2){
  if (detPandD(l1, d1, d2)+detPandD(l2, d1, d2) == 0){
    if (detPandD(d1, l1, l2)+detPandD(d2, l1, l2) == 0){
      return true;
    }
  }

  return false;
}

void Map::addBlockToMap(const sf::Vector2f &pos, const std::string &block){
  if(!block_exist(pos)){
    sf::Vector2f tmppos;
    tmppos.x = (int)pos.x-((int)pos.x%bloc_size);
    tmppos.y = (int)pos.y-((int)pos.y%bloc_size);

    for (int i = 0; i < zones.size(); i++){
      if (zones[i].addBlock(pos, blocks[0].block)){
        break;
      }
    }
  }else{
    std::cout << "block exists !!! cannot add it" << std::endl;
  }
}

float Map::distance(const sf::Vector2f &p1, const sf::Vector2f &p2){
  return std::sqrt( std::pow( p2.x-p1.x , 2)+std::pow( p2.y-p1.y , 2) );
}

void Map::poseBlock(const sf::Vector2f &eyes_pos, const sf::Vector2f &mousepos, const float &dist_poseblock) {
  sf::Vector2f max_pos;
  float dist = distance(eyes_pos, mousepos);
  max_pos = eyes_pos;

  max_pos.x += (mousepos.x-eyes_pos.x)/dist*dist_poseblock;
  max_pos.y += (mousepos.y-eyes_pos.y)/dist*dist_poseblock;

  this->eyes_pos = eyes_pos;
  this->mousepos = mousepos;

  CollideData collision = getNearestPosCollide(eyes_pos, max_pos);

  sf::Vector2f pos_block = collision.pos;

  pos_block.x = (int)pos_block.x-((int)pos_block.x%bloc_size);
  pos_block.y = (int)pos_block.y-((int)pos_block.y%bloc_size);


  if (collision.side == RIGHT){
    addBlockToMap(sf::Vector2f(pos_block.x+bloc_size, pos_block.y), blocks[0].block);
  }else if (collision.side == LEFT){
    addBlockToMap(sf::Vector2f(pos_block.x-bloc_size, pos_block.y), blocks[0].block);
  }else if (collision.side == TOP){
    addBlockToMap(sf::Vector2f(pos_block.x, pos_block.y-bloc_size), blocks[0].block);
  }else if (collision.side == DOWN){
    addBlockToMap(sf::Vector2f(pos_block.x, pos_block.y+bloc_size), blocks[0].block);
  }
}

bool Map::intersection(const sf::Vector2f &p0, const sf::Vector2f &p1, const sf::Vector2f &p2, const sf::Vector2f &p3, sf::Vector2f *pos)
{
    float s1_x, s1_y, s2_x, s2_y;
    s1_x = p1.x - p0.x;     s1_y = p1.y - p0.y;
    s2_x = p3.x - p2.x;     s2_y = p3.y - p2.y;

    float s, t;
    s = (-s1_y * (p0.x - p2.x) + s1_x * (p0.y - p2.y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (p0.y - p2.y) - s2_y * (p0.x - p2.x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        // Collision detected
        if (pos != nullptr){
          pos->x = p0.x + (t * s1_x);
          pos->y = p0.y + (t * s1_y);
        }
        return true;
    }

    return false; // No collision
}

float Map::Points2Angle(const sf::Vector2f &p1, const sf::Vector2f &p2){
  float angle_start = 0;
  float coeff = 1;
  float o = (float)std::abs(p2.x-p1.x);
  float a = (float)std::abs(p2.y-p1.y);
  if (p2.x>p1.x && p2.y>p1.y){  // DOWN-RIGHT
    angle_start = 180;
    coeff = -1;
  }else if(p2.x<p1.x && p2.y < p1.y){ // UP-LEFT
    coeff = -1;
  }else if(p2.x<p1.x && p2.y > p1.y){ // DOWN-RIGHT
    angle_start = 180;
  }
  return (float)((std::atan (o/a)*180/M_PI)*coeff)+angle_start;
}

bool Map::blockInThisArea(const sf::FloatRect &rect) const{
  sf::IntRect tmprect(rect);
  tmprect.left = (int)rect.left-((int)rect.left%bloc_size);
  tmprect.top = (int)rect.top-((int)rect.top%bloc_size);

  tmprect.width += rect.left-tmprect.left;
  tmprect.height += rect.top-tmprect.top;

  for (int i = tmprect.top; i < tmprect.height; i+=bloc_size){
    for (int j = tmprect.left; j < tmprect.width; j+=bloc_size){
      if (block_exist(sf::Vector2f(j, i)))return true;
    }
  }
  return false;
}

CollideData Map::getNearestPosCollide(const sf::Vector2f &l1, const sf::Vector2f &l2)const {
  CollideData result;
  sf::Vector2f pos(-1, -1);
  result.side = RIEN;
  if (l1 == l2)return result;

  sf::FloatRect area(std::min(l1.x, l2.x), std::min(l1.y, l2.y), std::max(l1.x, l2.x), std::max(l1.y, l2.y));

  sf::IntRect tmprect(area);
  tmprect.left = (int)area.left-((int)area.left%bloc_size);
  tmprect.top = (int)area.top-((int)area.top%bloc_size);

  tmprect.width += area.left-tmprect.left;
  tmprect.height += area.top-tmprect.top;

  float best_dist = -1;

  for (int i = tmprect.top; i <= tmprect.height; i+=bloc_size){
    for (int j = tmprect.left; j <= tmprect.width; j+=bloc_size){
      sf::Vector2f block_mappos(j, i);
      if (block_exist(block_mappos)){
      //  std::cout << "block exist !!" << std::endl;
        float curbestdist = -1;
        BLOCK_SIDE curside;
        sf::Vector2f curbestdist_pos;
        sf::Vector2f intersect;
        if (intersection(l1, l2, block_mappos, sf::Vector2f(block_mappos.x, block_mappos.y+bloc_size), &intersect)){ //LEFT
          curbestdist = distance(l1, intersect);
          curbestdist_pos = intersect;
          curside = LEFT;
        }
        if (intersection(l1, l2, sf::Vector2f(block_mappos.x+bloc_size, block_mappos.y), sf::Vector2f(block_mappos.x+bloc_size, block_mappos.y+bloc_size), &intersect)) { //RIGHT
          float tmpdist = distance(l1, intersect);
          if (curbestdist == -1 || tmpdist<curbestdist){
            curbestdist = tmpdist;
            curbestdist_pos = intersect;
            curside = RIGHT;
          }
        }
        if (intersection(l1, l2, block_mappos, sf::Vector2f(block_mappos.x+bloc_size, block_mappos.y), &intersect)) { //TOP
          float tmpdist = distance(l1, intersect);
          if (curbestdist == -1 || tmpdist<curbestdist){
            curbestdist = tmpdist;
            curbestdist_pos = intersect;
            curside = TOP;
          }
        }
        if (intersection(l1, l2, sf::Vector2f(block_mappos.x, block_mappos.y+bloc_size), sf::Vector2f(block_mappos.x+bloc_size, block_mappos.y+bloc_size), &intersect)) { //DOWN
          float tmpdist = distance(l1, intersect);
          if (curbestdist == -1 || tmpdist<curbestdist){
            curbestdist = tmpdist;
            curbestdist_pos = intersect;
            curside = DOWN;
          }
        }

        if (curbestdist != -1 && (curbestdist<best_dist || best_dist == -1) ){
          best_dist = curbestdist;
          pos = curbestdist_pos;
          result.side = curside;
        }

      }
    }
  }

  if (result.side != RIEN && ( !std::isnan(pos.x) && !std::isnan(pos.y))) {
    if (result.side == RIGHT)std::cout << "RIGHT" << std::endl;
    else if (result.side == LEFT)std::cout << "LEFT" << std::endl;
    else if (result.side == TOP)std::cout << "TOP" << std::endl;
    else if (result.side == DOWN)std::cout << "DOWN" << std::endl;

    std::cout << "pos find : " << pos.x << ", " << pos.y << std::endl;
  }else{
    result.side = RIEN;
    pos = {-1, -1};
  }

  result.pos = pos;
  return result;

}
