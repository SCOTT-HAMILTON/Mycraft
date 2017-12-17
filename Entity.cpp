#include "Entity.h"

Entity::Entity(){
  fps = 60;
  time = fps/100;
}

Entity::~Entity(){

}
void Entity::draw(sf::RenderTarget &target){
  //std::cout << "Entity draw!" << std::endl;
}

void Entity::update(){
  //std::cout << "Entity update!" << std::endl;
}

sf::Vector2f Entity::getMapPosition() const{
  return map_pos;
}
