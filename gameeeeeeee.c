#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include <time.h>
#include <SDL_ttf.h>
#include <stdlib.h>



#define STATUS_STATE_LIVES 0
#define STATUS_STATE_GAME 1
#define STATUS_STATE_GAMEOVER 2

#define GRAVITY 0.04f //gravity constant
//the lesser the gravity constant, the slower will be the falling of the man
#define ACC 0.04f
typedef struct {
    float x, y;
    int lives;
    char* name;
    float dy;
    float dx;
    int onLedge;
    int isDead;
    int isTouch;
    int count;
    int freq[100];
    int county;
    int freqdead[100];
    int countdead;
}Man; //name of the structure is man

typedef struct {
    int x, y;

 }Reward;

typedef struct {
    int x, y;
}Star;

typedef struct {
    int x, y, w, h;

}Ledge;

typedef struct {
    int x, y;
}Book;

typedef struct {
    int x, y;
}Tree;

typedef struct {
    int x, y;
}Bush;

//typedef struct {
  //  int x, y;
//}Fire;


typedef struct {

    float scrollX;
    //creating players (man)
    Man man;

    //stars
    Star stars[100];

    //adding ledges
    Ledge ledges[100];

    Tree trees[100];

    Book books[100];

    Bush bushes[20];

    Reward rewards[100];

    //Fire fires[20];

     //for adding images
    SDL_Texture* star;

    //Images
    SDL_Texture* manframes[2];
    SDL_Texture* brick;

    SDL_Texture* label;//for text
    SDL_Texture* tree;
    SDL_Texture* book;
    SDL_Texture* bush;
    SDL_Texture* fire;
    SDL_Texture* booktouch;
    SDL_Texture* reward;

    TTF_Font* font;

    int time;
    int statusState;
    int deathCountDown;

    SDL_Renderer* renderer;
}Gamestate;


void init_status_lives(Gamestate* gameState)
{

    char str[100] = "";


    SDL_Colour white = { 255,255,255,255 };
    SDL_Surface* tmp = TTF_RenderText_Blended(gameState->font, "ATOM-MAN", white);
    gameState->label = SDL_CreateTextureFromSurface(gameState->renderer, tmp);
    SDL_FreeSurface(tmp);



}



void draw_status_lives(Gamestate* gameState)

{
    SDL_SetRenderDrawColor(gameState->renderer, 0, 0, 0, 255);
    SDL_RenderClear(gameState->renderer);

    //draw image:
   // SDL_Rect rect = { 320 - 40,240,48,48 };
    //SDL_RenderCopyEx(gameState->renderer, gameState->manframes[0], NULL, &rect, 0, NULL,
      //  1);


    SDL_SetRenderDrawColor(gameState->renderer, 255, 255, 255, 255);

    SDL_Rect textRect = { 200,100,100,100 };
    SDL_RenderCopy(gameState->renderer, gameState->label, NULL, &textRect);
}

void shutdown_status_lives(Gamestate* gameState)
{
    SDL_DestroyTexture(gameState->label);
    gameState->label = NULL;

}
//void collissionDetect(Gamestate* game);
void process(Gamestate* game) {
    //add time
    game->time++;
    if (game->time > 120) {
        shutdown_status_lives(game);
        game->statusState = STATUS_STATE_GAME;
    }

    if (game->statusState == STATUS_STATE_GAME) {
        // if (game->man.isDead == 0) {
        Man* man = &game->man;
        man->y += man->dy;
        man->dy += GRAVITY;// makes atoman travel downwards



        game->scrollX = -game->man.x + 320;
        // }
        if (game->man.isDead && game->deathCountDown < 0) {
            game->deathCountDown = 120;
        }
        if (game->deathCountDown > 0) {
            game->deathCountDown--;
            if (game->deathCountDown < 0) {
                game->man.lives--;
                if (game->man.lives >= 0) {
                    init_status_lives(game);
                    game->statusState = STATUS_STATE_LIVES;
                    game->time = 0;

                    //set it again
                    game->man.isDead = 0;
                    game->man.x = 100;
                    game->man.y = 240 - 40;
                    game->man.dx = 0;
                    game->man.dy = 0;
                    game->man.onLedge = 0;

                }

                else {
                    shutdown_status_lives(game);
                    game->statusState = STATUS_STATE_GAMEOVER;
                    game->time = 0;
                }
            }
        }
    }

}

