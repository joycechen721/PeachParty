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

//======== CHARACTER CLASS (PARENT: ACTOR CLASS) ========
Character::Character(StudentWorld* world, int imageID, double startX, double startY) : Actor(world, imageID, startX, startY, 0){
    m_dir = right;
    m_walking = false;
    m_fork = false;
}
//up over down, right over left
void Character::findNewDir(){
    if(m_dir == left || m_dir == right){
        if(canMoveForward(up)){
            m_dir = up;
//            std::cout << "NEW DIRECTION IS UP" << std::endl;
        }
        else {
            m_dir = down;
//            std::cout << "NEW DIRECTION IS DOWN" << std::endl;
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
bool Character::isFork(){
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
bool Character::canMoveForward(int dir){
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
        case down:
            return bd->getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT - 1) != Board::empty;
        default:
            return false;
    }
}
void Character::moveForward(int dir){
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
        case down:
            moveTo(getX(), getY() - 2);
    }
}
void Character::setMoveDirection(int dir){
    m_dir = dir;
    if(dir == left) setDirection(left);
    else setDirection(right);
}
bool Character::checkUserAction(int playerID){
//    std::cout << "IS FORK" << std::endl;
    int action = getWorld()->getAction(playerID);
    if(action == ACTION_RIGHT && canMoveForward(right) && m_dir != left){
        setDirection(right);
        m_dir = right;
        m_fork = false;
        return true;
    }
    else if(action == ACTION_LEFT && canMoveForward(left) && m_dir != right){
        setDirection(left);
        m_dir = left;
        m_fork = false;
        return true;
    }
    else if(action == ACTION_UP && canMoveForward(up) && m_dir != down){
//        std::cout << "GOING UP" << std::endl;
        setDirection(right);
        m_dir = up;
        m_fork = false;
        return true;
    }
    else if(action == ACTION_DOWN && canMoveForward(down) && m_dir != up){
//        std::cout << "GOING DOWN" << std::endl;
        setDirection(right);
        m_dir = down;
        m_fork = false;
        return true;
    }
    return false;
}
void Character::setWalkDir(int dir){
    m_dir = dir;
}
int Character::getWalkDir(){
    return m_dir;
}
void Character::setWalkStatus(bool isWalking){
    m_walking = isWalking;
}
bool Character::isWalking(){
    return m_walking;
}
void Character::setTicks(int ticks){
    m_ticksToMove = ticks;
}
int Character::getTicks(){
    return m_ticksToMove;
}
void Character::setFork(bool isFork){
    m_fork = isFork;
}
bool Character::getFork(){
    return m_fork;
}

//======== AVATAR CLASS (PARENT: CHARACTER CLASS) ========
Avatar::Avatar(StudentWorld* world, int imageID, int playerID, double startX, double startY) : Character(world, imageID, startX, startY){
    m_id = playerID;
    m_dieRoll = 0;
    m_coins = 0;
    m_stars = 0;
    m_new = true;
    m_overlapped = false;
    m_vortex = nullptr;
}
void Avatar::doSomething(){
    if(!isWalking()){
        int action = getWorld()->getAction(m_id);
        if(action == ACTION_ROLL){
            m_dieRoll = randInt(1, 10);
            setTicks(m_dieRoll * 8);
            setWalkStatus(true);
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
        m_overlapped = true;
        //directional square check
        if(m_changedDir){
            m_changedDir = false;
            if(getFork()) setFork(false);
        }
        //if location is a fork
        else if(getTicks() % 8 == 0 && getFork()){
            if(!checkUserAction(m_id)){
                return;
            }
        }
        //if can't move forward in curr direction, go perpendicular
        else if(getTicks() % 8 == 0 && !canMoveForward(getWalkDir()))
            findNewDir();
        //below will always execute
        moveForward(getWalkDir());
        setTicks(getTicks() - 1);
        if(getTicks() % 8 == 0){
            m_dieRoll--;
            if(isFork()) setFork(true);
        }
        if(getTicks() == 0){
            setWalkStatus(false);
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
Vortex* Avatar::getVortex(){
    return m_vortex;
}
void Avatar::giveVortex(){
//    m_vortex = new Vortex();
    getWorld()->playSound(SOUND_GIVE_VORTEX);
}
bool Avatar::isNew(){
    return m_new;
}
void Avatar::setNew(bool status){
    m_new = status;
}
bool Avatar::isOverlapped(){
    return m_overlapped;
}
void Avatar::setOverlapped(bool overlapped){
    m_overlapped = overlapped;
}
void Avatar::changeDir(bool status){
    m_changedDir = status;
}
void Avatar::swapPlayer(Avatar *avatar){
    //swap x and y coordinates
    int tempX = getX();
    int tempY = getY();
    moveTo(avatar->getX(), avatar->getY());
    avatar->moveTo(tempX, tempY);
    
    //swap number of ticks left
    int tempTicks = getTicks();
    setTicks(avatar->getTicks());
    avatar->setTicks(tempTicks);
    
    //swap walk direction
    int tempDir = getWalkDir();
    setWalkDir(avatar->getWalkDir());
    avatar->setWalkDir(tempDir);
    
    //swap sprite direction
    int tempSpriteDir = getDirection();
    setDirection(avatar->getDirection());
    avatar->setDirection(tempSpriteDir);
    
    //swap roll/walk state
    bool tempStatus = isWalking();
    setWalkStatus(avatar->isWalking());
    avatar->setWalkStatus(tempStatus);
}

//======== VORTEX CLASS (PARENT: ACTOR CLASS) ========
void Vortex::doSomething(){
//    getWorld()->playSound(SOUND_HIT_BY_VORTEX);
}

//======== MONSTER CLASS (PARENT: CHARACTER CLASS) ========
Monster::Monster(StudentWorld* world, int imageID, double startX, double startY) : Character(world, imageID, startX, startY){
    m_pauseCounter = 180;
    m_squares = 0;
}
int Monster::pickDirection(){
    int randomDir = -1;
    do{
        randomDir = 90 * randInt(0,3);
    }
    while(!canMoveForward(randomDir));
    switch(randomDir){
        case left:
            setDirection(left);
            break;
        default:
            setDirection(right);
    }
    return randomDir;
}
void Monster::handleZeroPaused(){
//    std::cout << "HANDLE ZERO PAUSE" << std::endl;
    m_squares = randInt(1, 10);
    setTicks(m_squares * 8);
    setWalkDir(pickDirection());
    setWalkStatus(true);
}
void Monster::startWalking(){
//    std::cout << "STARTED WALKING" << std::endl;
    if(getTicks() % 8 == 0 && getFork()){
        setWalkDir(pickDirection());
    }
    else if(getTicks() % 8 == 0 && !canMoveForward(getWalkDir())){
        findNewDir();
    }
    moveForward(getWalkDir());
    setTicks(getTicks()-1);
}
int Monster::getPauseCounter(){
    return m_pauseCounter;
}
void Monster::setPauseCounter(int count){
    m_pauseCounter = count;
}
void Monster::checkOverlap(Avatar* avatar){
//    std::cout << "OVERLAPPED!!! WITH BOWSER!!" << std::endl;
    if(avatar->isOverlapped() && !avatar->isWalking() && getX() == avatar->getX() && getY() == avatar->getY()){
        executeOverlap(avatar);
        avatar->setOverlapped(false);
    }
}

//======== BOWSER CLASS (PARENT: MONSTER CLASS) ========
Bowser::Bowser(StudentWorld* world, int imageID, double startX, double startY) : Monster(world, imageID, startX, startY){
    
}
void Bowser::executeOverlap(Avatar* avatar){
//    std::cout << "EXECUTED OVERLAPPP!" << std::endl;
    if(randInt(0,1)){
        avatar->addCoins(avatar->getCoins() * -1);
        avatar->addStars(avatar->getStars() * -1);
        getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
    }
}
void Bowser::doSomething(){
    if(!isWalking()){
        checkOverlap(getWorld()->getPeach());
        checkOverlap(getWorld()->getYoshi());
        setPauseCounter(getPauseCounter() - 1);
        if(getPauseCounter() == 0){
            handleZeroPaused();
        }
    }
    else{
        startWalking();
        if(getTicks() == 0){
//            std::cout << "BOWSER JUST STOPPED WALKING" << std::endl;
            setWalkStatus(false);
            setPauseCounter(180);
            //implement below as 25% chance
//            getWorld()->replaceSquare(getX(), getY());
            getWorld()->playSound(SOUND_DROPPING_SQUARE_CREATED);
        }
    }
}

//======== BOO CLASS (PARENT: MONSTER CLASS) ========
Boo::Boo(StudentWorld* world, int imageID, double startX, double startY) : Monster(world, imageID, startX, startY){
    
}
void Boo::doSomething(){
//    getWorld()->playSound(SOUND_BOO_ACTIVATE);
}
void Boo::executeOverlap(Avatar* avatar){
    
}

//======== SQUARE CLASS (PARENT: ACTOR CLASS) ========
Square::Square(StudentWorld* world, int imageID, double startX, double startY) : Actor(world, imageID, startX, startY, 1){
    m_active = true;
}
bool Square::avatarLanded(Avatar* avatar){
    return !avatar->isWalking() && avatar->isNew() && getX() == avatar->getX() && getY() == avatar->getY();
}
bool Square::avatarPassed(Avatar* avatar){
    return avatar->isWalking() && getX() == avatar->getX() && getY() == avatar->getY();
}
void Square::getRandomSquare(int& newX, int& newY){
    Board* bd = getWorld()->getBoard();
    do{
        newX = randInt(1, BOARD_WIDTH);
        newY = randInt(1, BOARD_HEIGHT);
    }
    while(bd->getContentsOf(newX, newY) == Board::empty);
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
//            std::cout<<"PLAY SOUND" << std::endl;
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
    if(avatarPassed(avatar) || avatarLanded(avatar)){
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
//        std::cout << "AVATAR PASSED A DIRECTIONAL SQUARE" << std::endl;
        avatar->setMoveDirection(m_forceDir);
        avatar->changeDir(true);
    }
}

//======== EVENT_SQUARE CLASS (PARENT: SQUARE CLASS) ========
EventSquare::EventSquare(StudentWorld* world, int imageID, double startX, double startY) : Square(world, imageID, startX, startY){
    
}
void EventSquare::doSomething(){
    landAvatar(getWorld()->getPeach());
    landAvatar(getWorld()->getYoshi());
}
void EventSquare::landAvatar(Avatar *avatar){
    if(avatarLanded(avatar)){
        int option = randInt(1,3);
        switch(option){
            case 1: {
//                std::cout << "MOVE TO RANDOM SQUARE" << std::endl;
                int x = -1;
                int y = -1;
                getRandomSquare(x,y);
                avatar->moveTo(SPRITE_WIDTH * x, SPRITE_HEIGHT * y);
                getWorld()->playSound(SOUND_PLAYER_TELEPORT);
                break;
            }
            case 2: {
//                std::cout << "SWAP PLAYERS" << std::endl;
                if(avatar == getWorld()->getPeach())
                    avatar->swapPlayer(getWorld()->getYoshi());
                else
                    avatar->swapPlayer(getWorld()->getPeach());
                getWorld()->playSound(SOUND_PLAYER_TELEPORT);
                break;
            }
            default: {
//                std::cout << "GIVE VORTEX" << std::endl;
                if(avatar->getVortex() == nullptr)
                    avatar->giveVortex();
            }
        }
        avatar->setNew(false);
    }
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
