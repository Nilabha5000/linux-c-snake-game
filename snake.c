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

#define WIDTH 100
#define HEIGHT 50
#define LEFT 'a'
#define RIGHT 'd'
#define UP 'w'
#define DOWN 's'
typedef struct snake_segment{
     int x;
     int y;
     int direction;
     struct snake_segment *next;
}snake_segment;

typedef struct{
     snake_segment *head , *tail;
     int speed;
}snake;
typedef struct{
    int x;
    int y;
}fruit;

int score = 0;

snake *initSnake();
char **initScr();
void renderScr(char **);
void updateScr(char ** , snake * , fruit *);
void destroyScr(char**);
void destroySnake(snake *);
void input(snake * ,int *);
void addSegment(snake *);
void logic(snake * , fruit *, int *);
void updateSnake(snake *);
//this function is use to allocates the screen buffer in the memory
char **initScr(){
    char **scr = (char**)malloc(sizeof(char*)*HEIGHT);
    for(int i = 0; i < HEIGHT; ++i){
          scr[i] = (char*)malloc(sizeof(char)*WIDTH);
    }
    return scr;
}
//updates the screen buffer
void updateScr(char **scr , snake *s , fruit * f){
      snake_segment *temp = s->head;
      for(int i = 0; i < HEIGHT; ++i){
            for(int j = 0; j < WIDTH; ++j){
                if(i == 0 || j == WIDTH-1 || i == HEIGHT-1 || j == 0)
                  scr[i][j] = '+';
                else
                 scr[i][j] = ' ';
            }
      }
     scr[f->y][f->x] = 'F';
     int ishead = 1;
    while(temp != NULL){
         if(temp->x >= 0 && temp->x <= WIDTH-1 && temp->y >= 0 && temp->y <= HEIGHT-1)
              scr[temp->y][temp->x] = (ishead) ? 'O' : 'o';
           temp = temp->next;
           ishead = 0;
    }
}
// this function display the screen contents in the terminal
void renderScr(char **screen){
        printf("score : %d\n",score);
        for(int i = 0; i < HEIGHT; ++i){
           for(int j = 0; j < WIDTH; ++j){
                 printf("%c",screen[i][j]);
           }
           printf("\n");
      }
}
//this function deallocates the screen buffer in the memory
void destroyScr(char **scr){
     for(int i = 0; i < HEIGHT; ++i)
         free(scr[i]);
     free(scr);
}
snake *initSnake(){
     // allocating a default snake head
     snake_segment *head = (snake_segment*)malloc(sizeof(snake_segment));
     // allocating a snake object
     snake *s = (snake*)malloc(sizeof(snake));
     //assigning the head coordinates and default direction
     head->x = (WIDTH-1)/2;
     head->y = (HEIGHT-1)/2;
     head->direction = UP;
     head->next = NULL;
     //initially the snake has no head so assigning s.head and s.tail by head
     s->head = s->tail = head;
     //assigning a default speed
     s->speed = 1;
     return s;
}
//this function adds the new segments to the snake
void addSegment(snake *snake1){
       //allocating a new segment
       snake_segment *newSegment = (snake_segment*)malloc(sizeof(snake_segment));
       //assigning NULL to the next pointer
       newSegment->next = NULL;
       //setting cordinates to new segment
       newSegment->x = snake1->tail->x;
       newSegment->y = snake1->tail->y;
       //adding a new segment to its tail
       snake1->tail->next = newSegment;
       //updating the tail pointer
       snake1->tail = snake1->tail->next;
}
void destroySnake(snake * s){
      snake_segment *prev , *curr;
      prev = NULL; 
      curr = s->head;
     // deallocating all the nodes of the linked list
      while(curr != NULL){
           prev = curr;
           curr = curr->next;
           free(prev);
      }
      // deallocate the snake object
      free(s);
}
void input(snake *snake1 , int *gameOver){
    
    int direction = (char)getch();
      if(((direction == DOWN && snake1->head->direction == UP)) || 
         (direction == UP && snake1->head->direction == DOWN) || 
              (direction == LEFT && snake1->head->direction == RIGHT) ||
               (direction == RIGHT && snake1->head->direction == LEFT)){
        // Prevent the snake from moving in the opposite direction
        // This avoids immediate self-collision
        return;
    }
       
    switch(direction){
      case LEFT:
         snake1->head->direction = LEFT;
         break ;
      case RIGHT:
         snake1->head->direction  = RIGHT;
         break;
      case UP:
         snake1->head->direction  = UP;
         break;
     case DOWN:
         snake1->head->direction  = DOWN;
         break;
     case 'q':
       *gameOver = 1;
        break;
       
   }
}
void logic(snake *snake1, fruit *f ,int *gameOver){
      if(snake1->head->x == 0 || snake1->head->y == HEIGHT-1 || snake1->head->x == WIDTH-1 || snake1->head->y == 0)
      {
          *gameOver = 1;
           return;
      }
    // collision logic
    if(snake1->head->x == f->x && snake1->head->y == f->y)
    {
       addSegment(snake1);
       f->x = rand()%(WIDTH-1);
       f->y = rand()%(HEIGHT-1);
       
       score++;
       return;
    }
    // logic for self collition
      for(snake_segment *i = snake1->head->next; i != NULL; i = i->next){
           if(snake1->head->x == i->x && snake1->head->y == i->y){
                 *gameOver = 1;
                 printf("self collied \n");
                getch(); // Wait for user input before exiting
                 return;
           }
      }
} 
void updateSnake(snake *snake1){
  
    // Store previous positions
    int prevX = snake1->head->x;
    int prevY = snake1->head->y;
    int prevDir = snake1->head->direction;

    // Move head
    switch (snake1->head->direction) {
        case LEFT:
            snake1->head->x -= 1;
            break;
        case RIGHT:
            snake1->head->x += 1;
            break;
        case UP:
            snake1->head->y -= 1;
            break;
        case DOWN:
            snake1->head->y += 1;
            break;
    }
    
    // Move the rest of the body
    for (snake_segment *i = snake1->head->next; i != NULL; i = i->next) {
        int tempX = i->x;
        int tempY = i->y;
        int tempDir = i->direction;

        i->x = prevX;
        i->y = prevY;
        i->direction = prevDir;

        prevX = tempX;
        prevY = tempY;
        prevDir = tempDir;
    }
}
int main(){

      int gameOver = 0;
      snake *snake1 = initSnake();
      fruit f1;
      f1.x = snake1->head->x;
      f1.y = snake1->head->y-5;
      char **screen = initScr();
      system("clear");
      while(!gameOver){
         updateSnake(snake1);
         updateScr(screen , snake1 , &f1);
         logic(snake1, &f1, &gameOver);
         renderScr(screen);
         input(snake1, &gameOver);
         
         system("clear");
      }
     destroySnake(snake1);
     destroyScr(screen);
     printf("Game Over \n\n");
     return 0;
}
