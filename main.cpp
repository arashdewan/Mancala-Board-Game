//
//  main.cpp
//  CS32 P3
//
//  Created by Arash Dewan on 5/29/23.
//

#include <iostream>
#include "Player.h"
#include "Game.h"
#include "Board.h"

int main()
{
        HumanPlayer hp("Marge");
        BadPlayer bp("Homer");
        Board b(3, 2);
        Game g(b, &hp, &bp);
        g.play();

    return 0;
}