int collide2d(float x1, float y1, float x2, float y2, float wt1, float ht1,
    float wt2, float ht2) {
    return (!((x1 > (x2 + wt2)) || (x2 > (x1 + wt1)) || (y1 > (y2 + ht2)) || (y2 > (y1 + ht1))));
}

void dorender2(SDL_Renderer* renderer, Gamestate* game) {
    SDL_Rect rect = { game->scrollX + game->man.x,game->man.y,10,10 };
    //SDL_RenderFillRect(renderer, &rect);
    SDL_RenderCopyEx(renderer, game->booktouch, NULL, &rect, 0, NULL, (game->time % 20 < 10));


}


void collissionDetect(Gamestate* game, SDL_Renderer* renderer, SDL_Window* window ) {

    //check the collission of atom-man and demon
    for (int i = 0; i < 100; i++) {
        if (game->man.freqdead[i] == 0)
        if (collide2d(game->man.x, game->man.y, game->stars[i].x, game->stars[i].y
            , 48, 48, 64, 64)) {
            game->man.isDead = 1;//if they collide (man and demon)
           // printf("Look around!");
            game->man.countdead = game->man.countdead + 1;
            printf("\n%d MORE LIVES LEFT", 3 - game->man.countdead);
            game->man.freqdead[i]++;

            if (game->man.countdead == 3) {
                printf("\nYou lost!");
               // delay(2);
                SDL_Quit();
                exit(1);
            }


        }
    }

    for (int i = 0; i < 100; i++) {
        if (game->man.freq[i] == 0) {
            if (collide2d(game->man.x, game->man.y, game->books[i].x, game->books[i].y,
                48, 48, 70, 70)) {
                //SDL_Window* window = SDL_CreateWindow("Getting Started", SDL_WINDOWPOS_UNDEFINED,
                  //  SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
                //SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
                //doRender2(renderer, &game);
                game->man.isTouch = 1;
                // printf("keep going buddy!");
                game->man.county = game->man.county + 1;
                //SDL_Rect rect = { game->scrollX + 200,200,10,10 };
                //SDL_RenderFillRect(renderer, &rect);
                //SDL_RenderCopyEx(renderer, game->booktouch, NULL, &rect, 0, NULL, (game->time % 20 < 10));
                printf("\nBOOK COUNT: %d", game->man.county);

                game->man.freq[i]++;

                if ((game->man.county % 3 == 0) && (game->man.county != 0)) {
                    int a;
                    int r;
                    if (game->man.county == 3) {
                        r = rand() % 3;
                        switch (r) {
                        case 0:
                                printf("\nHow many numbers are binary numbers made of ?");
                                printf("\n 1. 1 2.2 3.3 4.4");
                                printf("\n Please enter your response");

                                scanf_s("%d", &a);
                                if (a != 2) {
                                    printf("Sorry, try again");
                                    SDL_Quit();
                                    exit(1);

                                }
                                break;

                        case 1:
                                printf("\nWhat is the national game of India ?");
                                printf("\n 1. Hockey 2.Basketball ");
                                printf("\n 3. Cricket 4. Kabbaddi");
                                printf("\n Please enter your response");

                                scanf_s("%d", &a);
                                if (a != 1) {
                                    printf("Sorry, try again");
                                    SDL_Quit();
                                    exit(1);

                                }
                                break;

                        case 2:
                                printf("\nPortable computer is also called as ?");
                                printf("\n 1. Laptop 2.Computer ");
                                printf("\n 3. Adapter 4. Monitor");
                                printf("\n Please enter your response");

                                scanf_s("%d", &a);
                                if (a != 1) {
                                    printf("Sorry, try again");
                                    SDL_Quit();
                                    exit(1);

                                }
                                break;


                        }
                    }
                    if (game->man.county == 6) {
                        r = rand() % 2;
                        switch (r) {
                        case 0 :
                                printf("\nWho invented computer ?");
                                printf("\n 1. Charles Babbage 2.GrahamBell");
                                printf("\n 3. Marconi 4. Tesla");
                                printf("\n Please enter your response");

                                scanf_s("%d", &a);
                                if (a != 1) {
                                    printf("Sorry, try again");
                                    SDL_Quit();
                                    exit(1);

                                }
                                break;

                        case 1:
                                printf("\nThe display screen image that is used to provide visual output from a computer is?");
                                printf("\n 1. Monitor 2.CPU");
                                printf("\n 3. Mouse 4.  AC adapter");
                                printf("\n Please enter your response");

                                scanf_s("%d", &a);
                                if (a != 1) {
                                    printf("Sorry, try again");
                                    SDL_Quit();
                                    exit(1);

                                }
                                break;

                        }

                    }

                    if (game->man.county == 9) {
                        r = rand() % 2;
                        switch (r) {
                        case 0:
                            printf("\nWhat is the main circuit board of computer ?");
                            printf("\n 1. Mother board 2. Chipset");
                            printf("\n Please enter your response");

                            scanf_s("%d", &a);
                            if (a != 1) {
                                printf("Sorry, try again");
                                SDL_Quit();
                                exit(1);

                            }
                            break;
                        case 1:
                            printf("\nWhat is the national bird of India ?");
                            printf("\n 1. Penguin 2. Peacock");
                            printf("\n 3. Parrot 4. Sparrow");
                            printf("\n Please enter your response");

                            scanf_s("%d", &a);
                            if (a != 2) {
                                printf("Sorry, try again");
                                SDL_Quit();
                                exit(1);

                            }
                            break;


                        }
                    }



                    if (game->man.county == 9) {
                        printf("CONGRATS YOU WON!!");
                        SDL_Quit();
                        exit(1);
                    }
                }








            }
        }
    }

    //To ensure that the man does not go inside the brick
    for (int i = 0; i < 100; i++) {
        float mw = 49, mh = 48;//man's width and height
        float mx = game->man.x;
        float my = game->man.y;
        float bx = game->ledges[i].x;
        float by = game->ledges[i].y;
        float bw = game->ledges[i].w;
        float bh = game->ledges[i].h;

        if ((my + mh) > by && my < (by + bh)) {
            //if man touches the right edge:
            if (mx < (bx + bw) && (mx + mw) >(bx + bw)) {
                game->man.x = bx + bw;
                //bring the man to the correct position
                mx = bx + bw;

            }

            //check for left edge
            else if ((mx + mw) > bx && mx < bx) {
                //correct the coordinate x
                game->man.x = bx - mw;
                mx = bx - mw;
            }
        }

        if ((mx + mw) > bx && mx < (bx + bw)) {

            if (my < (by + bh) && my>by) {
                game->man.y = by + bh;
                //correct y
                mx = bx + bw;

                //if bumps the brick, make the velocity of man 0
                game->man.dy = 0;
                game->man.onLedge = 1;
            }

            //check if we are landing on the edge
            else if ((my + mh) > by && my < by) {
                //correct the coordinate x
                game->man.y = by - mh;
                //if landed, stop the jump velocity
                game->man.dy = 0;
                game->man.onLedge = 1;
            }
        }



    }
}

