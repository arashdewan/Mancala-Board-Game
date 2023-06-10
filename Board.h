//
//  Board.hpp
//  CS32 P3
//
//  Created by Arash Dewan on 5/29/23.
//

#ifndef Board_hpp
#define Board_hpp

#include "Side.h"
#include <vector>

class Board
{
public:
    Board (int nHoles, int nInitialBeansPerHole);
    int holes() const;
    int beans(Side s, int hole) const;
    int beansInPlay(Side s) const;
    int totalBeans() const;
    bool sow(Side s, int hole, Side& endSide, int& endHole);
    bool moveToPot(Side s, int hole, Side potOwner);
    bool setBeans(Side s, int hole, int beans);
private:
    
    int m_holes;
    int m_initialbeansperhole;
    std::vector <int> m_south;
    std::vector <int> m_north;
};

#endif /* Board_hpp */
