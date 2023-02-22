#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(GameWorld* world, int imageID, double startX, double startY) : GraphObject(imageID, startX, startY, right, 0, 1.0){
    m_world = world;
}

GameWorld* Actor::getWorld(){
    return m_world;
}

Avatar::Avatar(GameWorld* world, int imageID, int playerID, double startX, double startY) : Actor(world, imageID, startX, startY){
    m_id = playerID;
    m_walking = false;
    m_coins = 0;
    m_stars = 0;
}

void Avatar::doSomething(){
    if(!m_walking){
        int action = getWorld()->getAction(m_id);
        if(action == ACTION_ROLL){
            m_dieRoll = randInt(1, 10);
            std::cout << m_dieRoll << std::endl;
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
        
    }
}

Bowser::Bowser(GameWorld* world, int imageID, double startX, double startY) : Actor(world, imageID, startX, startY){}

Boo::Boo(GameWorld* world, int imageID, double startX, double startY) : Actor(world, imageID, startX, startY){}

CoinSquare::CoinSquare(GameWorld* world, int imageID, double startX, double startY) : Actor(world, imageID, startX, startY){}

StarSquare::StarSquare(GameWorld* world, int imageID, double startX, double startY) : Actor(world, imageID, startX, startY){}

DirectionalSquare::DirectionalSquare(GameWorld* world, int imageID, double startX, double startY, int dir) : Actor(world, imageID, startX, startY){
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

BankSquare::BankSquare(GameWorld* world, int imageID, double startX, double startY) : Actor(world, imageID, startX, startY){}

EventSquare::EventSquare(GameWorld* world, int imageID, double startX, double startY) : Actor(world, imageID, startX, startY){}

DroppingSquare::DroppingSquare(GameWorld* world, int imageID, double startX, double startY) : Actor(world, imageID, startX, startY){}
