//
//  Board.cpp
//  CS32 P3
//
//  Created by Arash Dewan on 5/29/23.
//

#include "Board.h"

Board::Board(int nHoles, int nInitialBeansPerHole)
{
    //Construct a Board with the indicated number of holes per side (not counting the pot) and initial number of beans per hole. If nHoles is not positive, act as if it were 1; if nInitialBeansPerHole is negative, act as if it were 0.
    if (nHoles <= 0)
        nHoles = 1;
    if (nInitialBeansPerHole < 0)
        nInitialBeansPerHole = 0;
    m_holes = nHoles;
    m_initialbeansperhole = nInitialBeansPerHole;
    m_north.push_back(POT);//sets the north pot when its empty
    m_south.push_back(POT); // sets the south pot when its empty
    for (int i = 0; i <= m_holes; i++)
    {
        //in each iteration, fills each hole with the given amount of beans
        m_north.push_back(m_initialbeansperhole);
        m_south.push_back(m_initialbeansperhole);
    }
}
int Board::holes() const
{
    //Return the number of holes on a side (not counting the pot).
    return m_holes;
}
int Board::beans(Side s, int hole) const
{
    //Return the number of beans in the indicated hole or pot, or −1 if the hole number is invalid.
    if (hole > m_holes || hole < 0)
        return -1;
    else
    {
        if (s == NORTH)
        {
            return m_north[hole];
        }
        if (s == SOUTH)
        {
            return m_south[hole];
        }
    }
    return -1;
}

int Board::beansInPlay(Side s) const
{
    //Return the total number of beans in all the holes on the indicated side, not counting the beans in the pot.
    
    int total = 0;
    if (s == NORTH)
    {
        for (int i = 1; i <= m_holes; i++)
        {
            total += m_north[i];
        }
    }
    if (s == SOUTH)
    {
        for (int i = 1; i <= m_holes; i++)
        {
            total += m_south[i];
        }
    }
    return total;

}
int Board::totalBeans() const
{
    //Return the total number of beans in the game, including any in the pots.
    int total = 0;
    for (int i = 0; i <= m_holes; i++)
    {
        total = total + m_north[i] + m_south[i];
    }
    return total;

}
bool Board::sow(Side s, int hole, Side& endSide, int& endHole)
{
    //If the hole indicated by (s,hole) is empty or invalid or a pot, this function returns false without changing anything. Otherwise, it will return true after sowing the beans: the beans are removed from hole (s,hole) and sown counterclockwise, including s's pot if encountered, but skipping s's opponent's pot. The function sets the parameters endSide and endHole to the side and hole where the last bean was placed. (This function does not make captures or multiple turns; different Kalah variants have different rules about these issues, so dealing with them should not be the responsibility of the Board class.)
    if (hole > m_holes || hole <= 0 || beans(s, hole) == 0)
        return false;
    int nbeans = 0;
    if (s == NORTH) //starting on north
    {
        nbeans = m_north[hole];
        m_north[hole] = 0;
        while (nbeans != 0)
        {
            for (int i = hole-1; nbeans != 0 && i >= 0; i--) //traversing through north
            {
                m_north[i]++;
                nbeans--;
                if (nbeans == 0)
                {
                    endSide = NORTH;
                    endHole = i;
                    return true;
                }
            }
            for (int i = 1; nbeans != 0 && i <= m_holes; i++) //traversing through south
            {
                m_south[i]++;
                nbeans--;
                if (nbeans == 0)
                {
                    endSide = SOUTH;
                    endHole = i;
                    return true;
                }
            }
        }
    }
    else //starting at south
    {
        nbeans = m_south[hole];
        m_south[hole] = 0;
        while (nbeans != 0)
        {
            for (int i = hole + 1; nbeans != 0 && i <= m_holes; i++) //traversing through south
            {
                m_south[i]++;
                nbeans--;
                if (nbeans == 0)
                {
                    endSide = SOUTH;
                    endHole = i;
                    return true;
                }
            }
            if (nbeans != 0) //adding to south pot separately due to the layout of game
            {
                m_south[0]++;
                nbeans--;
                if (nbeans == 0)
                {
                    endSide = SOUTH;
                    endHole = 0;
                    return true;
                }
            }
            for (int i = m_holes; nbeans != 0 && i >= 1; i--) //traversing through north
            {
                m_north[i]++;
                nbeans--;
                if (nbeans == 0)
                {
                    endSide = NORTH;
                    endHole = i;
                    return true;
                }
            }
        }
    }
    return true;

}
bool Board::moveToPot(Side s, int hole, Side potOwner)
{
    //If the indicated hole is invalid or a pot, return false without changing anything. Otherwise, move all the beans in hole (s,hole) into the pot belonging to potOwner and return true.
    if (hole > m_holes || hole <= 0)
        return false;
    if (s == NORTH)
    {
        if (potOwner == NORTH)
        {
            m_north[POT] += m_north[hole];
            m_north[hole] = 0;
        }
        else
        {
            m_south[POT] += m_north[hole];
            m_north[hole] = 0;
        }
    }
    else
    {
        if (potOwner == NORTH)
        {
            m_north[POT] += m_south[hole];
            m_south[hole] = 0;
        }
        else
        {
            m_south[POT] += m_south[hole];
            m_south[hole] = 0;
        }
    }
    return true;
}
bool Board::setBeans(Side s, int hole, int beans)
{
    //If the indicated hole is invalid or beans is negative, this function returns false without changing anything. Otherwise, it will return true after setting the number of beans in the indicated hole or pot to the value of the third parameter. (This could change what beansInPlay and totalBeans return if they are called later.) This function exists solely so that we and you can more easily test your program: None of your code that implements the member functions of any class is allowed to call this function directly or indirectly. (We'll show an example of its use below.)
    if (beans < 0 || hole > m_holes || hole <= 0)
        return false;
    if (s == NORTH)
        m_north[hole] = beans;
    else
        m_south[hole] = beans;
    return true;

}
