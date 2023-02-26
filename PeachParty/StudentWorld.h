#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include "Actor.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
    public:
        StudentWorld(std::string assetPath);
        virtual int init();
        //runs a single tick in a game
        virtual int move();
        virtual void cleanUp();
        virtual ~StudentWorld();
        Board* getBoard();
        Avatar* getPeach();
        Avatar* getYoshi();
        std::vector<Actor*>::iterator getActors();
        int getBankAmt();
        void addToBank(int amt);
        void replaceSquare(int x, int y);

    private:
        Board m_bd;
        int m_bank;
        Avatar* m_peach; //make into Avatar pointers, cause need to get coins
        Avatar* m_yoshi;
        std::vector<Actor*> m_actors;
};

#endif // STUDENTWORLD_H_
