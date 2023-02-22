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

private:
    Avatar* Peach;
    Avatar* Yoshi;
    std::vector<GraphObject*> m_actors;
};

#endif // STUDENTWORLD_H_
