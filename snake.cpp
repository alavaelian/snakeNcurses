#include <cctype>
#include <cstdlib>
#include <sys/ioctl.h>
#include <ncursesw/cursesw.h>
#include <ncursesw/curses.h>
#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>
#include <ncursesw/ncurses.h>
#include <ctime>
#include <time.h>
#include <errno.h>
#include <cmath>

#define N 20
#define M 40

int Field[N][M],x,y,gy,Head,Tail,Game,Frogs,var,dir,score,HighScore=0,speed=150;


FILE *f;
void snakeInitialization(){
  f=fopen("highscore.txt","r+");
    rewind(f);
  fscanf(f,"%d",&HighScore);
  fclose(f);
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      Field [i][j]=0;
    }
  }
  x=N/2;
  y=M/2;
  Head=5;
  Tail=1;
  gy=y;
  Game = 0;
  Frogs = 0;
  var = 0;
  dir = 'd';
  score = 0;
  speed = 150;
  for(int i=0;i<Head;i++)
  {
    gy++;
    Field[x][gy-Head]=i+1;
  }
}
int kbhit(void)    /* comment */
{
    int ch, r;

    // turn off getch() blocking and echo
    nodelay(stdscr, TRUE);
    noecho();

    // check for input
    ch = getch();
    if( ch == ERR)      // no input
            r = FALSE;
    else                // input
    {
            r = TRUE;
            ungetch(ch);
    }

    // restore block and echo
    nodelay(stdscr, FALSE);
    return(r);
}

int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}
int interpolateSpeed(int y) {
    // Ajusta esta función según tus necesidades
    // Aquí hay un ejemplo básico que disminuye la velocidad gradualmente a medida que la serpiente se mueve hacia arriba
return 150 + (y * 3); // Aumenta la velocidad linealmente con la posición vertical
}
void print()
{
  wchar_t RDC = 0x255D;//right down corner simbol
  wchar_t LDC = 0x255A;//left down corner unicode simbol
  wchar_t VW = 0x2551;//vertical wall simbol
  wchar_t HW = 0x2550;//Horizontal wall simbol
  wchar_t RUC = 0x2557;//right up corner simbol
  wchar_t LUC = 0x2554;//left up corner simbol
  wchar_t HS = 0x2593;
  wchar_t BS = 0x2591;
  //wchar_t Frg = 0x101EF;//frog
  wchar_t Frg = 0x1F425;
  // wprintf(L"unicode code  255A prints  %lc\n", LDC);
  // wprintf(L"unicode code 2551 prints %lc\n",VW);
  // wprintf(L"unicode code 2550 prints %lc\n",HW);
  // wprintf(L"unicode code 255D prints %lc\n",RDC);
  // wprintf(L"unicode code 2557 prints %lc\n",RUC);

  // printf ("asscii of 188 gives: %c\n\n",188);
  // printf("%c", 65);
  for (int i=0;i<=M+1;i++)
  {
    if(i==0)
    {
      printw("%lc",LUC);
    }
    else if (i==M+1){
      printw("%lc",RUC);
    }
    else{
      printw("%lc",HW);
    }
  }
  printw( "Current Score: %d | HighScore: %d",score,HighScore);
  printw("\n");
  for(int i=0;i<N;i++)
  {
    printw("%lc",VW);
    for (int j=0;j<M;j++)
    {
      if(Field[i][j]==0){
      printw(" ");
      } 
    
      if(Field[i][j]>0&&Field[i][j]!=Head) printw("%lc",BS);
      if(Field[i][j]==Head)
      {
		  int red=0;
		  if(Frogs==0){
			  attron(COLOR_PAIR(1));
			  printw("%lc",HS);
			  attroff(COLOR_PAIR(1));
			  red=1;
			  }
			  if(red==0)printw("%lc",HS);
	   }
	  
	   
       
      if(Field[i][j]==-1){
// Ajusta la posición de impresión antes de imprimir el carácter grande
          
        cchar_t wch;

         setcchar(&wch, L"\U000101EF", A_NORMAL, 0, NULL);
 add_wch(&wch);
         
        //printw("%lc",Frg);
   
       
        // this one was used when the unicode caracter uses more than one position in widej++;
      } 
     
      
      if(j==M-1) printw("%lc\n",VW);
    }
    
  }
  for (int i=0;i<=M+1;i++)
  {
    if(i==0)
    {
      printw("%lc",LDC);
    }
    else if (i==M+1){

      printw("%lc",RDC);
    }
    else{
      printw("%lc",HW);
    }
  }
}
void ResetScreenPosition(){
  move(0, 0);
}
void Random (){
  srand(time(0));
  int a = 1 + rand() % 18;
  int b = 1 + rand() % 38;
  if (Frogs == 0 && Field[a][b]==0) {
    Field[a][b] = -1;
    Frogs = 1;
    if(speed>10 && score!=0){
      speed = speed -2;
    }
  }
}
// long int getch_noblock(){
//   if(!kbhit())
//     return getch();
//   else
//     return -1;
// }
long int getch_noblock(){
  if(kbhit())
    return getch();
  else
    return -1;
}
void GameOver(){
  system("clear");
    if(score>HighScore){
    mvprintw(N/2, M/2, " New HighScore %d !!!! \n\n",score);
    refresh();
    getch();
    f=fopen("highscore.txt", "w");
    rewind(f);
    fprintf(f, "%d", score);
    fclose(f);
}
  system("clear");

   mvprintw(N/2, M/2, "GameOver\n \t \t    score:%d \n\n Press the letter 'p' to play again or the letter 'e' to exit",score);
  refresh();

    while(1){
        var = getch_noblock();
        if(var == 'p'){
            Game = 0;
            system("clear");
            snakeInitialization();
            clear();
            refresh();
            break;
        }else if(var == 'e'){
            Game = 1;
            break;
        }
    }
}

