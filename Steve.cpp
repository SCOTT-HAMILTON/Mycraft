#include "Steve.h"
#include <algorithm>

Steve::Steve(const std::shared_ptr<Map> &map, const sf::Vector2f &window_pos) : Entity()
{
  right = false;
  left = false;
  jump = false;
  jumping = false;
  jumpy = 13.0f;
  vec_jump = -jumpy;
  gravity = 1.0f;
  kicking = false;
  kick_gived = false;
  block_posdist = bloc_size*4;

  this->window_pos = window_pos;
  this->map = map;
  std::cout << "steve the pos : " << this->window_pos.x << ", " << this->window_pos.y << std::endl;
  angle_arm = 50;
  angle_sword = 0;

  text_walk0.loadFromFile("res/walk/walk_0.png");
  text_walk1.loadFromFile("res/walk/walk_1.png");
  text_arm.loadFromFile("res/arm.png");
  text_sword.loadFromFile("res/sword.png");

  arm_length = text_arm.getSize().y;
  std::cout << "arm_length " << arm_length << std::endl;

  sprite = 0;
}

Steve::~Steve(){

}

void Steve::update(){

  sf::Vector2f prec_pos = map_pos;

  bool blockunderfeets = blockUnderFeets();

  sf::Vector2f vec;
  if (kicking){
    if (kickclock.getElapsedTime().asMilliseconds()>50 && !kick_gived){
      kick_gived = true;
      angle_arm += 50;
    }
    else if (kickclock.getElapsedTime().asMilliseconds()>100){
      angle_arm -= 50;
      kicking = false;
      kick_gived = false;
    }
  }

  if (right){
    vec.x+=10*time;
  }else if (left){
    vec.x+=-10*time;
  }

  if (jump || jumping){
    if (!jumping && blockunderfeets){
      jumping = true;
      vec_jump = -jumpy;
      jumpclock.restart();
      while (jumpclock.getElapsedTime().asMilliseconds()<1);
    }
    else if (jumpclock.getElapsedTime().asMilliseconds()>1000/60){
      float delay = jumpclock.getElapsedTime().asMilliseconds();
      vec_jump += gravity;
      if (vec_jump >= jumpy){
        jumping = false;
      }else{
        vec.y += vec_jump;
      }
      jumpclock.restart();
    }
  }

  if (vec.y >= 0 && !jumping){
    if (!blockunderfeets){
      vec_fall+=gravity*time;
    }else{
      vec_fall = 0;
      vec.y = 0;
    }
  }else{
    vec_fall = 0;
  }



  map_pos.x += vec.x;
  map_pos.y += vec.y+vec_fall;


  //COLLISIONS
   /*

  CollideData tmpcollision1 = map->getNearestPosCollide(sf::Vector2f(prec_pos.x, prec_pos.y-(bloc_size*0.5)), sf::Vector2f(map_pos.x, map_pos.y-(bloc_size*0.5)));
  CollideData tmpcollision2 = map->getNearestPosCollide(sf::Vector2f(prec_pos.x, prec_pos.y+(bloc_size*0.5)), sf::Vector2f(map_pos.x, map_pos.y+(bloc_size*0.5)));
  CollideData collision;

  sf::Vector2f collision_startchoose, dec_startchoose;

  if (tmpcollision1.side == RIEN && tmpcollision2.side != RIEN){
    collision = tmpcollision2;
    collision_startchoose = sf::Vector2f(prec_pos.x, prec_pos.y+(bloc_size*0.5));
    std::cout << "FEET collision" << std::endl;
  }else if (tmpcollision2.side == RIEN && tmpcollision1.side != RIEN){
    collision = tmpcollision1;
    collision_startchoose = sf::Vector2f(prec_pos.x, prec_pos.y-(bloc_size*0.5));
    std::cout << "HEAD collision" << std::endl;
  }else if(tmpcollision1.side != RIEN && tmpcollision2.side != RIEN){
    if ( Map::distance(sf::Vector2f(prec_pos.x, prec_pos.y+(bloc_size*0.5)), tmpcollision1.pos) <= Map::distance(sf::Vector2f(prec_pos.x, prec_pos.y+(bloc_size*0.5)), tmpcollision2.pos) ) {
      collision = tmpcollision1;
      collision_startchoose = sf::Vector2f(prec_pos.x, prec_pos.y-(bloc_size*0.5));
      std::cout << "HEAD collision" << std::endl;
    }else{
      collision = tmpcollision2;
      collision_startchoose = sf::Vector2f(prec_pos.x, prec_pos.y+(bloc_size*0.5));
      std::cout << "FEET collision" << std::endl;
    }
  }else{
    collision = { sf::Vector2f(0, 0), RIEN};
  }


  if (collision.side != RIEN){
    dec_startchoose = {collision_startchoose.x-prec_pos.x, collision_startchoose.y-prec_pos.y};
    std::cout << "map pos : " << map_pos.x << ", " << map_pos.y << std::endl;

    std::cout << "collision : " << collision.pos.x << ", " << collision.pos.y << std::endl;
    map_pos = collision.pos;
    sf::Vector2f pos_perso = {map_pos.x-(sprite_rect.width/2), map_pos.y-(sprite_rect.height/2)};

    int best_dist = -1;
    sf::Vector2f vec_collision;

    sf::Vector2f intersect1, intersect2, intersect3, intersect4;
    if (Map::intersection(collision_startchoose, map_pos, pos_perso, sf::Vector2f(pos_perso.x, pos_perso.y+sprite_rect.height), &intersect1)){
      best_dist = Map::distance(collision_startchoose, intersect1);
      map_pos = intersect1;
      map_pos.x -= dec_startchoose.x;
      map_pos.y -= dec_startchoose.y;
    }

    if (Map::intersection(collision_startchoose, map_pos, pos_perso, sf::Vector2f(pos_perso.x+sprite_rect.width, pos_perso.y), &intersect2)){
      float dist = Map::distance(collision_startchoose, intersect2);
      if (dist < best_dist || best_dist == -1){
        best_dist = dist;
        map_pos = intersect2;
        map_pos.x -= dec_startchoose.x;
        map_pos.y -= dec_startchoose.y;
      }
    }

    if (Map::intersection(collision_startchoose, map_pos, sf::Vector2f(pos_perso.x+sprite_rect.width, pos_perso.y), sf::Vector2f(pos_perso.x+sprite_rect.width, pos_perso.y+sprite_rect.height), &intersect3)){
      float dist = Map::distance(collision_startchoose, intersect3);
      if (dist < best_dist || best_dist == -1){
        best_dist = dist;
        map_pos = intersect3;
        map_pos.x -= dec_startchoose.x;
        map_pos.y -= dec_startchoose.y;
      }
    }

    if (Map::intersection(collision_startchoose, map_pos, sf::Vector2f(pos_perso.x, pos_perso.y+sprite_rect.height), sf::Vector2f(pos_perso.x+sprite_rect.width, pos_perso.y+sprite_rect.height), &intersect4)){
      float dist = Map::distance(collision_startchoose, intersect4);
      if (dist < best_dist || best_dist == -1){
        best_dist = dist;
        map_pos = intersect4;
        map_pos.x -= dec_startchoose.x;
        map_pos.y -= dec_startchoose.y;
      }
    }

    std::cout << "map pos : " << map_pos.x << ", " << map_pos.y << std::endl;
  }
  */

  checkCollision(prec_pos, map_pos);
}

