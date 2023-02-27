#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
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
    m_bank = 0;
    int boardNum = getBoardNumber();
    string board_file = assetPath() + "board0" + std::to_string(boardNum) + ".txt";
    Board::LoadResult result = m_bd.loadBoard(board_file);
    
    for(int col = 0; col < BOARD_WIDTH; col++){
        for(int row = 0; row < BOARD_HEIGHT; row++){
            Board::GridEntry ge = m_bd.getContentsOf(col, row);
            int sprite_col = col * SPRITE_WIDTH;
            int sprite_row = row * SPRITE_HEIGHT;
            switch (ge) {
                case Board::empty:
                    break;
                case Board::player:
                    m_yoshi = new Avatar(this, IID_YOSHI, 2, sprite_col, sprite_row);
                    m_peach = new Avatar(this, IID_PEACH, 1, sprite_col, sprite_row);
                    m_actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, sprite_col, sprite_row, true));
                    break;
                case Board::boo:
                    m_actors.push_back(new Boo(this, IID_BOO, sprite_col, sprite_row));
                    m_actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, sprite_col, sprite_row, true));
                    break;
                case Board::bowser:
                    m_actors.push_back(new Bowser(this, IID_BOWSER, sprite_col, sprite_row));
                    m_actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, sprite_col, sprite_row, true));
                    break;
                case Board::blue_coin_square:
                    m_actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, sprite_col, sprite_row, true));
                    break;
                case Board::red_coin_square:
                    m_actors.push_back(new CoinSquare(this, IID_RED_COIN_SQUARE, sprite_col, sprite_row, false));
                    break;
                case Board::up_dir_square:
                    m_actors.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, sprite_col, sprite_row, Actor::up));
                    break;
                case Board::down_dir_square:
                    m_actors.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, sprite_col, sprite_row, Actor::down));
                    break;
                case Board::left_dir_square:
                    m_actors.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, sprite_col, sprite_row, Actor::left));
                    break;
                case Board::right_dir_square:
                    m_actors.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, sprite_col, sprite_row, Actor::right));
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
    if (timeRemaining() <= 0){
        playSound(SOUND_GAME_FINISHED);
        if(m_yoshi->getCoins() + m_yoshi->getStars() > m_peach->getCoins() + m_yoshi->getStars()){
            setFinalScore(m_yoshi->getStars(), m_yoshi->getCoins());
            return GWSTATUS_YOSHI_WON;
        }
        else if(m_yoshi->getCoins() + m_yoshi->getStars() < m_peach->getCoins() + m_yoshi->getStars()){
            setFinalScore(m_peach->getStars(), m_peach->getCoins());
            return GWSTATUS_PEACH_WON;
        }
        else{
            int winner = randInt(1,2);
            if(winner == 1){
                setFinalScore(m_peach->getStars(), m_peach->getCoins());
                return GWSTATUS_PEACH_WON;
            }
            setFinalScore(m_yoshi->getStars(), m_yoshi->getCoins());
            return GWSTATUS_YOSHI_WON;
        }
    }
    
    vector<Actor*>::iterator it = m_actors.begin();
    while(it != m_actors.end()){
        if((*it)->isAlive()){
            (*it)->doSomething();
            it++;
        }
        else{
            delete *it;
            it = m_actors.erase(it);
        }
    }
    
    if(m_yoshi->isAlive()) m_yoshi->doSomething();
    else delete m_yoshi;
    
    if(m_peach->isAlive()) m_peach->doSomething();
    else delete m_peach;
    
//   P1 Roll: 3 Stars: 2 $$: 15 | Time: 75 | Bank: 9 | P2 Roll: 0 Stars: 1 $$: 22 VOR
    int time = timeRemaining();
    int p1Roll = m_peach->getRolls();
    int p2Roll = m_yoshi->getRolls();
    int p1Stars = m_peach->getStars();
    int p2Stars = m_yoshi->getStars();
    int p1Coins = m_peach->getCoins();
    int p2Coins = m_yoshi->getCoins();
    int bank = m_bank;
    setGameStatText("P1 Roll: " + std::to_string(p1Roll) + " Stars: " + std::to_string(p1Stars) + " $$: " + std::to_string(p1Coins) + " | " + "Time: " + std::to_string(time) + " | Bank: " + std::to_string(bank) + " | P2 Roll: " + std::to_string(p2Roll) + " Stars: " + std::to_string(p2Stars) + " $$: " + std::to_string(p2Coins));
    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    vector<Actor*>::iterator it = m_actors.begin();
    while(it != m_actors.end()){
        delete *it;
        it = m_actors.erase(it);
    }
    delete m_peach;
    delete m_yoshi;
}

StudentWorld::~StudentWorld(){
    if(!m_actors.empty()) //so it doesn't duplicate the cleanup
        cleanUp();
}

Board* StudentWorld::getBoard(){
    return &m_bd;
}

Avatar* StudentWorld::getPeach(){
    return m_peach;
}

Avatar* StudentWorld::getYoshi(){
    return m_yoshi;
}

//std::vector<Actor*>::iterator StudentWorld::getActors(){
//    return m_actors.begin();
//}

int StudentWorld::getBankAmt(){
    return m_bank;
}

void StudentWorld::addToBank(int amt){
    m_bank += amt;
}

void StudentWorld::replaceSquare(int x, int y){
    vector<Actor*>::iterator it = m_actors.begin();
    while((*it)->getX() != x && (*it)->getY() != y){
        it++;
    }
    delete *it;
    *it = new DroppingSquare(this, IID_DROPPING_SQUARE, x, y);
}
