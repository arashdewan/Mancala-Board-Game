//
//  Player.cpp
//  CS32 P3
//
//  Created by Arash Dewan on 5/29/23.
//

#include "Player.h"
#include <iostream>
using namespace std;
//Every concrete class derived from this class must implement this function so that if the player were to be playing side s and had to make a move given board b, the function returns the move the player would choose. If no move is possible, return −1.
//Since this class is designed as a base class, it should have a virtual destructor.


int HumanPlayer::chooseMove(const Board& b, Side s) const
{
    if (b.beansInPlay(s) == 0)
        return -1;
    int n_hole;
    do {
        cout << "Select a hole, " + name() + ": ";
        cin >> n_hole;
        if (n_hole <= 0 || n_hole > b.holes())
            cout << "The hole number must be from 1 to " + to_string(b.holes()) + "." << endl;
        if (b.beans(s, n_hole) == 0)
            cout << "There are no beans in that hole." << endl;
    }while (b.beans(s, n_hole) <= 0 ||n_hole <= 0 || n_hole > b.holes());
    return n_hole;
}

int BadPlayer::chooseMove(const Board& b, Side s) const
{
    if (b.beansInPlay(s) == 0)
        return -1;
    for (int n_hole = 1; n_hole <= b.holes(); n_hole++)
    {
        if (b.beans(s, n_hole) > 0)
            return n_hole;
    }
    return -1;
}


//---SmartPlayer Methods---//
int SmartPlayer::chooseMove(const Board &b, Side s) const
{
    Timer timer;
    int depth = 0;
    if (b.beansInPlay(s) == 0)
        return -1;
    int move = 0;
    int newmove = 0;
    int bestmove = POOR - 20;
    for (int n_hole = 1; n_hole <= b.holes(); n_hole++)
    {
        if (b.beans(s, n_hole) > 0)
        {
            Board test_b(b);
            bool max;
            Side endSide;
            int endHole;
            bool check = false;
            if (test_b.sow(s, n_hole,endSide, endHole)  || (endHole == 0 && endSide == s))
            {
                check = true;
            }
            else if (test_b.beans(endSide, endHole) == 1 && test_b.beans(opponent(endSide), endHole) > 0 && s == endSide)
            {
                test_b.moveToPot(s, endHole, s);
                test_b.moveToPot(opponent(s), endHole, s);
                check = true;
            }
            if (check)
            {
                max = true;
                newmove = minimax(test_b, s, depth, max, timer);
            }
            else
            {
                max = false;
                newmove = minimax(test_b, s, depth, max, timer);
            }
            if (newmove >= bestmove)
            {
                bestmove = newmove;
                move = n_hole;
            }
        }
    }
    return move;
}

int SmartPlayer::evaluate(const Board& b, Side s) const
{

    if(b.beansInPlay(s) == 0 || b.beansInPlay(opponent(s)) == 0)
    {
        if (b.beans(s,0) + b.beansInPlay(s) < b.beans(opponent(s), 0) + b.beansInPlay(opponent(s)))
            return POOR;
        else if (b.beans(s,0) + b.beansInPlay(s) > b.beans(opponent(s), 0) + b.beansInPlay(opponent(s)))
            return GOOD;
        else
            return TIE;
    }
    int board_split = b.totalBeans()/2;
    if (b.beans(s, 0) < board_split)
        return POOR;
    else if (b.beans(s, 0) > board_split)
        return GOOD;
    
    int last_case = b.beans(s, 0) - b.beans(opponent(s), 0);
    return last_case;
}
int SmartPlayer::minimax(const Board &b, Side s, int depth, bool best, Timer& timer) const
{
    int n_hole;
    int bestHole;
    if(depth == 0 || b.beansInPlay(s) == 0 || b.beansInPlay(opponent(s)) == 0)
        return evaluate(b, s);
    if (timer.elapsed() > 5000)
        return evaluate(b, s);
    if (best)
    {
        bestHole = POOR - 10;
        for (n_hole = 1; n_hole <= b.holes(); n_hole++)
        {
            if (timer.elapsed() > 5000)
                return evaluate(b, s);
            if(b.beans(s,n_hole) > 0)
            {
                Board test_b(b);
                Side endSide;
                bool opp_best;
                int new_max;
                int endHole;
                if (timer.elapsed() > 5000)
                    return evaluate(b, s);
                if (test_b.sow(s, n_hole,endSide, endHole)  || (endHole == 0 && endSide == s))
                {
                    opp_best = true;
                    new_max = minimax(test_b, s, depth-1, opp_best, timer);
                }
                else
                {
                    opp_best = false;
                    new_max = minimax(test_b, s, depth-1, opp_best, timer);
                }
                if (test_b.beans(endSide, endHole) == 1 && test_b.beans(opponent(endSide), endHole) > 0 && s == endSide)
                {
                    test_b.moveToPot(s, endHole, s);
                    test_b.moveToPot(opponent(s), endHole, s);
                    opp_best = true;
                    new_max = minimax(test_b, s, depth-1, opp_best, timer);
                }
                else
                {
                    test_b.moveToPot(s, endHole, s);
                    test_b.moveToPot(opponent(s), endHole, s);
                    opp_best = false;
                    new_max = minimax(test_b, s, depth-1, opp_best, timer);
                }
                if (new_max >= bestHole)
                {
                    bestHole = new_max;
                }
            }
        }
    }
    else
    {
        bestHole = GOOD + 10;
        for(n_hole = 1; n_hole <= b.holes(); n_hole++)
        {
            if (timer.elapsed() > 5000)
                return evaluate(b, s);
        }
        if (b.beans(s, n_hole) > 0)
        {
            Board test_b(b);
            Side endSide;
            bool opp_best;
            int new_min;
            int endHole;
            if (timer.elapsed() > 5000)
                return evaluate(b, s);
            if (test_b.sow(s, n_hole,endSide, endHole)  || (endHole == 0 && endSide == s))
            {
                opp_best = false;
                new_min = minimax(test_b, opponent(s), depth-1, opp_best, timer);
            }
            else
            {
                opp_best = true;
                new_min = minimax(test_b, opponent(s), depth-1, opp_best, timer);
            }
            if (test_b.beans(endSide, endHole) == 1 && test_b.beans(opponent(endSide), endHole) > 0 && s == endSide)
            {
                test_b.moveToPot(s, endHole, s);
                test_b.moveToPot(opponent(s), endHole, s);
                opp_best = false;
                new_min = minimax(test_b, s, depth-1, opp_best, timer);
            }
            else
            {
                test_b.moveToPot(s, endHole, s);
                test_b.moveToPot(opponent(s), endHole, s);
                opp_best = true;
                new_min = minimax(test_b, opponent(s), depth-1, opp_best, timer);
            }
            if (new_min >= bestHole)
            {
                bestHole = new_min;
            }
            
        }
    }
    return bestHole;
}
    

    
                    


