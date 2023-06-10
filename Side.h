//
//  Side.h
//  CS32 P3
//
//  Created by Arash Dewan on 5/29/23.
//

#ifndef Side_h
#define Side_h

enum Side { NORTH, SOUTH };

const int NSIDES = 2;
const int POT = 0;

//smartplayer const helpers
const int LOSS = -1000000000;
const int POOR = -10000;
const int TIE = 0;
const int GOOD = 10000;
const int WIN = 1000000000;

inline
Side opponent(Side s)
{
    return Side(NSIDES - 1 - s);
}

#endif /* Side_h */
