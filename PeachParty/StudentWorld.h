#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
    //constructing a representation of the current board in your StudentWorld object and populating it with initial objects (e.g., squares, baddies, Yoshi and Peach), using one or more data structures that you come up with
  virtual int init();
    //runs a single tick in a game
  virtual int move();
  virtual void cleanUp();

private:
};

#endif // STUDENTWORLD_H_
