//
//  Player.hpp
//  CS32 P3
//
//  Created by Arash Dewan on 5/29/23.
//

#ifndef Player_hpp
#define Player_hpp

#include <string>
#include "Board.h"
#include "Side.h"

//========================================================================
// Timer t;                 // create and start a timer
// t.start();               // restart the timer
// double d = t.elapsed();  // milliseconds since timer was last started
//========================================================================

#include <chrono>

class Timer
{
  public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::chrono::high_resolution_clock::now();
    }
    double elapsed() const
    {
        std::chrono::duration<double, std::milli> diff =
                          std::chrono::high_resolution_clock::now() - m_time;
        return diff.count();
    }
  private:
    std::chrono::high_resolution_clock::time_point m_time;
};

//========================================================================

class Player
{
public:
    Player(std::string name) : m_name(name) {}
    std::string name() const {return m_name;}
    virtual bool isInteractive() const {return false;}
    virtual int chooseMove(const Board& b, Side s) const = 0;
    virtual ~Player() {}
private:
    std::string m_name;
};

class HumanPlayer : public Player
{
public:
    HumanPlayer(std::string name) : Player(name) {}
    virtual ~HumanPlayer() {}
    virtual int chooseMove(const Board& b, Side s) const;
    virtual bool isInteractive() const {return true;}
};

class BadPlayer : public Player
{
public:
    BadPlayer(std::string name) : Player(name) {}
    virtual ~BadPlayer() {}
    virtual int chooseMove(const Board& b, Side s) const;
};

class SmartPlayer : public Player
{
public:
    SmartPlayer(std::string name) : Player(name) {}
    virtual ~SmartPlayer() {}
    virtual int chooseMove(const Board& b, Side s) const;
private:
    int evaluate(const Board& b, Side s) const;
    int minimax(const Board &b, Side s, int depth, bool max, Timer& timer) const;
};

#endif /* Player_hpp */
