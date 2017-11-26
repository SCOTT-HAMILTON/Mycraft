#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include <vector>
#include <memory>

class EntityManager{
public:
  EntityManager();
  void drawEntities(sf::RenderTarget &target);
  void updateEntities();
  void addEntity(std::shared_ptr<Entity> && entity);

private:
  std::vector<std::shared_ptr<Entity>> entities;
};

#endif //ENTITY_MANAGER_H
