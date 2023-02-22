#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: public GraphObject {
    public:
        Actor(GameWorld* world, int imageID, double startX, double startY);
        GameWorld* getWorld();
    private:
        GameWorld* m_world;
};

class Vortex: public Actor {
    
};

//Peach and Yoshi
class Avatar: public Actor {
    public:
        Avatar(GameWorld* world, int imageID, int playerID, double startX, double startY);
        void doSomething();
    private:
        int m_id;
        bool m_walking;
        int m_coins;
        int m_stars;
        Vortex* m_vortex;
        int m_dieRoll;
        int m_ticksToMove;
};

//Bowser
class Bowser: public Actor {
    public:
        Bowser(GameWorld* world, int imageID, double startX, double startY);
};

//Boo
class Boo: public Actor {
    public:
        Boo(GameWorld* world, int imageID, double startX, double startY);
};

//blue and red coin squares
class CoinSquare: public Actor {
    public:
        CoinSquare(GameWorld* world, int imageID, double startX, double startY);
};

class StarSquare: public Actor {
    public:
        StarSquare(GameWorld* world, int imageID, double startX, double startY);
};

class DirectionalSquare: public Actor {
    public:
        DirectionalSquare(GameWorld* world, int imageID, double startX, double startY, int dir);
};

class BankSquare: public Actor {
    public:
        BankSquare(GameWorld* world, int imageID, double startX, double startY);
};

class EventSquare: public Actor {
    public:
        EventSquare(GameWorld* world, int imageID, double startX, double startY);
};

class DroppingSquare: public Actor {
    public:
        DroppingSquare(GameWorld* world, int imageID, double startX, double startY);
};

#endif // ACTOR_H_
