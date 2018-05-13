# AR-BoardGames
Classic board games, now in augmented reality! Play Battleship, Connect4, and Tic-Tac-Toe against your friends across multiple devices.

(gif goes here)

This program allows users to play multiplayer games of Battleship, Connect4, and Tic-Tac-Toe where the game board is drawn onscreen as an augmented reality object. Camera localization is based on an Aruco marker within the camera's FOV, and is computed using OpenCV. Game objects are drawn in the appropriate locations and perspectives (based on camera localization) using OpenGL. Making game moves (for example, interpreting mouse clicks as the intention to fire a shot at space C4 in battleship) is also  All games are multiplayer: two users play against each other in real time on seperate devices. cURL is used by the program to communicate with a simple web API for each game. MySQL databases' were setup to store information about each game such as logistics (how many players have joined), game state (whose turn, has any player won), and the state of all game pieces (i.e. whether each spot in a Connect4 board has a red piece, blue piece, or no piece). PHP scripts are used as the middlemen between the databases and API endpoints. The program has a simple interface: it starts on a menu where the user can select which game to play, each game is played by clicking on the game boards to make moves, a banner displays the game status (whose turn, who won, etc), and a button is available to return to the main menu. This program has been tested and runs well on Windows, OS X, and Linux.

(pics go here)

Three modes of play are available: 
- dynamic - uses a webcam to track and draw the objects based on an Aruco marker in real time
- static video - if a backwards facing webcam or Aruco marker is not available, you can still see the program track and draw objects based on a provided video file
- static image - if you are simply interested in playing the games with no distractions, several static image files are provided which will show stationary game boards

## Installation

Compiling from source is automated with the provided CMake scripts. This project has three dependencies:
- [OpenCV](https://opencv.org/) (plus contrib modules)
- [cURL](https://curl.haxx.se/)
- [YS-Class Library](http://ysflight.in.coocan.jp/programming/ysclass/ysclass-e.html)

OpenCV and cURL are popular libraries, and there are many tutorials online for installing these for your OS of choice. YS-Class Library is a helpful collection of C++ development tools developed by Dr. Soji Yamakawa. For this project, it is mainly used as an interface to OpenGL and for manipulating STL objects on screen. As it is a niche library, for convenience it is explicity included (with permission) in the source code of this project and is seamlessly integrated with the provided CMake scripts. 

## Authors
- Carlneil Domkam
- Dimitri Nikitopoulos
- Rahul Raina
- Harrison Van Til

## Acknowledgements
- Dr. Soji Yamakawa, for the use of YS-Class Library and for providing support and advice throughout this project's development
