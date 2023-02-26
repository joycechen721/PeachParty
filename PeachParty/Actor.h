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
        void setAlive(bool isAlive);
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
        void setMoveDirection(int dir);
        virtual void doSomething();
        void addCoins(int coins);
        void addStars(int stars);
        int getCoins();
        int getRolls();
        int getStars();
        bool isWalking();
        bool isNew();
        void setNew(bool status);
        bool isFork();
        void changeDir(bool status);
    private:
        int m_id;
        int m_dir;
        bool m_walking;
        int m_coins;
        int m_stars;
        Vortex* m_vortex;
        int m_dieRoll;
        int m_ticksToMove;
        bool m_new;
        bool m_fork;
        bool m_changedDir;
};

//Monster class for Bowser and Boo
class Monster: public Actor{
    public:
        Monster(StudentWorld* world, int imageID, double startX, double startY, int depth);
        virtual void doSomething();
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
        Square(StudentWorld* world, int imageID, double startX, double startY);
        virtual void doSomething() = 0;
        virtual void landAvatar(Avatar* avatar) = 0;
        bool isActive();
        bool avatarLanded(Avatar* avatar);
        bool avatarPassed(Avatar* avatar);
    private:
        bool m_active;
};

//blue and red coin squares
class CoinSquare: public Square {
    public:
        CoinSquare(StudentWorld* world, int imageID, double startX, double startY, bool isBlue);
        void doSomething();
        void landAvatar(Avatar* avatar);
    private:
        bool m_give;
};

class StarSquare: public Square {
    public:
        StarSquare(StudentWorld* world, int imageID, double startX, double startY);
        void doSomething();
        void landAvatar(Avatar* avatar);
};

class DirectionalSquare: public Square {
    public:
        DirectionalSquare(StudentWorld* world, int imageID, double startX, double startY, int dir);
        void doSomething();
        void landAvatar(Avatar* avatar);
    private:
        int m_forceDir;
};

class BankSquare: public Square {
    public:
        BankSquare(StudentWorld* world, int imageID, double startX, double startY);
        void doSomething();
        void landAvatar(Avatar* avatar);
        void moveAvatar(Avatar* avatar);
};

class EventSquare: public Square {
    public:
        EventSquare(StudentWorld* world, int imageID, double startX, double startY);
        void doSomething();
        void landAvatar(Avatar* avatar);
};

class DroppingSquare: public Square {
    public:
        DroppingSquare(StudentWorld* world, int imageID, double startX, double startY);
        void doSomething();
        void landAvatar(Avatar* avatar);
};

#endif // ACTOR_H_
