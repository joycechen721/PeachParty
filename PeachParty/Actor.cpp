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
    m_id = playerID;
    m_dieRoll = 0;
    m_dir = right;
    m_walking = false;
    m_coins = 3;
    m_stars = 0;
    m_new = false;
    m_fork = false;
}
//up over down, right over left
void Avatar::findNewDir(){
    if(m_dir == left || m_dir == right){
        if(canMoveForward(up)){
            m_dir = up;
            std::cout << "NEW DIRECTION IS UP" << std::endl;
        }
        else {
            m_dir = down;
            std::cout << "NEW DIRECTION IS DOWN" << std::endl;
        }
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
bool Avatar::isFork(){
    switch(m_dir){
        case up:
            return (canMoveForward(up) && (canMoveForward(left) || canMoveForward(right))) || (canMoveForward(left) && canMoveForward(right));
        case left:
            return (canMoveForward(left) && (canMoveForward(up) || canMoveForward(down))) || (canMoveForward(up) && canMoveForward(down));
        case right:
            return (canMoveForward(right) && (canMoveForward(up) || canMoveForward(down))) || (canMoveForward(up) && canMoveForward(down));
        default:
            return (canMoveForward(down) && (canMoveForward(left) || canMoveForward(right))) || (canMoveForward(left) && canMoveForward(right));
    }
}
bool Avatar::canMoveForward(int dir){
    Board* bd = getWorld()->getBoard();
    switch(dir){
        case right:
            return bd->getContentsOf(getX()/SPRITE_WIDTH + 1, getY()/SPRITE_HEIGHT) != Board::empty;
            break;
        case left:
            return bd->getContentsOf(getX()/SPRITE_WIDTH - 1, getY()/SPRITE_HEIGHT) != Board::empty;
            break;
        case up:
            return bd->getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT + 1) != Board::empty;
            break;
        default:
            return bd->getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT - 1) != Board::empty;
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
            moveTo(getX(), getY() + 2);
            break;
        default:
            moveTo(getX(), getY() - 2);
    }
}
void Avatar::setMoveDirection(int dir){
    m_dir = dir;
    if(dir == left) setDirection(left);
    else setDirection(right);
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
    //if player is in a walking state
    else{
        m_new = true;
        //directional square check
        if(m_changedDir){
            m_changedDir = false;
            if(m_fork) m_fork = false;
        }
        //if location is a fork
        else if(m_ticksToMove % 8 == 0 && m_fork){
            std::cout << "IS FORK" << std::endl;
            int action = getWorld()->getAction(m_id);
            if(action == ACTION_RIGHT && canMoveForward(right)){
                setDirection(right);
                m_dir = right;
                m_fork = false;
            }
            else if(action == ACTION_LEFT && canMoveForward(left)){
                setDirection(left);
                m_dir = left;
                m_fork = false;
            }
            else if(action == ACTION_UP && canMoveForward(up)){
                std::cout << "GOING UP" << std::endl;
                setDirection(right);
                m_dir = up;
                m_fork = false;
            }
            else if(action == ACTION_DOWN && canMoveForward(down)){
                std::cout << "GOING DOWN" << std::endl;
                setDirection(right);
                m_dir = down;
                m_fork = false;
            }
            else{
                return;
            }
        }
        //if can't move forward in curr direction, go perpendicular
        else if(m_ticksToMove % 8 == 0 && !canMoveForward(m_dir))
            findNewDir();
        //below will always execute
        moveForward(m_dir);
        m_ticksToMove--;
        if(m_ticksToMove % 8 == 0){
            m_dieRoll--;
            if(isFork()) m_fork = true;
        }
        if(m_ticksToMove == 0){
            m_walking = false;
        }
    }
}
void Avatar::addCoins(int coins){
    m_coins += coins;
    if(m_coins < 0) m_coins = 0;
}
void Avatar::addStars(int stars){
    m_stars += stars;
    if(m_stars < 0) m_stars = 0;
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
void Avatar::changeDir(bool status){
    m_changedDir = status;
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
Square::Square(StudentWorld* world, int imageID, double startX, double startY) : Actor(world, imageID, startX, startY, 1){
    m_active = true;
}
void Square::doSomething(){
    
}
bool Square::avatarLanded(Avatar* avatar){
    return !avatar->isWalking() && avatar->isNew() && getX() == avatar->getX() && getY() == avatar->getY();
}
bool Square::avatarPassed(Avatar* avatar){
    return avatar->isWalking() && getX() == avatar->getX() && getY() == avatar->getY();
}

//======== COIN_SQUARE CLASS (PARENT: SQUARE CLASS) ========
CoinSquare::CoinSquare(StudentWorld* world, int imageID, double startX, double startY, bool isBlue) : Square(world, imageID, startX, startY){
    m_give = isBlue;
}
void CoinSquare::doSomething(){
    if(!isAlive()){
        return;
    }
    landAvatar(getWorld()->getPeach());
    landAvatar(getWorld()->getYoshi());
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

//======== STAR_SQUARE CLASS (PARENT: SQUARE CLASS) ========
StarSquare::StarSquare(StudentWorld* world, int imageID, double startX, double startY) : Square(world, imageID, startX, startY){
    
}
void StarSquare::doSomething(){
    landAvatar(getWorld()->getPeach());
    landAvatar(getWorld()->getYoshi());
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

//======== BANK_SQUARE CLASS (PARENT: SQUARE CLASS) ========
BankSquare::BankSquare(StudentWorld* world, int imageID, double startX, double startY) : Square(world, imageID, startX, startY){
    
}
void BankSquare::doSomething(){
    landAvatar(getWorld()->getPeach());
    landAvatar(getWorld()->getYoshi());
    moveAvatar(getWorld()->getPeach());
    moveAvatar(getWorld()->getYoshi());
}
void BankSquare::landAvatar(Avatar *avatar){
    if(avatarLanded(avatar)){
        int bankStash = getWorld()->getBankAmt();
        avatar->addCoins(bankStash);
        getWorld()->addToBank(bankStash * -1);
        getWorld()->playSound(SOUND_WITHDRAW_BANK);
    }
}
void BankSquare::moveAvatar(Avatar *avatar){
    if(avatarPassed(avatar)){
        int deduction = (avatar->getCoins()) < 5 ? avatar->getCoins() : 5;
        avatar->addCoins(deduction * -1);
        getWorld()->addToBank(deduction);
        getWorld()->playSound(SOUND_DEPOSIT_BANK);
//        avatar->setNew(false);
    };
}

//======== DIRECTIONAL_SQUARE CLASS (PARENT: SQUARE CLASS) ========
DirectionalSquare::DirectionalSquare(StudentWorld* world, int imageID, double startX, double startY, int dir) : Square(world, imageID, startX, startY){
    setDirection(dir);
    m_forceDir = dir;
}
void DirectionalSquare::doSomething(){
    landAvatar(getWorld()->getPeach());
    landAvatar(getWorld()->getYoshi());
}
void DirectionalSquare::landAvatar(Avatar *avatar){
    if(avatarLanded(avatar) || avatarPassed(avatar)){
        std::cout << "AVATAR PASSED A DIRECTIONAL SQUARE" << std::endl;
        avatar->setMoveDirection(m_forceDir);
        avatar->changeDir(true);
    }
}

//======== EVENT_SQUARE CLASS (PARENT: SQUARE CLASS) ========
EventSquare::EventSquare(StudentWorld* world, int imageID, double startX, double startY) : Square(world, imageID, startX, startY){
    
}
void EventSquare::doSomething(){
    
}
void EventSquare::landAvatar(Avatar *avatar){
    
}

//======== DROPPING_SQUARE CLASS (PARENT: SQUARE CLASS) ========
DroppingSquare::DroppingSquare(StudentWorld* world, int imageID, double startX, double startY) : Square(world, imageID, startX, startY){
    
}
void DroppingSquare::doSomething(){
    landAvatar(getWorld()->getPeach());
    landAvatar(getWorld()->getYoshi());
}
void DroppingSquare::landAvatar(Avatar *avatar){
    if(avatarLanded(avatar)){
        int option = randInt(1,2);
        switch(option){
            case 1:
                avatar->addCoins(-10);
                break;
            case 2:
                avatar->addStars(-1);
                break;
        }
        getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
    }
}
