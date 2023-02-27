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
    public:
        virtual void doSomething();
    
};

class Character: public Actor{
    public:
        Character(StudentWorld* world, int imageID, double startX, double startY);
        virtual void doSomething() = 0;
        void findNewDir();
        void moveForward(int dir);
        void setMoveDirection(int dir);
        bool canMoveForward(int dir);
        bool checkUserAction(int playerID);
        bool isFork();
        //getters and setters
        void setWalkDir(int dir);
        int getWalkDir();
        void setWalkStatus(bool isWalking);
        bool isWalking();
        void setTicks(int ticks);
        int getTicks();
        void setFork(bool isFork);
        bool getFork();
    private:
        int m_dir;
        bool m_walking;
        int m_ticksToMove;
        bool m_fork;
};

//Avatar Class for Peach and Yoshi
class Avatar: public Character {
    public:
        Avatar(StudentWorld* world, int imageID, int playerID, double startX, double startY);
        virtual void doSomething();
        void addCoins(int coins);
        void addStars(int stars);
        int getCoins();
        int getRolls();
        int getStars();
        bool isNew();
        void setNew(bool status);
        bool isOverlapped();
        void setOverlapped(bool overlapped);
        void changeDir(bool status);
        void swapPlayer(Avatar* avatar);
        Vortex* getVortex();
        void giveVortex();
    private:
        int m_id;
        int m_coins;
        int m_stars;
        int m_dieRoll;
        bool m_new;
        bool m_overlapped;
        bool m_changedDir;
        Vortex* m_vortex;
};

//Monster class for Bowser and Boo
class Monster: public Character{
    public:
        Monster(StudentWorld* world, int imageID, double startX, double startY);
        virtual void doSomething() = 0;
        void handleZeroPaused();
        void startWalking();
        int pickDirection();
        int getPauseCounter();
        void setPauseCounter(int count);
        void checkOverlap(Avatar* avatar);
        virtual void executeOverlap(Avatar* avatar) = 0;
    private:
        int m_pauseCounter;
        int m_squares;
};

//Bowser
class Bowser: public Monster {
    public:
        Bowser(StudentWorld* world, int imageID, double startX, double startY);
        virtual void doSomething();
        virtual void executeOverlap(Avatar* avatar);
};

//Boo
class Boo: public Monster {
    public:
        Boo(StudentWorld* world, int imageID, double startX, double startY);
        virtual void doSomething();
        virtual void executeOverlap(Avatar* avatar);
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
        void getRandomSquare(int& newX, int& newY);
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
