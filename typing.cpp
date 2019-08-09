//Created By Adam Bozzo
//Typing game where you type characters on screen to try and get a high score
#include <curses.h>
#include <ctime>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
using namespace std;

//returns random capital or lower case letter and makes sure
//no repeats of same letter
int randLetter(char letters[10]){
  //generates random integer to determine if capital or lower case
  int capOrLow = rand()%2;
  char randomChar;
  if(capOrLow == 1){
    randomChar = 'A' + rand()%26;
  }else{
    randomChar = 'a' + rand()%26;
  }
  //loop to make sure letters don't repeat
  for(int c = 0; c < 10; c++){
    if(letters[c] == randomChar){
      if(capOrLow == 1){
        randomChar = 'A' + rand()%26;
      }else{
        randomChar = 'a' + rand()%26;
      }
      c = 0;
    }
  }
  return randomChar;
}

//returns random x coordinate making sure 2 cant be the same
int randCoord(int letterX[10], int maxX){
  int randomX = rand()%(maxX-2);
  //loop to make sure new random x coordinate doesn't overlap others
  for(int c = 0; c < 10; c++){
      if(letterX[c] == randomX){
        randomX = rand()%(maxX-2);
        c = 0;
      }
  }
  return randomX;
}

int main(){
  initscr();//initializes the screen
  cbreak();//allows exit commands like ctrl z
  noecho();//prevents user input to show up on screen
  curs_set(0);//hides the cursor on screen
  nodelay(stdscr, TRUE);//prevents delay when using getch() in loop
  int maxY;
  int maxX;
  int score = 0;
  int missed = 0;
  int initializeX = 1;//variable for initial x coordinates of letters
  srand(time(0));//sets up the seed for random
  char letters[10];
  int letterX[10];
  int letterY[10];
  getmaxyx(stdscr, maxY, maxX);//get the max y and x values for the screen
  for(int c = 0; c < 10; c++){
    letters[c] = randLetter(letters);
  }
  //initialize the coordinates for the letters
  for(int c = 0; c < 10; c++){
    letterY[c] = 0;
    letterX[c] = initializeX;
    initializeX = initializeX + maxX/10;
  }
  int input;
  do{
    input = getch();//gets user input
    mvprintw(maxY-1,0, "Score is %d    Press ESC to exit.", score);
    for(int c = 0; c < 10; c++){
      mvprintw(letterY[c]-1,letterX[c], " ");
      mvprintw(letterY[c],letterX[c], "%c", letters[c]);
      letterY[c] = letterY[c] + 1;
      //if statement if the letter gets to bottom of screen
      if(letterY[c] >= maxY-3){
        //erases letter if it moves down too far
        mvprintw(letterY[c],letterX[c], " ");
        mvprintw(letterY[c]-1,letterX[c], " ");
        letterY[c] = 0;
        letters[c] = randLetter(letters);
        letterX[c] = randCoord(letterX, maxX);
        missed++;//increments amount missed
      }
      refresh();
    }
    for(int c = 0; c < 10; c++){
      //if statement if you click correct letter
      if(input == letters[c]){
        //lines that erase letter if chosen correctly
        mvprintw(letterY[c],letterX[c], " ");
        mvprintw(letterY[c]-1,letterX[c], " ");
        score++;//increments score
        letters[c] = randLetter(letters);//gets new random letter
        letterY[c] = 0;//resets y coordinate
        letterX[c] = randCoord(letterX, maxX);//gets new random x coord
        refresh();
      }
    }
    napms(200);//delay for program so it can be made easier or harder
    refresh();
  }while(input != 27);
  endwin();
  cout << "Total Score: " << score << endl;
  cout << "Total Missed: " << missed << endl;
  return 0;
}
