#include <ncurses.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#define SPAWN_CHANCE 3 //1 in 3
#define DELAY 100 //in ms

typedef struct Fish{
        int length;
        int yPosition;
        int xPosition;
        int speed;
        struct Fish *next;

    }Fish;

Fish *create_fish(int length, int yPos, int speed, Fish *next);
void append_fish(Fish **head, Fish *new_fish);
void move_fish(Fish **head);
void cleanup(Fish **head);

int main(){
    initscr();
    srand(time(NULL));
    noecho();
    timeout(0);
    cbreak();
    keypad(stdscr, 1);
    refresh();
    getch();
    int xcounter = 0;
    Fish *head = create_fish((rand() % 9 + 1), (rand() % LINES), (rand() % 3 + 1), NULL);
    while(1){
        bool newFish = (rand() % SPAWN_CHANCE) == 1;
        if (newFish){
            Fish *nextFish =create_fish((rand() % 15 + 1), (rand() % LINES), (rand() % 3 + 1), NULL);
            append_fish(&head, nextFish);
        }
        move_fish(&head);
        napms(DELAY);
        if(getch() == 'q'){
            cleanup(&head);
            endwin();
            return 0;
        }
    }
}
Fish *create_fish(int length, int yPos, int speed, Fish *next){
    Fish *new_fish = malloc(sizeof(Fish));
    if (!new_fish) return NULL;
    new_fish->length = length;
    new_fish->yPosition = yPos;
    new_fish->xPosition = 0;
    new_fish->speed = speed;
    new_fish->next = next;

    return new_fish;
}
void append_fish(Fish **head, Fish *new_fish){
    if (*head == NULL){
        *head = new_fish;
    }
    else{
        Fish *curr = *head;
        while(curr->next != NULL){
            curr = curr->next;
        }
        curr->next = new_fish;
    }
    new_fish->next = NULL;
}
   
void move_fish(Fish **head){
        Fish *curr = *head;
        Fish *prev = NULL;
        while(curr != NULL){   
            Fish *next = curr->next;

            mvaddch(curr->yPosition, curr->xPosition+1+curr->length,'D');
            mvaddch(curr->yPosition, curr->xPosition-1,  '>');
            for(int i=0; i<curr->length; i++){
                mvaddch(curr->yPosition, curr->xPosition+i, i%2==0 ? '-' : '+' );
            }
            curr->xPosition+=curr->speed;

            if(curr->xPosition>=COLS+1){
                if(prev == NULL){
                    *head = curr->next;
                }
                else{
                    prev->next = curr->next;
                }
                free(curr);
            }
            else{
                prev = curr;
            }
            curr = next;

   
        }
        refresh();
        erase();
}
void cleanup(Fish **head){
    Fish *curr = *head;
    while(curr != NULL){
        Fish *next = curr->next;
        free(curr);
        curr = next;
    }
}