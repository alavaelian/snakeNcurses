# snakeNcurses
snake game using ncurses and the c programing language implemented for GNU/linux operating systems

how to compile it ,
first you need to install the ncurses develoment library you can do that in debian based distros with :

sudo apt-get install libncurses5-dev libncursesw5-dev

and for compiling it you need use:

g++ snake.cpp -o snake -lncursesw

i linked the "w" version of ncurses becose it the one that have compability with the unicode characters that i used.

and for excecuting it just need to do an:

./snake 

into a terminal
gracias  a paddy en stack overflow, que me ayudo a solucionar un bug.
