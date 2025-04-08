# BrickGames 

## Introduction

To implement the BrickGames, the project must consist of four parts: a library's that implements the logic of the games: tetris and snake, a terminal interface developed using the `ncurses` library and desktop interface developed using gtk+. The logic of the library's must be implemented using finite state machines, one of the possible descriptions of which will be given below.

## General information
### BrickGame

BrickGame is a popular portable console from the 90s with several ~~thousands~~ of built-in games, developed in China. It was originally a copy of the Tetris game developed in the USSR and released by Nintendo on the GameBoy platform, but it also included many other games that were added over time. The console had a small screen with a 10x20 playing field, which was a matrix of “pixels”. To the right of the field there was a scoreboard with a digital indication of the status of the current game, records and other additional information. The most common games on BrickGame were: Tetris, Tanks, Racing, Frogger and Snake.

## Install instructions:

Open a terminal.  
Type make install  
The “Brick_Games” directory will be created in the user root.
Go to this directory.  
To start:  
./tetris_cli - terminal interface tetris  
./tetris_desktop - desktop interface tetris  
./snake_cli - terminal interface snake  
./snake_desktop - desktop interface snake  

## Uninstall:

Open a terminal.  
Type make uninstall


## To control use the following keys:

TETRIS:

Enter - Start game  
P - Pause  
Esc - End game  
Left arrow - movement of the figure to the left  
Right arrow - movement of the figure to the right  
Down arrow - falling figure  
Space - rotation of the figure
  
SNAKE:

Enter - Start game  
P - Pause  
Esc - End game   
Up arrow - change direction  
Left arrow - change direction  
Right arrow - change direction   
Down arrow - change direction  
Space - move faster