void Loadgame(Gamestate* game) {
    int j;
    //loading fonts:
    game->font = TTF_OpenFont("OpenSans-Bold.ttf", 50);
    if (!game->font) {
        printf("Font not available");
        SDL_Quit();
        exit(1);
    }
    SDL_Surface* starsurface = NULL;
    starsurface = IMG_Load("star.png");
    if (starsurface == NULL) {
        printf("could not load image");
        SDL_Quit();
        exit(1);
    }
    game->man.x = 320 - 80;
    game->man.y = 320 - 20;//the lesser the y cor, the more upwards he will be
    game->man.dy = 0;
    game->man.onLedge = 0;
    game->man.isDead = 0;
    game->time = 0;
    game->statusState = STATUS_STATE_LIVES;
    game->label = NULL;
    game->man.lives = 3;
    game->deathCountDown = -1; //since we are at the start of the game!
    game->man.isTouch = 0;
    game->man.count = 0;
    game->man.county = 0;
    game->man.countdead = 0;

    for (int i = 0; i < 100; i++) {
        game->man.freq[i] = 0;
    }

    for (int i = 0; i < 100; i++) {
        game->man.freqdead[i] = 0;
    }



    init_status_lives(game);

    game->star = SDL_CreateTextureFromSurface(game->renderer, starsurface);
    SDL_FreeSurface(starsurface);

    starsurface = IMG_Load("man_walk.png");
    if (starsurface == NULL) {
        printf("could not load image man_walk.png");
        SDL_Quit();
        exit(1);
    }

    game->manframes[0] = SDL_CreateTextureFromSurface(game->renderer, starsurface);
    SDL_FreeSurface(starsurface);

    starsurface = IMG_Load("man_ltb.png");
    if (starsurface == NULL) {
        printf("could not load image man_ltb.png");
        SDL_Quit();
        exit(1);
    }

    game->manframes[1] = SDL_CreateTextureFromSurface(game->renderer, starsurface);
    SDL_FreeSurface(starsurface);

    starsurface = IMG_Load("brick.png");
    game->brick = SDL_CreateTextureFromSurface(game->renderer, starsurface);
    SDL_FreeSurface(starsurface);

    game->scrollX = 0;

    starsurface = IMG_Load("tree.png");
    game->tree = SDL_CreateTextureFromSurface(game->renderer, starsurface);
    SDL_FreeSurface(starsurface);

    starsurface = IMG_Load("book.png");
    game->book = SDL_CreateTextureFromSurface(game->renderer, starsurface);
    SDL_FreeSurface(starsurface);

    starsurface = IMG_Load("bush.png");
    game->bush = SDL_CreateTextureFromSurface(game->renderer, starsurface);
    SDL_FreeSurface(starsurface);

    //if (game->man.isDead == 1)
    //{
    starsurface = IMG_Load("fire.png");
    game->fire = SDL_CreateTextureFromSurface(game->renderer, starsurface);
    SDL_FreeSurface(starsurface);

    //}

    starsurface = IMG_Load("book2.png");
    game->booktouch = SDL_CreateTextureFromSurface(game->renderer, starsurface);
    SDL_FreeSurface(starsurface);





    //initialise stars
    for (int i = 0; i < 100; i++) {
        game->stars[i].x = i * 500;//random number between 0 and 4000
        game->stars[i].y = rand() % 400;
    }

    //initilise the ledges
    for (int i = 0; i < 100; i++) {
        game->ledges[i].w = 256;
        game->ledges[i].h = 64;
        game->ledges[i].x = i * 256;
        game->ledges[i].y = 400;
    }

    game->ledges[99].x = 350;
    game->ledges[99].y = 200;

    //initilase clouds
    for (int i = 0; i < 100; i++) {
        game->trees[i].x = rand() % 6000;
        game->trees[i].y = 0;
    }

    for (int i = 99, j = 1; i > 50; i--, j++) {
        game->ledges[i].x = j * 550;
        game->ledges[i].y = 200 - rand() % 100;
    }


    //initilase books
    for (int i = 0; i < 100; i++) {
        game->books[i].x = i * 400;
        game->books[i].y = rand() % 600;
    }

    //initilase bushes
    for (int i = 0; i < 20; i++) {
        game->bushes[i].x = rand() % 6000;
        game->bushes[i].y = 235;
    }

    //for (int i = 0; i < 20; i++) {
        //if (game->man.isDead == 1) {
      //      game->fires[i].x = game->man.x;
        //    game->fires[i].y = game->man.y;
        //}

   // }


}

