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
void Actor::setAlive(bool isAlive){
    m_alive = isAlive;
}

//======== AVATAR CLASS (PARENT: ACTOR CLASS) ========
Avatar::Avatar(StudentWorld* world, int imageID, int playerID, double startX, double startY, int depth) : Actor(world, imageID, startX, startY, depth){
    std::cout << "START: " << startX << " END: " << startY << std::endl;
    m_id = playerID;
    m_dieRoll = 0;
    m_dir = right;
    m_walking = false;
    m_coins = 3;
    m_stars = 0;
    m_new = false;
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
            return bd.getContentsOf(getX()/SPRITE_WIDTH + 1, getY()/SPRITE_HEIGHT) != Board::empty;
            break;
        case left:
            return bd.getContentsOf(getX()/SPRITE_WIDTH - 1, getY()/SPRITE_HEIGHT) != Board::empty;
            break;
        case up:
            return bd.getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT - 1) != Board::empty;
            break;
        default:
            return bd.getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT + 1) != Board::empty;
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
        m_new = true;
        if(m_ticksToMove % 8 == 0 && !canMoveForward(m_dir)){
            findNewDir();
        }
        //check if can move forward in other directions as well
        //have user input
        moveForward(m_dir);
        m_ticksToMove--;
        if(m_ticksToMove % 8 == 0) m_dieRoll--;
        if(m_ticksToMove == 0){
            m_walking = false;
        }
    }
}
//void Avatar::canMoveDiff(){
//
//}
//int Avatar::chooseDir(int dir1, int dir2){
//    int action = getWorld()->getAction(m_id);
//    switch(action){
//        case ACTION_RIGHT:
//            moveTo(getX() + SPRITE_WIDTH, getY());
//            break;
//        case ACTION_LEFT:
//            moveTo(getX() - SPRITE_WIDTH, getY());
//            break;
//        case ACTION_UP:
//            moveTo(getX(), getY() + SPRITE_HEIGHT);
//            break;
//        case ACTION_DOWN:
//            moveTo(getX(), getY() - SPRITE_HEIGHT);
//            break;
//    }
//}
void Avatar::addCoins(int coins){
    m_coins += coins;
}
void Avatar::addStars(int stars){
    m_stars += stars;
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
bool Avatar::isWalking(){
    return m_walking;
}
bool Avatar::isNew(){
    return m_new;
}
void Avatar::setNew(bool status){
    m_new = status;
}

//======== MONSTER CLASS (PARENT: ACTOR CLASS)========
Monster::Monster(StudentWorld* world, int imageID, double startX, double startY, int depth) : Actor(world, imageID, startX, startY, depth){
    
}
void Monster::doSomething(){
    
}

//======== BOWSER CLASS (PARENT: MONSTER CLASS) ========
Bowser::Bowser(StudentWorld* world, int imageID, double startX, double startY) : Monster(world, imageID, startX, startY, 0){}

//======== BOO CLASS (PARENT: MONSTER CLASS) ========
Boo::Boo(StudentWorld* world, int imageID, double startX, double startY) : Monster(world, imageID, startX, startY, 0){}


//======== SQUARE CLASS (PARENT: ACTOR CLASS) ========
Square::Square(StudentWorld* world, int imageID, double startX, double startY, int depth) : Actor(world, imageID, startX, startY, depth){
    m_active = true;
    m_yoshi = world->getYoshi();
    m_peach = world->getPeach();
}
void Square::doSomething(){
    
}
Avatar* Square::getYoshi(){
    return m_yoshi;
}
Avatar* Square::getPeach(){
    return m_peach;
}
bool Square::avatarLanded(Avatar* avatar){
    return avatar->getX() == getX() && avatar->getY() == getY() && !avatar->isWalking() && avatar->isNew();
}

//======== COINSQUARE CLASS (PARENT: SQUARE CLASS) ========
CoinSquare::CoinSquare(StudentWorld* world, int imageID, double startX, double startY, bool isBlue) : Square(world, imageID, startX, startY, 1){
    m_give = isBlue;
}
void CoinSquare::doSomething(){
    if(!isAlive()){
        return;
    }
    landAvatar(getPeach());
    landAvatar(getYoshi());
}
void CoinSquare::landAvatar(Avatar* avatar){
    if(avatarLanded(avatar)){
        if(m_give){
            avatar->addCoins(3);
            getWorld()->playSound(SOUND_GIVE_COIN);
        }
        else{
            avatar->addCoins(-3);
            getWorld()->playSound(SOUND_TAKE_COIN);
        }
        avatar->setNew(false);
    }
}

//======== STARSQUARE CLASS (PARENT: COINSQUARE CLASS) ========
StarSquare::StarSquare(StudentWorld* world, int imageID, double startX, double startY) : CoinSquare(world, imageID, startX, startY, false){
    
}
void StarSquare::doSomething(){
    landAvatar(getPeach());
    landAvatar(getYoshi());
}
void StarSquare::landAvatar(Avatar* avatar){
    if(avatarLanded(avatar)){
        if(avatar->getCoins() < 20) return;
        avatar->addCoins(-20);
        avatar->addStars(1);
        getWorld()->playSound(SOUND_GIVE_STAR);
        avatar->setNew(false);
    }
}

BankSquare::BankSquare(StudentWorld* world, int imageID, double startX, double startY) : Square(world, imageID, startX, startY, 1){
    
}
void BankSquare::doSomething(){
    landAvatar(getPeach());
    landAvatar(getYoshi());
    moveAvatar(getPeach());
    moveAvatar(getYoshi());
}
void BankSquare::landAvatar(Avatar *avatar){
    int bankStash = getWorld()->getBankAmt();
    avatar->addCoins(bankStash);
    getWorld()->setBankAmt(0);
    getWorld()->playSound(SOUND_WITHDRAW_BANK);
}
void BankSquare::moveAvatar(Avatar *avatar){
    if(avatar->getX() == getX() && avatar->getY() == getY() && avatar->isWalking()){
        int deduction = avatar->getCoins() < 5 ? avatar->getCoins() : 5;
        avatar->addCoins(deduction * -1);
        getWorld()->setBankAmt(getWorld()->getBankAmt() + deduction);
        getWorld()->playSound(SOUND_DEPOSIT_BANK);
    };
}

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
void DirectionalSquare::doSomething(){
    
}
void DirectionalSquare::landAvatar(Avatar *avatar){
    
}

EventSquare::EventSquare(StudentWorld* world, int imageID, double startX, double startY) : Square(world, imageID, startX, startY, 1){
    
}
void EventSquare::doSomething(){
    
}
void EventSquare::landAvatar(Avatar *avatar){
    
}

DroppingSquare::DroppingSquare(StudentWorld* world, int imageID, double startX, double startY) : Square(world, imageID, startX, startY, 1){
    
}
void DroppingSquare::doSomething(){
    
}
void DroppingSquare::landAvatar(Avatar *avatar){
    
}