void Steve::draw(sf::RenderTarget &target){
  sf::Sprite sp;
  if (sprite == 0){
    sp.setTexture(text_walk0);
  }else{
    sp.setTexture(text_walk1);
  }
  sprite_rect = sp.getGlobalBounds();
  sp.setOrigin(sp.getGlobalBounds().width/2, sp.getGlobalBounds().height/2);
  sp.setPosition(window_pos);

  sf::Sprite sparm(text_arm);
  sparm.setOrigin(sparm.getGlobalBounds().width/2, sparm.getGlobalBounds().height);
  sparm.setPosition(sp.getPosition().x+5, sp.getPosition().y-7);
  sparm.setRotation(angle_arm);

  sf::Sprite spsword(text_sword);
  spsword.setOrigin(10, 34);
  sf::Vector2f pos_sword = sparm.getPosition();
  pos_sword.x += std::cos( (angle_arm-90)*M_PI/180 )*arm_length;
  pos_sword.y += std::sin( (angle_arm-90)*M_PI/180 )*arm_length;
  spsword.setPosition(pos_sword);
  spsword.setRotation(angle_sword);

  target.draw(sp);
  target.draw(spsword);
  target.draw(sparm);
}

void Steve::setright(bool val){
  right = val;
}

void Steve::setleft(bool val){
  left = val;
}

void Steve::setjump(bool val){
  jump = val;
}

void Steve::kick(){
  if (kicking){
    angle_arm-=50;
  }
  kick_gived = false;
  kicking = true;
  kickclock.restart();
}

void Steve::sword_down(){
  angle_sword+=45*time;
  while (angle_sword>360)angle_sword-=360;
}

void Steve::sword_up(){
  angle_sword-=45*time;
  while (angle_sword<0)angle_sword+=360;
}

bool Steve::isJumping(){
  return jumping;
}