//To make sure that window appears on the screen until we click escape key or any buttton,
//we use while loop here.
int ProcessEvent(SDL_Window* window, Gamestate* game)//passing refernce of window as an argument
{
    SDL_Event event;
    int done = 0;

    while (SDL_PollEvent(&event))
    {
        switch (event.type) {
        case SDL_WINDOWEVENT_CLOSE:
            // case SDL_WINDOWEVENT_CLOSE:
        {
            if (window)
            {
                SDL_DestroyWindow(window);
                window = NULL;
                done = 1;
            }
        }
        break;

        case SDL_KEYDOWN:
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
            {
                done = 1;
            }break;
            case SDLK_RIGHT:
                game->man.x += 10;
                break;
            case SDLK_LEFT:
                game->man.x -= 10;
                break;
            case SDLK_UP:
                if (game->man.onLedge) {//he can jump only if he is on the ledge
                    game->man.dy = -5;
                    game->man.onLedge = 0;//once he jumps, he is not on ledge anymore..
                }

                //game->man.y -= 10;
                break;
            case SDLK_DOWN:

                game->man.y += 10;
                break;
            }
        }
        break;

        case SDL_QUIT:
        {
            done = 1;
        }
        break;
        }
    }
    //below code can be used instead of switch left and right keys.
    //state is basically an array. *state = is equal to state[] =
    //const Uint8* state = SDL_GetKeyboardState(NULL);
    //if (state[SDL_SCANCODE_RETURN]) {
      //  printf("RETURN IS PRESSED");
    //}

    return done;
}

