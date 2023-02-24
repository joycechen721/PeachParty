#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//======== ACTOR CLASS (PARENT: GRAPHOBJECT CLASS) ========
Actor::Actor(StudentWorld* world, int imageID, double startX, double startY, int depth) : GraphObject(imageID, startX, startY, right, depth, 1.0){
    m_world = world;
    m_alive = true;
}
StudentWorld* Actor::getWorld(){
    return m_world;
}
bool Actor::isAlive(){
    return m_alive;
}

//======== AVATAR CLASS (PARENT: ACTOR CLASS) ========
Avatar::Avatar(StudentWorld* world, int imageID, int playerID, double startX, double startY, int depth) : Actor(world, imageID, startX, startY, depth){
    m_id = playerID;
    m_dieRoll = 0;
    m_dir = right;
    m_walking = false;
    m_coins = 0;
    m_stars = 0;
}
//up over down, right over left
void Avatar::findNewDir(){
    if(m_dir == left || m_dir == right){
        if(canMoveForward(up)) m_dir = up;
        else m_dir = down;
        setDirection(right);
    }
    else{
        if(canMoveForward(right)){
            m_dir = right;
            setDirection(right);
        }
        else {
            m_dir = left;
            setDirection(left);
        }
    }
}
bool Avatar::canMoveForward(int dir){
    Board bd = getWorld()->getBoard();
    switch(dir){
        case right:
            return bd.getContentsOf((getX() + 2)/SPRITE_WIDTH, getY()/SPRITE_HEIGHT) != Board::empty;
            break;
        case left:
            return bd.getContentsOf((getX() - 2)/SPRITE_WIDTH, getY()/SPRITE_HEIGHT) != Board::empty;
            break;
        case up:
            return bd.getContentsOf(getX()/SPRITE_WIDTH, (getY() - 2)/SPRITE_HEIGHT) != Board::empty;
            break;
        default:
            return bd.getContentsOf(getX()/SPRITE_WIDTH, (getY() + 2)/SPRITE_HEIGHT) != Board::empty;
    }
}
void Avatar::moveForward(int dir){
    switch(dir){
        case right:
            moveTo(getX() + 2, getY());
            break;
        case left:
            moveTo(getX() - 2, getY());
            break;
        case up:
            moveTo(getX(), getY() - 2);
            break;
        default:
            moveTo(getX(), getY() + 2);
    }
}
void Avatar::doSomething(){
    if(!m_walking){
        int action = getWorld()->getAction(m_id);
        if(action == ACTION_ROLL){
            m_dieRoll = randInt(1, 10);
            m_ticksToMove = m_dieRoll * 8;
            m_walking = true;
        }
        else if(action == ACTION_FIRE){
            getWorld()->playSound(SOUND_PLAYER_FIRE);
        }
        else{
            return;
        }
    }
    else{
        std::cout << "THIS IS THE DIRECTION: " << m_dir << std::endl;
        if(!canMoveForward(m_dir)){
            findNewDir();
        }
        moveForward(m_dir);
        m_ticksToMove--;
        m_dieRoll = m_dieRoll - 1.0/20;
        if(m_ticksToMove == 0)
            m_walking = false;
        
//        int action = getWorld()->getAction(m_id);
//        switch(action){
//            case ACTION_RIGHT:
//                moveTo(getX() + SPRITE_WIDTH, getY());
//                break;
//            case ACTION_LEFT:
//                moveTo(getX() - SPRITE_WIDTH, getY());
//                break;
//            case ACTION_UP:
//                moveTo(getX(), getY() + SPRITE_HEIGHT);
//                break;
//            case ACTION_DOWN:
//                moveTo(getX(), getY() - SPRITE_HEIGHT);
//                break;
//        }
    }
}
void Avatar::addCoins(int coins){
    m_coins += coins;
}
int Avatar::getCoins(){
    return m_coins;
}
int Avatar::getRolls(){
    return m_dieRoll;
}
int Avatar::getStars(){
    return m_stars;
}

//======== MONSTER CLASS ========
Monster::Monster(StudentWorld* world, int imageID, double startX, double startY, int depth) : Actor(world, imageID, startX, startY, depth){
    
}
void Monster::doSomething(){
    
}

//======== SQUARE CLASS ========
Square::Square(StudentWorld* world, int imageID, double startX, double startY, int depth) : Actor(world, imageID, startX, startY, depth){
    
}
void Square::doSomething(){
    
}

//======== BOWSER CLASS ========
Bowser::Bowser(StudentWorld* world, int imageID, double startX, double startY) : Monster(world, imageID, startX, startY, 0){}

Boo::Boo(StudentWorld* world, int imageID, double startX, double startY) : Monster(world, imageID, startX, startY, 0){}

CoinSquare::CoinSquare(StudentWorld* world, int imageID, double startX, double startY) : Square(world, imageID, startX, startY, 1){}

StarSquare::StarSquare(StudentWorld* world, int imageID, double startX, double startY) : Square(world, imageID, startX, startY, 1){}

DirectionalSquare::DirectionalSquare(StudentWorld* world, int imageID, double startX, double startY, int dir) : Square(world, imageID, startX, startY, 1){
    switch(dir){
        case 0:
            setDirection(up);
            break;
        case 1:
            setDirection(down);
            break;
        case 2:
            setDirection(right);
            break;
        case 3:
            setDirection(left);
            break;
    }
}

BankSquare::BankSquare(StudentWorld* world, int imageID, double startX, double startY) : Square(world, imageID, startX, startY, 1){}

EventSquare::EventSquare(StudentWorld* world, int imageID, double startX, double startY) : Square(world, imageID, startX, startY, 1){}

DroppingSquare::DroppingSquare(StudentWorld* world, int imageID, double startX, double startY) : Square(world, imageID, startX, startY, 1){}
