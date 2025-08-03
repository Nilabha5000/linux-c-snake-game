#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
/* reads from keypress, doesn't echo */
int getch(void) {
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr); // Get current terminal attributes
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echoing
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr); // Apply new attributes
    ch = getchar(); // Read a character
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr); // Restore old attributes
    return ch;
}

#define WIDTH 50
#define HEIGHT 50

#define MAXLEN 50

#define LEFT 'a'
#define RIGHT 'd'
#define UP 'w'
#define DOWN 's'
typedef struct {
     int x;
     int y;
     int direction;
}snake_segment;

typedef struct{
     snake_segment *body;
     int bodylength;
     int speed;
}snake;
typedef struct{
    int x;
    int y;
}fruit;

int score = 0;
snake *initSnake();
void destroySnake(snake *);
void input(snake * ,int *);
void output(snake * , fruit *);
void logic(snake * , fruit *, int *);
void updateSnake(snake *);

snake *initSnake(){
     snake_segment *sb = (snake_segment*)malloc(sizeof(snake_segment)*MAXLEN);
     snake *s = (snake*)malloc(sizeof(snake));
     s->body = sb;
     s->bodylength = 0;
     s->body[0].x = (WIDTH-1)/2;
     s->body[0].y = (HEIGHT-1)/2;
     s->speed = 1;
     return s;
}


void destroySnake(snake * s){
      free(s->body);
      free(s);
}
void input(snake *snake1 , int *gameOver){
    
    int c = (char)getch();
    switch(c){
      case LEFT:
         snake1->body[0].direction = LEFT;
         break ;
      case RIGHT:
         snake1->body[0].direction = RIGHT;
         break;
      case UP:
         snake1->body[0].direction = UP;
         break;
     case DOWN:
         snake1->body[0].direction = DOWN;
         break;
     case 'q':
       *gameOver = 1;
        break;
       
   }
}
void logic(snake *snake1, fruit *f ,int *gameOver){
      if(snake1->body[0].x == 0 || snake1->body[0].y == HEIGHT-1 || snake1->body[0].x == WIDTH-1 || snake1->body[0].y == 0)
      {
          *gameOver = 1;
           return;
      }
    // collision logic
    if(snake1->body[0].x == f->x && snake1->body[0].y == f->y)
    {
        snake1->bodylength++;
       f->x = rand()%(WIDTH-1);
       f->y = rand()%(HEIGHT-1);
       
       score++;
       return;
    }
    // logic for self collition
      for(int i = 1; i < snake1->bodylength; ++i){
           if(snake1->body[0].x == snake1->body[i].x && snake1->body[0].y == snake1->body[i].y){
                 *gameOver = 1;
                 printf("self collied \n");
                getch(); // Wait for user input before exiting
                 return;
           }
      }
} 
void updateSnake(snake *snake1){
  
    // Store previous positions
    int prevX = snake1->body[0].x;
    int prevY = snake1->body[0].y;
    int prevDir = snake1->body[0].direction;

    // Move head
    switch (snake1->body[0].direction) {
        case LEFT:
            snake1->body[0].x -= 1;
            break;
        case RIGHT:
            snake1->body[0].x += 1;
            break;
        case UP:
            snake1->body[0].y -= 1;
            break;
        case DOWN:
            snake1->body[0].y += 1;
            break;
    }
    
    // Move the rest of the body
    for (int i = 1; i <= snake1->bodylength; ++i) {
        int tempX = snake1->body[i].x;
        int tempY = snake1->body[i].y;
        int tempDir = snake1->body[i].direction;

        snake1->body[i].x = prevX;
        snake1->body[i].y = prevY;
        snake1->body[i].direction = prevDir;

        prevX = tempX;
        prevY = tempY;
        prevDir = tempDir;
    }
} 
void output(snake *snake1 ,fruit *f){
     
     printf("Score : %d\n",score);


    for(int i = 0; i < HEIGHT; ++i){

       for(int j = 0; j < WIDTH; ++j){
           int printed = 0;
           if(i == 0 || j == 0 || i == HEIGHT-1 || j == WIDTH-1){
                 printf("+");
                 continue;
           }
            
           else if(i == f->y && j == f->x){
              printf("A");
              continue;
           }  
          
	     else{
               for(int k = 0; k <= snake1->bodylength; ++k){
                    if(k == 0 && (snake1->body[k].x == j && snake1->body[k].y == i)){
                           printf("O");
                           printed = 1;
                    }
                    else if(snake1->body[k].x == j && snake1->body[k].y == i){
                            printf("o");
                            printed = 1;
                    }
               }
              if(!printed)
              printf(" ");  
           }   
       }
        printf("\n");
    }

}
int main(){

      int gameOver = 0;
      snake *snake1 = initSnake();
      fruit f1;
      f1.x = snake1->body[0].x;
      f1.y = snake1->body[0].y-5;
      system("clear");
      while(!gameOver){
        
         logic(snake1, &f1, &gameOver);
         output(snake1, &f1);
         input(snake1, &gameOver);
         updateSnake(snake1);
         system("clear");
      }
     destroySnake(snake1);
     printf("Game Over \n\n");
     return 0;
}
