#include <iostream>
#include <ctime>

#include "ataxx.hpp"
#include "search.hpp"
#include "perft.hpp"
#include "eval.hpp"
#include "zobrist.hpp"
#include "other.hpp"

int main()
{
    srand(time(0));
    zobristInit();

    Position pos;
    setBoard(pos, "test");
    //makemoves(pos, "f1 a2 g2 b3 e1 b3c1 d1 b3 d1b2 f7 b1 f6 d1 e6 g3 f6d6 b3d5 f6 e7 g6 f5 f7g5 f7 g5f3 d1f2 f5d7 d5c7 f5 f3g5 e6f4 f6g4 f6 f4e6 g6e4 g4g6 f3 f4 f2g4 f2 g4e2 e4g4 f1d2 f3d1 f3 f1 g3e4 g3 e4c6 d2e4 d6b7 a2b4 d7d5 b4b6 e4d2 c2 e6e4 d3 f5d7 e4d6 f3f5 e6 g5f3 g5 f2e4 f2 f5d4 f5 d5b5 d5 d3b3 d3 b2c4 b2 d5b4 d5 b5a3 a2 d3b5 d3 b4a6 c4a5 c6c4 a2b4 c6 c4a2 c4 a6a4"); //  c7a6 e7c7
    //makemoves(pos, "b7 b1 c6 c1 c6a5 b1d2 f1 d2f2 b7b5 c2 b5d7 g7e7 a5c6 e7c7");
    //makemoves(pos, "g1f3 a1a3 f4 a4");
    //pos.turn = !pos.turn;

    Hashtable tt;
    tableInit(&tt);
    create(&tt, 128);
    //printDetails(&tt);
    //std::cout << std::endl;

    print(pos);
    std::cout << std::endl;
    std::cout << std::endl;

/*
    int wins = 0;
    int losses = 0;
    int draws = 0;
    for(int n = 0; n < 10000; ++n)
    {
        int r = rollout(pos, 300);
             if(r == 1)  {wins++;}
        else if(r == -1) {losses++;}
        else             {draws++;}
    }

    std::cout << "Random poss:" << std::endl;
    std::cout << 100.0*(double)wins/(wins+losses+draws)
              << "%"
              << " ("
              << wins
              << " - "
              << losses
              << " - "
              << draws
              << ")"
              << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
*/

/*
    std::string moves = "";
    bool flip = false;
    while(1)
    {
        print(pos);
        std::cout << "Moves:" << moves << std::endl;
        std::cout << std::endl;

        if(flip)
        {
            //std::cout << "Search:" << std::endl;
            //search(pos, 6);
            //std::cout << std::endl;
        }
        else
        {
            std::cout << "MCTS UCT:" << std::endl;
            mctsUct(pos);
            std::cout << std::endl;
        }

        std::string input;
        std::cin >> input;
        makemove(pos, input);
        moves += " " + input;

        std::cout << std::endl;
        std::cout << std::endl;

        flip = !flip;
    }
*/

/*
    splitEval(pos);
    std::cout << std::endl;
*/

/*
    std::cout << "Movegen:" << std::endl;
    printMoves(pos);
    std::cout << std::endl;
*/

/*
    std::cout << "Split perft:" << std::endl;
    splitPerft(pos, 4);
    std::cout << std::endl;
    std::cout << std::endl;
*/

/*
    std::cout << "Split Search:" << std::endl;
    splitSearch(&tt, pos, 7);
    clear(&tt);
    std::cout << std::endl;
    std::cout << std::endl;
*/

/*
    std::cout << "MCTS Pure:" << std::endl;
    mctsPure(pos);
    std::cout << std::endl;
    std::cout << std::endl;
*/

/*
    std::cout << "MCTS UCT:" << std::endl;
    mctsUct(pos);
    std::cout << std::endl;
    std::cout << std::endl;
*/

    std::cout << "Alpha-beta Search:" << std::endl;
    search(&tt, pos, 8);
    clear(&tt);
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "Perft:" << std::endl;
    perft(pos, 6);
    std::cout << std::endl;
    std::cout << std::endl;

    getchar();
    return 0;
}