void doRender(SDL_Renderer* renderer, Gamestate* game)
{
    int h = 0;

    if (game->statusState == STATUS_STATE_LIVES) {
        draw_status_lives(game);
    }

    else if (game->statusState == STATUS_STATE_GAME) {
        //Set the draw color of renderer to blue
        SDL_SetRenderDrawColor(renderer, 0, 0, 100, 100);

        SDL_RenderClear(renderer);

        //set the color of the renderer to white
        SDL_SetRenderDrawColor(renderer, 510, 510, 510, 510);

        for (int i = 0; i < 100; i++) {
            SDL_Rect ledgeRect = { game->scrollX + game->ledges[i].x,game->ledges[i].y,game->ledges[i].w,game->ledges[i].h };
            SDL_RenderCopy(renderer, game->brick, NULL, &ledgeRect);
        }


        //Draw a rectangle at man's position
        SDL_Rect rect = { game->scrollX + game->man.x,game->man.y,76,48 };
        //SDL_RenderFillRect(renderer, &rect);
        SDL_RenderCopyEx(renderer, game->manframes[0], NULL, &rect, 0, NULL, 0);


        //Drawing the images of stars
        for (int i = 0; i < 100; i++) {
            SDL_Rect starrect = { game->scrollX + game->stars[i].x,game->stars[i].y,64,64 };
            SDL_RenderCopy(renderer, game->star, NULL, &starrect);

        }

        for (int i = 0; i < 100; i++) {
            //size of clouds
            SDL_Rect starrect = { game->scrollX + game->trees[i].x,game->trees[i].y,70,70 };
            SDL_RenderCopy(renderer, game->tree, NULL, &starrect);

        }

        for (int i = 0; i < 100; i++) {
            //size of books
            SDL_Rect starrect = { game->scrollX + game->books[i].x,game->books[i].y,70,70 };
            SDL_RenderCopy(renderer, game->book, NULL, &starrect);

        }

        for (int i = 0; i < 20; i++) {
            //size of bushes
            SDL_Rect starrect = { game->scrollX + game->bushes[i].x,game->bushes[i].y,200,200 };
            SDL_RenderCopy(renderer, game->bush, NULL, &starrect);

        }

       /* if (game->man.isDead == 1)
        {
            SDL_Rect rect = { game->scrollX + game->man.x,game->man.y,48,48 };
            //SDL_RenderFillRect(renderer, &rect);
            SDL_RenderCopyEx(renderer, game->fire, NULL, &rect, 0, NULL, (game->time%20<10));

            SDL_Delay(1000);
            // printf("look around!");
            // init_status_lives(game);

             //  SDL_DestroyTexture(game->fire);
            SDL_Quit();
            exit(1);
            // printf("look around!");

        }*/
        //SDL_DestroyTexture(game->fire);
       if (game->man.isTouch == 1 ) {
          //  h = 0;
            //SDL_Rect rect = { game->scrollX + game->man.x,game->man.y,10,10 };
            SDL_Rect rect = { game->scrollX +200,200,10,10 };
            //SDL_RenderFillRect(renderer, &rect);
            SDL_RenderCopyEx(renderer, game->booktouch, NULL, &rect, 0, NULL, (game->time % 20 < 10));
            // printf("keep going buddy");

           // h++;
          //  game->man.count++;
            init_status_lives(game);
        }

        /*for (int i = 0; i < 20; i++) {
            //size of bushes
            SDL_Rect starrect = { game->scrollX + game->fires[i].x,game->fires[i].y,10,10 };
            SDL_RenderCopy(renderer, game->fire, NULL, &starrect);

        }*/




        //Clear the renderer with the draw color
        //SDL_RenderPresent(renderer);

        //Update the renderer which will show the renderer cleared by the draw color which is green
        //When we are done with drawing, this function presents us the screen
    }
    SDL_RenderPresent(renderer);
   // return game->man.count;

}