void Steve::changeArmAngle(const sf::Vector2f &mousepos){
  if (!kicking){
    sf::Vector2f tmppos =  {window_pos.x+5, window_pos.y-7};
    float angle_start = 0;
    float coeff = 1;
    float o = (float)std::abs(mousepos.x-window_pos.x);
    float a = (float)std::abs(mousepos.y-window_pos.y);
    if (mousepos.x>window_pos.x && mousepos.y>window_pos.y){
      angle_start = 180;
      coeff = -1;
    }else if(mousepos.x<window_pos.x && mousepos.y < window_pos.y){
      angle_start = -90;
    }else if(mousepos.x<window_pos.x && mousepos.y > window_pos.y){
      angle_start = 180;
    }


    float angle = ((std::atan (o/a)*180/M_PI)*coeff)+angle_start;
    if (angle<50)angle = 50;
    if (angle>150)angle=150;
    angle_arm = angle;
  }
}

sf::Vector2f Steve::getMapPosition() const{
  return map_pos;
}

sf::Vector2f Steve::getWindowPosition() const{
  return window_pos;
}

sf::Vector2f Steve::pos_bloclooking(){
  //for (int i = 0)
}


void Steve::setMapPos(const sf::Vector2f &pos){
  map_pos = pos;
}

void Steve::setWindowPos(const sf::Vector2f &pos){
  window_pos = pos;
}

void Steve::pose_block(const sf::Vector2f &mousepos){
  sf::Vector2f pos_mapeyes(map_pos.x, map_pos.y);

  sf::Vector2f vecP2M = {mousepos.x-window_pos.x, mousepos.y-window_pos.y};

  sf::Vector2f mouse_mappos = {pos_mapeyes.x+vecP2M.x, pos_mapeyes.y+vecP2M.y};
  map->poseBlock(pos_mapeyes, mouse_mappos, block_posdist);
}

bool Steve::blockUnderFeets(){
  return map->block_exist(sf::Vector2f(map_pos.x, map_pos.y+(sprite_rect.height/2)+2 ) );
}

void Steve::checkCollision(const sf::Vector2f &prec_pos, const sf::Vector2f &wanted_pos){
  sf::Vector2f up_right, up_left, down_right, down_left;

  up_right = {(sprite_rect.width/2)-1, -(sprite_rect.height/2)+1};
  down_right = {(sprite_rect.width/2)-1, (sprite_rect.height/2)-1};
  down_left = {-(sprite_rect.width/2)+1, (sprite_rect.height/2)-1};
  up_left = {-(sprite_rect.width/2)+1, -(sprite_rect.height/2)+1};

  CollideData col_up_right, col_up_left, col_down_right, col_down_left;

  col_up_right = map->getNearestPosCollide( prec_pos+up_right, wanted_pos+up_right);
  col_down_right = map->getNearestPosCollide( prec_pos+down_right, wanted_pos+down_right);
  col_down_left = map->getNearestPosCollide( prec_pos+down_left, wanted_pos+down_left);
  col_up_left = map->getNearestPosCollide( prec_pos+up_left, wanted_pos+up_left);

  CollideData *first_col = nullptr;

  float best_dist = -1;

  std::vector<SorterCollision> sorted_collision;

  if (col_up_left.side != RIEN){
    float dist = Map::distance(prec_pos+up_left, col_up_left.pos);
    sorted_collision.push_back({dist, col_up_left});
  }
  if (col_down_right.side != RIEN){
    float dist = Map::distance(prec_pos+down_right, col_down_right.pos);
    sorted_collision.push_back({dist, col_down_right});
  }
  if (col_up_right.side != RIEN){
    float dist = Map::distance(prec_pos+up_right, col_up_right.pos);
    sorted_collision.push_back({dist, col_up_right});
  }
  if (col_down_left.side != RIEN){
    float dist = Map::distance(prec_pos+down_left, col_down_left.pos);
    sorted_collision.push_back({dist, col_down_left});
  }

  std::sort(sorted_collision.begin(), sorted_collision.end(),
    [](const SorterCollision &c1, const SorterCollision &c2){
      return c1.dist>c2.dist;
    }
  );

  for (int i = 0; i < sorted_collision.size(); i++){
    if (sorted_collision[i].collision.side == RIGHT){
      std::cout << "first col : RIGHT" << std::endl;
      map_pos.x = sorted_collision[i].collision.pos.x+(sprite_rect.width/2);
    }
    else if (sorted_collision[i].collision.side == LEFT){
      std::cout << "first col : LEFT" << std::endl;
      map_pos.x = sorted_collision[i].collision.pos.x-(sprite_rect.width/2);
    }
    else if (sorted_collision[i].collision.side == TOP){
      std::cout << "first col : TOP" << std::endl;
      map_pos.y = sorted_collision[i].collision.pos.y-(sprite_rect.height/2);
    }
    else if (sorted_collision[i].collision.side == DOWN){
      std::cout << "first col : DOWN" << std::endl;
      map_pos.y = sorted_collision[i].collision.pos.y+(sprite_rect.height/2);
    }
  }
}
