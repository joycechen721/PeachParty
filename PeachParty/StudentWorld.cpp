#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    startCountdownTimer(100);
    
    Board bd;
    ostringstream oss;
    oss << getBoardNumber();
    string board_file = assetPath() + "board0" + oss.str() + ".txt";
    Board::LoadResult result = bd.loadBoard(board_file);
    
    for(int col = 0; col < BOARD_WIDTH; col++){
        for(int row = 0; row < BOARD_HEIGHT; row++){
            Board::GridEntry ge = bd.getContentsOf(col, row);
            int sprite_col = col * SPRITE_WIDTH;
            int sprite_row = row * SPRITE_HEIGHT;
            switch (ge) {
                case Board::empty:
                    break;
                case Board::player:
                    Yoshi = new Avatar(this, IID_YOSHI, 2, sprite_col, sprite_row);
                    Peach = new Avatar(this, IID_PEACH, 1, sprite_col, sprite_row);
                    break;
                case Board::boo:
                    m_actors.push_back(new Boo(this, IID_BOO, sprite_col, sprite_row));
                    break;
                case Board::bowser:
                    m_actors.push_back(new Bowser(this, IID_BOWSER, sprite_col, sprite_row));
                    break;
                case Board::blue_coin_square:
                    m_actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, sprite_col, sprite_row));
                    break;
                case Board::red_coin_square:
                    m_actors.push_back(new CoinSquare(this, IID_RED_COIN_SQUARE, sprite_col, sprite_row));
                    break;
                case Board::up_dir_square:
                    m_actors.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, sprite_col, sprite_row, 0));
                    break;
                case Board::down_dir_square:
                    m_actors.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, sprite_col, sprite_row, 1));
                    break;
                case Board::left_dir_square:
                    m_actors.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, sprite_col, sprite_row, 2));
                    break;
                case Board::right_dir_square:
                    m_actors.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, sprite_col, sprite_row, 3));
                    break;
                case Board::star_square:
                    m_actors.push_back(new StarSquare(this, IID_STAR_SQUARE, sprite_col, sprite_row));
                    break;
                case Board::bank_square:
                    m_actors.push_back(new BankSquare(this, IID_BANK_SQUARE, sprite_col, sprite_row));
                    break;
                case Board::event_square:
                    m_actors.push_back(new EventSquare(this, IID_EVENT_SQUARE, sprite_col, sprite_row));
                    break;
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.
    Yoshi->doSomething();
    Peach->doSomething();
    setGameStatText("Game will end in a few seconds");
    
    if (timeRemaining() <= 0)
		return GWSTATUS_NOT_IMPLEMENTED;
    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    vector<GraphObject*>::iterator it = m_actors.begin();
    while(it != m_actors.end()){
        delete *it;
        it = m_actors.erase(it);
    }
    delete Peach;
    delete Yoshi;
}
