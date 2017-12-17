#include "Zone.h"

Zone::Zone(const sf::IntRect &rectzone, const sf::Vector2i &min_rect, int div){
  rect = rectzone;
  this->min_rect = min_rect;
  this->div = div;
  contains_block = false;

  if (rect.height < min_rect.y || rect.width < min_rect.x || rect.width%div != 0 || rect.width%min_rect.x != 0 || rect.height%div != 0 || rect.height%min_rect.y != 0){
    std::cout << "error zone invalid" << std::endl;
    type = InvalidZone;
  }
  else if (rect.width / min_rect.x <= div){
    type = BlockZone;
    for (int i = 0; i < div*div; i++){
      Block tmpblock;
      tmpblock.block = "";
      tmpblock.map_pos = sf::Vector2f( (i%div)*min_rect.x, ((i-(i%div))/div)*min_rect.y );
      tmpblock.block_different = false;
      blocks.emplace_back(std::move(tmpblock));
    }
    std::cout << "block zone !" << std::endl;
  }else{
    type = ContainerZone;
    std::cout << "container !" << std::endl;
    sf::Vector2i echelle = {rect.width/div, rect.height/div};
    for (int y = rect.top; y < rect.top+rect.height; y+=echelle.y){
      for (int x = rect.left; x < rect.left+rect.width; x+=echelle.x){
        sf::IntRect thisrect(x, y, echelle.x, echelle.y);
        children.emplace_back(std::move( Zone(thisrect, min_rect, div) ));
      }
    }
  }
}

bool Zone::containsBlocks() const{
  if (type == BlockZone){
    return contains_block;
  }else{
    for (auto i = 0; i < children.size(); i++){
      if (children[i].containsBlocks())return true;
    }
    return false;
  }
  return false;
}

bool Zone::containsThisBlock(const sf::Vector2f &pos) const{
  if (!rect.contains(static_cast<sf::Vector2i>(pos) ))return false;
  if (type == BlockZone){
    if (!contains_block)return false;
    sf::Vector2i tmppos = { (int)pos.x-((int)pos.x%min_rect.x), (int)pos.y-((int)pos.y%min_rect.y) };
    tmppos.x-=rect.left;
    tmppos.y-=rect.top;
    int index = (tmppos.y/min_rect.y)*div+(tmppos.x/min_rect.x);
    if (index>=0 && index<blocks.size()){
      if (blocks[index].block != "")return true;
      return false;
    }
  }else if (type == ContainerZone){
    for (int i = 0; i < children.size(); i++){
      if (children[i].containsThisBlock(pos))return true;
    }
  }else{
    std::cout << "invalid : " << std::endl;
  }
  return false;
}


bool Zone::addBlock(const sf::Vector2f &pos, const std::string &block){

  if (type == BlockZone){
    sf::Vector2i tmppos = { (int)pos.x-((int)pos.x%min_rect.x), (int)pos.y-((int)pos.y%min_rect.y) };

    if (rect.contains(tmppos)){
      tmppos.x-=rect.left;
      tmppos.y-=rect.top;
      int index = (tmppos.y/min_rect.y)*div+(tmppos.x/min_rect.x);
      if (index>=0 && index<blocks.size()){
        blocks[index] = {block, pos, false};
        contains_block = true;
        return true;
      }else{
      }
    }
  }else if (type == ContainerZone){
    for (int i = 0; i < children.size(); i++){
      if (children[i].addBlock(pos, block))return true;
    }
  }else{
    std::cout << "invalid : " << std::endl;
  }
  return false;
}

std::vector<Block> Zone::getAllBlocks() const{
    std::vector<Block> result;
  if (type == BlockZone){
    for (int i = 0; i < blocks.size(); i++){
      if (blocks[i].block != "")result.push_back(blocks[i]);
    }
  }else if (type == ContainerZone){
    for (int i = 0; i < children.size(); i++){
      std::vector<Block> tmp = children[i].getAllBlocks();
      result.insert( result.end(), tmp.begin(), tmp.end() );
    }
  }
  return result;
}
