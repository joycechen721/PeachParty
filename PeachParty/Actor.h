#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: public GraphObject {
    public:
        Actor(StudentWorld* world, int imageID, double startX, double startY, int depth);
        StudentWorld* getWorld();
        bool isAlive();
        virtual void doSomething() = 0;
    private:
        StudentWorld* m_world;
        bool m_alive;
};

class Vortex: public Actor {
    
};

//Avatar Class for Peach and Yoshi
class Avatar: public Actor {
    public:
        Avatar(StudentWorld* world, int imageID, int playerID, double startX, double startY, int depth);
        void findNewDir();
        bool canMoveForward(int dir);
        void moveForward(int dir);
        void doSomething();
        void addCoins(int coins);
        int getCoins();
        int getRolls();
        int getStars();
    private:
        int m_id;
        int m_dir;
        bool m_walking;
        int m_coins;
        int m_stars;
        Vortex* m_vortex;
        int m_dieRoll;
        int m_ticksToMove;
};

//Monster class for Bowser and Boo
class Monster: public Actor{
    public:
        Monster(StudentWorld* world, int imageID, double startX, double startY, int depth);
        void doSomething();
};

//Bowser
class Bowser: public Monster {
    public:
        Bowser(StudentWorld* world, int imageID, double startX, double startY);
};

//Boo
class Boo: public Monster {
    public:
        Boo(StudentWorld* world, int imageID, double startX, double startY);
};

//SQUARE CLASS
class Square: public Actor {
    public:
        Square(StudentWorld* world, int imageID, double startX, double startY, int depth);
        void doSomething();
};

//blue and red coin squares
class CoinSquare: public Square {
    public:
        CoinSquare(StudentWorld* world, int imageID, double startX, double startY);
};

class StarSquare: public Square {
    public:
        StarSquare(StudentWorld* world, int imageID, double startX, double startY);
};

class DirectionalSquare: public Square {
    public:
        DirectionalSquare(StudentWorld* world, int imageID, double startX, double startY, int dir);
};

class BankSquare: public Square {
    public:
        BankSquare(StudentWorld* world, int imageID, double startX, double startY);
};

class EventSquare: public Square {
    public:
        EventSquare(StudentWorld* world, int imageID, double startX, double startY);
};

class DroppingSquare: public Square {
    public:
        DroppingSquare(StudentWorld* world, int imageID, double startX, double startY);
};

#endif // ACTOR_H_
