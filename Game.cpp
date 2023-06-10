//
//  Game.cpp
//  CS32 P3
//
//  Created by Arash Dewan on 5/29/23.
//

#include "Game.h"
#include <iostream>
using namespace std;

Game::Game(const Board& b, Player* south, Player* north) : m_board(b), m_south(south), m_north(north), m_move(SOUTH) {}
    //Construct a Game to be played with the indicated players on a copy of the board b. The player on the south side always moves first.
    
void Game::display() const
{
    //Display the game's board in a manner of your choosing, provided you show the names of the players and a reasonable representation of the state of the board.
    
    //north name
    for (int i = 0; i < m_board.holes(); i ++)
    {
        cout << "  ";
    }
    cout << m_north->name() << endl;
    //north holes
    cout << "    ";
    for (int i = 1; i <= m_board.holes(); i++)
    {
        cout << "[" << m_board.beans(NORTH, i) << "]" << " ";
    }
    //north pot
    cout << endl << "[" << m_board.beans(NORTH, POT) << "]";
    //spaces between pot
    for (int i = 1; i <= m_board.holes(); i++)
    {
        cout << "    ";
    }
    //south pot
    cout << " [" << m_board.beans(SOUTH, POT) << "]" << endl;
    //south holes
    cout << "    ";
    for (int i = 1; i <= m_board.holes(); i++)
    {
        cout << "[" << m_board.beans(SOUTH, i) << "]" << " ";
    }
    //south name
    cout << endl;
    for (int i = 0; i < m_board.holes(); i ++)
    {
        cout << "  ";
    }
    cout << m_south->name() << endl;

}
void Game::status(bool& over, bool& hasWinner, Side& winner) const
{
    //If the game is over (i.e., the move member function has been called and returned false), set over to true; otherwise, set over to false and do not change anything else. If the game is over, set hasWinner to true if the game has a winner, or false if it resulted in a tie. If hasWinner is set to false, leave winner unchanged; otherwise, set it to the winning side.
    if (m_board.beansInPlay(NORTH) > 0 && m_board.beansInPlay(SOUTH) > 0)
    {
        over = false;
        return;
    }
    else
        over = true;
    
    if (m_board.beans(NORTH, 0) + m_board.beansInPlay(NORTH) == m_board.beans(SOUTH, 0) + m_board.beansInPlay(SOUTH))
    {
        hasWinner = false;
    }
    else
    {
        if (m_board.beans(NORTH, 0) + m_board.beansInPlay(NORTH) > m_board.beans(SOUTH, 0) + m_board.beansInPlay(SOUTH))
        {
            hasWinner = true;
            winner = NORTH;
        }
        else
        {
            hasWinner = true;
            winner = SOUTH;
        }
    }
}
bool Game::move(Side s)
{
    //Attempt to make a complete move for the player playing side s. "Complete" means that the player sows the seeds from a hole and takes any additional turns required or completes a capture. Whenever the player gets an additional turn, you should display the board so someone looking at the screen can follow what's happening. If the move can be completed, return true; if not, because the move is not yet completed but side s has no holes with beans to pick up and sow, sweep any beans in s's opponent's holes into that opponent's pot and return false.
    bool n_over, n_haswinner;
    Side n_winner;
    status (n_over, n_haswinner, n_winner);
    Side n_endSide;
    int n_endHole;
    if (n_over)
    {
        for (int i = 1; i <= m_board.holes(); i++)
        {
            m_board.moveToPot(NORTH, i, NORTH);
            m_board.moveToPot(SOUTH, i, SOUTH);
        }
        return false;
    }
    
    while (true)
    {
        int check = 0;
        if (s == NORTH)
        {
            check = m_north->chooseMove(m_board, NORTH);
            if (!m_north->isInteractive())
                cout << m_north->name() + " chooses hole " + to_string(check) << endl;
        }
        else
        {
            check = m_south->chooseMove(m_board, SOUTH);
            if (!m_south->isInteractive())
                cout << m_south->name() + " chooses hole " + to_string(check) << endl;
        }
        if (check == -1)
            break;
        if (!m_board.sow(s, check, n_endSide, n_endHole))
            break;
        if (n_endSide != s)
            break;
        else
        {
            if(n_endHole == 0)
            {
                status(n_over, n_haswinner, n_winner);
                if (n_over)
                    break;
                display();
                if (s == NORTH)
                {
                    cout << m_north->name() + " gets another turn." << endl;
                    return move(NORTH);
                }
                else
                {
                    cout << m_south->name() + " gets another turn." << endl;
                    return move(SOUTH);
                }

            }
            if (m_board.beans(n_endSide, n_endHole) == 1 && n_endSide == s && m_board.beans(opponent(n_endSide), n_endHole) > 0)
            {
                m_board.moveToPot(s, n_endHole, s);
                m_board.moveToPot(opponent(s), n_endHole, s);
                if (s == NORTH)
                    cout << m_north->name() + " committed a capture." << endl;
                else
                    cout << m_south->name() + " committed a capture." << endl;
            }
            break;

            
        }
    }
    status(n_over, n_haswinner, n_winner);
    if (n_over)
    {
        if (m_board.beansInPlay(NORTH) > 0 || m_board.beansInPlay(SOUTH) > 0)
        {
            display();
            if (m_board.beansInPlay(NORTH) > 0)
                cout << "Sweeping remaining beans into " + m_north->name()  + "\'s pot" << endl;
            else
                cout << m_north->name() + " has no beans left to sow." << endl;
            if (m_board.beansInPlay(SOUTH) > 0)
                cout << "Sweeping remaining beans into " + m_south->name()  + "\'s pot" << endl;
            else
                cout << m_south->name() + " has no beans left to sow." << endl;
            for (int i = 1; i <= m_board.holes(); i++)
            {
                m_board.moveToPot(NORTH, i, NORTH);
                m_board.moveToPot(SOUTH, i, SOUTH);
            }
            
        }
    }
    m_move = opponent(m_move);
    return true;
}
void Game::play()
{
    //Play the game. Display the progress of the game in a manner of your choosing, provided that someone looking at the screen can follow what's happening. If neither player is interactive, then to keep the display from quickly scrolling through the whole game, it would be reasonable periodically to prompt the viewer to press ENTER to continue and not proceed until ENTER is pressed. (The ignore function for input streams is useful here.) Announce the winner at the end of the game. You can apportion to your liking the responsibility for displaying the board between this function and the move function.
    bool n_over, n_haswinner;
    Side n_winner;
    display();
    do {
        move(m_move);
        display();
        if (!m_north->isInteractive() && !m_south->isInteractive())
        {
            cout << "press ENTER to continue";
            cin.ignore();
        }
        status(n_over, n_haswinner, n_winner);
    } while (!n_over);
    if (n_haswinner)
    {
        if (n_winner == NORTH)
            cout << "The winner is " + m_north->name() << endl;
        else
            cout << "The winner is " + m_south->name() << endl;
    }
    else
        cout << "The game resulted in a tie." << endl;
    
}
int Game::beans(Side s, int hole) const
{
    //Return the number of beans in the indicated hole or pot of the game's board, or −1 if the hole number is invalid. This function exists so that we and you can more easily test your program.
    return m_board.beans(s, hole);
}
