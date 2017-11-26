#include "EntityManager.h"
#include <utility>

EntityManager::EntityManager(){

}

void EntityManager::drawEntities(sf::RenderTarget &target){
  for (auto i = 0; i < entities.size(); i++){
    entities[i]->draw(target);
  }
}

void EntityManager::updateEntities(){
  for (auto i = 0; i < entities.size(); i++){
    entities[i]->update();
  }
}


void EntityManager::addEntity(std::shared_ptr<Entity> && entity)
{
  entities.push_back(entity);
}
