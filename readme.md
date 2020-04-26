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
>x5o/7/7/7/7/7/o5x x<br/>
>x5o/7/2-1-2/7/2-1-2/7/o5x x

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
<info depth 1 nodes 15 score -135 time 0 pv f1
<info depth 2 nodes 45 score 261 time 0 pv f1
<info depth 3 nodes 565 score -147 time 0 pv g2 b1
<info depth 4 nodes 1213 score 273 time 0 pv g2 b1 f1 a2
<info depth 5 nodes 6787 score -53 time 1 pv f2 b1 e1 a2 e1c1
<bestmove f2
>quit
```