void movement(){
  var = getch_noblock();
  var = tolower(var);
  if (((var == 'd' || var == 'a') || (var == 'w' || var == 's')) &&( abs(dir-var)>5)){



    dir=var; 

  }
  if (dir == 'd'){
   speed = 150; 
    y++;
    if(y==M-1){
      y=0;
    }

    if (Field[x][y]!=0 && Field[x][y]!=-1) {
      GameOver();
    }
    if(Field[x][y]==-1){
      Frogs=0;
      score += 5;
      Tail -= 2;
    }
    Head++;
    Field[x][y]=Head;
  }
  if (dir == 'a'){
  speed = 150; 
    y--;
    if(y==0){
      y=M-1;
    }

    if (Field[x][y]!=0 && Field[x][y]!=-1) {
      GameOver();
    }

       if(Field[x][y] == -1 ){
      Frogs=0;
      score += 5;
      Tail -= 2;
    }
 
    Head++;
    Field[x][y]=Head;
  }
  if (dir == 'w'){
 speed = interpolateSpeed(y);
    x--;

    if(x==-1){
      x=N-1;
    }
    if (Field[x][y]!=0 && Field[x][y]!=-1) {
      GameOver();
    }

    
  if(Field[x][y] == -1 ){
      Frogs=0;
      score += 5;
      Tail -= 2;
   }
 
    Head++;
    Field[x][y]=Head;
  }
  if (dir == 's'){
 speed = interpolateSpeed(y);
    x++;
    if(x==N){
      x=0;
    }

   if (Field[x][y]!=0 && Field[x][y]!=-1) {
      GameOver();
    }

       if(Field[x][y] == -1){
      Frogs=0;
      score += 5;
      Tail -= 2;
   }
  
    Head++;
    Field[x][y]=Head;
  }
}
void TailRemove(){
  for(int i=0;i<N;i++){
    for (int j=0; j<M; j++) {
      if (Field[i][j]==Tail) {
        Field[i][j] = 0;
      }
    }
  }
  Tail++;
}
int main (int argc, char *argv[]) {
  setlocale(LC_CTYPE, "");
  snakeInitialization();  // moving cursor, x = 20, y = 10
  initscr();
  start_color(); 
  init_pair(1, COLOR_RED, COLOR_BLACK);
  curs_set(0);
  while (Game == 0) {
    print();
    ResetScreenPosition();
    Random();
    movement();
    TailRemove();
    msleep(speed);
    //msleep(speed + (dir == 'w' || dir == 's' ? (int)(speed * log((double)x + 1) / log(N + 1)) : 0));
//double decay = 0.8; // Valor entre 0 y 1, cuanto más cercano a 1, más gradual es el decaimiento
//int vertical_speed = (int)(speed * pow(decay, x / (double)N));
//msleep(speed + (dir == 'w' || dir == 's' ? vertical_speed : 0));

    //msleep(speed + (dir == 'w' || dir == 's' ? (int)(speed * 0.37) : 0));
    refresh();

  }
  endwin();
  return 0;
}
