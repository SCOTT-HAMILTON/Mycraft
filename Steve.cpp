#include "Steve.h"

Steve::Steve(const sf::Vector2f &pos ) : Entity()
{
  right = false;
  left = false;
  jump = false;
  jumping = false;
  jumpy = 30.0f;
  vec_jump = -jumpy;
  gravity = 5.0f;
  kicking = false;
  kick_gived = false;

  this->pos = pos;
  std::cout << "steve the pos : " << this->pos.x << ", " << this->pos.y << std::endl;
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
    pos.x+=10*time;
  }else if (left){
    pos.x-=10*time;
  }

  if (jump || jumping){
    if (!jumping){
      jumping = true;
      vec_jump = -jumpy;
      jumpclock.restart();
      while (jumpclock.getElapsedTime().asMilliseconds()<1);
    }
    if (jumpclock.getElapsedTime().asMilliseconds()>1000/60){
      float delay = jumpclock.getElapsedTime().asMilliseconds();
      pos.y += vec_jump*0.6;
      vec_jump+=gravity;
      if (vec_jump > jumpy)jumping = false;
      jumpclock.restart();
    }
  }

}

void Steve::draw(sf::RenderTarget &target){
  sf::Sprite sp;
  if (sprite == 0){
    sp.setTexture(text_walk0);
  }else{
    sp.setTexture(text_walk1);
  }
  sp.setOrigin(sp.getGlobalBounds().width/2, sp.getGlobalBounds().height/2);
  sp.setPosition(pos);

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

sf::Vector2f Steve::getPosition() const{
  return pos;
}

void Steve::sword_down(){
  angle_sword+=45*time;
  while (angle_sword>360)angle_sword-=360;
}

void Steve::sword_up(){
  angle_sword-=45*time;
  while (angle_sword<0)angle_sword+=360;
}

void Steve::changeArmAngle(const sf::Vector2f &mousepos){
  if (!kicking){
    sf::Vector2f tmppos =  {pos.x+5, pos.y-7};
    float angle_start = 0;
    float coeff = 1;
    float o = (float)std::abs(mousepos.x-pos.x);
    float a = (float)std::abs(mousepos.y-pos.y);
    if (mousepos.x>pos.x && mousepos.y>pos.y){
      angle_start = 180;
      coeff = -1;
    }else if(mousepos.x<pos.x && mousepos.y < pos.y){
      angle_start = -90;
    }else if(mousepos.x<pos.x && mousepos.y > pos.y){
      angle_start = 180;
    }


    float angle = ((std::atan (o/a)*180/M_PI)*coeff)+angle_start;
    if (angle<50)angle = 50;
    if (angle>150)angle=150;
    angle_arm = angle;
  }
}
