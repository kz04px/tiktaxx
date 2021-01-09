### About
Tiktaxx is a C++ engine for the board game Ataxx. It uses traditional alphabeta methods such as null move pruning, killer moves, and a transposition table.

---
### About the game
Ataxx is a two player, turn-based, perfect information board game created in 1988.<br/>
[Wikipedia article](https://en.wikipedia.org/wiki/Ataxx)

---
### Build instructions
```bash
git clone --recurse-submodules https://github.com/kz04px/tiktaxx
follow the libataxx build instructions
cd ./tiktaxx
make
```

---
### UAI protocol
UAI stands for "Universal Ataxx Interface" and is a slightly modified version of the Universal Chess Interface protocol.
The only differences are that instances of 'c' for 'chess' have been replaced by 'a' for 'ataxx', and that the [FEN](https://en.wikipedia.org/wiki/Forsyth–Edwards_Notation) format has been altered to suit the game.

---
### FEN strings
"x" are player one's pieces. "o" are player two's pieces. Numbers represent the number of empty squares. Dashes represent blocked squares. The final character shows the current side to play.<br/>
Examples:
>x5o/7/7/7/7/7/o5x x 0 1<br/>
>x5o/7/2-1-2/7/2-1-2/7/o5x x 0 1

---
### Play without a GUI
Tiktaxx supports the 'manual' mode that should be more suitable for entering commands manually than the UAI protocol.
A board will be printed in text and moves can be entered with chess-like algebraic notation e.g. g1g3, b6, etc

---
### Ataxx tools
Some potentially useful python scripts for Ataxx engine development, among other things, can be found here:<br/>
[Ataxx tools](https://github.com/kz04px/ataxx-tools)

---
### Example UAI usage
```
>uai
<id name Tiktaxx
<id author kz04px
<uaiok
>isready
<readyok
>uainewgame
>position startpos
>go depth 5
<info depth 1 nodes 17 score cp -140 time 0 pv f1
<info depth 2 nodes 51 score cp 236 time 0 pv f1
<info depth 3 nodes 741 score cp -102 time 0 pv f1 b1
<info depth 4 nodes 1666 score cp 178 time 0 pv f1 a2 e1
<info depth 5 nodes 12926 score cp -63 time 5 pv f1 a2 e1 b1
<bestmove f1
>quit
```