int main() {
    //int number;
    int a;
    int choice;
    printf("************************************************");
    printf("\nHello, press 1 to enter the game, any other key to exit the game!");
    scanf_s("%d", &choice);

    if (choice == 1)
    {

        printf("\nHello, welcome to the game");

        printf("\nChoose the gravity in the range 0.04f and 0.06f:");


        float g;
        scanf_s("%f", &g);
        if ((g > 0.06) || (g < 0.04)) {
            printf("\n Please select a valid value for the gravity");
        }
        else {

            printf("Answer the question correctly to enter into the game!");
            printf("\nAnswer the question below to enter the game: ");
            printf("\n What is the Capital city of India?");
            printf("\n1. Mumbai 2. Delhi");
            printf("\n3. Karnataka 4. Chennai");
            printf("\nPlease enter your option: ");
            scanf_s("%d", &a);



            int c;
            //scanf_s("%d", &a);
            if (a == 2) {
                void print();
                SDL_Init(SDL_INIT_VIDEO);//initialise SDL2
               // printf("Enter an integer: ");

                // reads and stores input
                //scanf_s("%d", &number);

                // displays output
                //printf("You entered: %d", number);
                SDL_Window* window = SDL_CreateWindow("Getting Started", SDL_WINDOWPOS_UNDEFINED,
                    SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);

                Gamestate gamestate;

                //Initialsing Random number generator
                srand(time(0)); //for the co-ordiantes of stars., to be typecasted into int datatype

                //Create a renderer for the window created above, with the first display driver present
                //and with no additional settings
                SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
                int done = 0;

                gamestate.renderer = renderer;

                //initialise the font system
                TTF_Init();
                //init_status_lives(&gamestate);
                Loadgame(&gamestate);

                Man man;
                //initialising man's omponents as same as rectangle
                man.x = 220;
                man.y = 140;

                // printf("hello");





                while (!done)
                {

                    //check for process events
                    done = ProcessEvent(window, &gamestate);

                    process(&gamestate);

                    //collission detection
                    collissionDetect(&gamestate, renderer, window);

                    //Render Display
                    doRender(renderer, &gamestate);
                    /* if (c == 1) {
                         int g;
                         int h, i, l, m, o;
                         printf("\nHow many continents are there in the world? ");
                         printf("\n1. 6 2. 7 3. 5 4. 9");
                         scanf_s("%d", &g);


                         printf("\nHow many countries are there in India ");
                         printf("\n1. 22 2. 28 3. 29 4. 27");
                         scanf_s("%d", &h);


                         printf("\nWho wrote Ramayana? ");
                         printf("\n1. Valmiki 2. Potana 3. Vadavyasa ");
                         scanf_s("%d", &i);




                         if ((g != 2) || (h != 3) || (i != 1)) {
                             printf("Sorry, wrong answer");
                             SDL_Quit();
                             exit(1);
                         }
                     }*/
                     /* else   if (c == 2) {
                          int g;
                          printf("1+1?");
                          scanf_s("%d", &g);
                          if (g != 2) {
                              printf("wrong ans");
                              SDL_Quit();
                              exit(1);
                          }
                      }*/

                      //Pause for 3 seconds (or 3000 milliseconds)
                    SDL_Delay(10);

                }

                //Destroy the created texture
                SDL_DestroyTexture(gamestate.star);

                if (gamestate.label != NULL) {
                    SDL_DestroyTexture(gamestate.label);
                }
                TTF_CloseFont(gamestate.font);

                //Destroy the renderer created above
                SDL_DestroyRenderer(renderer);

                //Destroy the window created above
                SDL_DestroyWindow(window);

                TTF_Quit();

                //Close all the systems of SDL initialized at the top
                SDL_Quit();

                return 0;
            }
            else {
                printf("Sorry, Try again");
            }
        }
    }


    else {
    printf("Exited from the game successfully!");
}
}
