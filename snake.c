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
void destroySnake(snake *);
void input(snake * ,int *);
void addSegment(snake *);
void output(snake * , fruit *);
void logic(snake * , fruit *, int *);
void updateSnake(snake *);

snake *initSnake(){
     // allocating a default snake head
     snake_segment *head = (snake_segment*)malloc(sizeof(snake_segment));
     // allocating a snake object
     snake *s = (snake*)malloc(sizeof(snake));
     //assigning the head coordinates
     head->x = (WIDTH-1)/2;
     head->y = (HEIGHT-1)/2;
     //initially the snake has no head so assigning s.head and s.tail by head
     s->head = s->tail = head;
     //assigning a default speed
     s->speed = 1;
     return s;
}

void addSegment(snake *snake1){
       //allocating a new segment
       snake_segment *newSegment = (snake_segment*)malloc(sizeof(snake_segment));
       //adding a new segment to its tail
       snake1->tail->next = newSegment;
       //updating the tail pointer
       snake1->tail = snake1->tail->next;
       updateSnake(snake1);
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
               for(snake_segment *k = snake1->head; k != NULL; k = k->next){
                    if(k == snake1->head && (k->x == j && k->y == i)){
                           printf("O");
                           printed = 1;
                    }
                    else if(k->x == j && k->y == i){
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
      f1.x = snake1->head->x;
      f1.y = snake1->head->y-5;
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
