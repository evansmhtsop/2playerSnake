#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>


#define SCREEN_W 1536
#define SCREEN_H 1152
#define BOX 20
#define MAX_TAIL 100
#define FOODS 50 


typedef struct {
    int x, y;
} Spot;


typedef struct {
    Spot head;
    int size;
    Spot tail[MAX_TAIL];
    Spot go;
    Color color1;
    Color color2;
    int alive;
    int points;
} Snake;


typedef struct {
    Spot place;
    int active;
    Color color;
} Food;

void StartSnake(Snake *s, int x, int y, Color c1, Color c2);
void MoveSnake(Snake *s, Food foods[], Snake *other, int *game_over);
void ShowSnake(Snake *s);
void MakeFood(Food foods[], Snake *s1, Snake *s2);
void MakeAllFoods(Food foods[], Snake *s1, Snake *s2);
int HitSnake(Spot p, Snake *s);
Color RandomFoodColor();

int main() {
    
    InitWindow(SCREEN_W, SCREEN_H, "2 PLAYER SNAKE GAME!!!");
    SetTargetFPS(10);  

 
    Snake snake1;  
    Snake snake2;  
    Food foods[FOODS];
    int game_over = 0;
    int winner = 0;  

   
    StartSnake(&snake1, 5, 5, DARKGREEN, GREEN);
    StartSnake(&snake2, 40, 30, DARKBLUE, BLUE);
    
    
    for (int i = 0; i < FOODS; i++) {
        foods[i].active = 0;
    }
    
   
    MakeAllFoods(foods, &snake1, &snake2);

    
    while (!WindowShouldClose()) {
        if (!game_over) {
           
            MoveSnake(&snake1, foods, &snake2, &game_over);
            
            
            if (!game_over) {
                MoveSnake(&snake2, foods, &snake1, &game_over);
            }
            
         
            if (game_over) {
                if (!snake1.alive && !snake2.alive) {
                    
                    winner = 0;
                } else if (!snake1.alive) {
                 
                    winner = 2; 
                } else {
                    
                    winner = 1; 
                }
            }
        }

     
        BeginDrawing();
        ClearBackground(BLACK);

        if (game_over) {
       
            DrawText("GAME OVER!!!", SCREEN_W/2 - 150, SCREEN_H/2 - 40, 40, RED);
            
            char winner_text[100];
            if (winner == 0) {
                sprintf(winner_text, "TIE GAME! Points: P1=%d P2=%d", snake1.points, snake2.points);
            } else if (winner == 2) {
                sprintf(winner_text, "BLUE WINS!!! Points: P1=%d P2=%d", snake1.points, snake2.points);
            } else {
                sprintf(winner_text, "GREEN WINS!!! Points: P1=%d P2=%d", snake1.points, snake2.points);
            }
            
            DrawText(winner_text, SCREEN_W/2 - 200, SCREEN_H/2, 30, YELLOW);
            DrawText("PRESS R TO PLAY AGAIN!!!", SCREEN_W/2 - 180, SCREEN_H/2 + 40, 20, WHITE);
        } else {
           
            if (snake1.alive) ShowSnake(&snake1);
            if (snake2.alive) ShowSnake(&snake2);
            
           
            for (int i = 0; i < FOODS; i++) {
                if (foods[i].active) {
                    DrawRectangle(foods[i].place.x * BOX, foods[i].place.y * BOX, BOX, BOX, foods[i].color);
                }
            }
            
         
            char score1[20];
            char score2[20];
            sprintf(score1, "GREEN: %d", snake1.points);
            sprintf(score2, "BLUE: %d", snake2.points);
            
            DrawText(score1, 10, 10, 20, GREEN);
            DrawText(score2, SCREEN_W - 100, 10, 20, BLUE);
            
           
            DrawText("P1: WASD!!!!", 10, SCREEN_H - 30, 20, GREEN);
            DrawText("P2: ARROWS!!!!", SCREEN_W - 150, SCREEN_H - 30, 20, BLUE);
        }

        EndDrawing();

      
        if (game_over && IsKeyPressed(KEY_R)) {
            StartSnake(&snake1, 5, 5, DARKGREEN, GREEN);
            StartSnake(&snake2, 40, 30, DARKBLUE, BLUE);
            for (int i = 0; i < FOODS; i++) {
                foods[i].active = 0;
            }
            MakeAllFoods(foods, &snake1, &snake2);
            game_over = 0;
            winner = 0;
        }
    }

    CloseWindow();
    return 0;
}

void StartSnake(Snake *s, int x, int y, Color c1, Color c2) {
    s->head.x = x;
    s->head.y = y;
    s->size = 1;
    s->go.x = 1; 
    s->go.y = 0;
    s->color1 = c1;
    s->color2 = c2;
    s->alive = 1;
    s->points = 0;
    
    
}

void MoveSnake(Snake *s, Food foods[], Snake *other, int *game_over) {
    if (!s->alive) return;
    
    
    if (s->color1.g > s->color1.b) { 
        if (IsKeyPressed(KEY_W) && s->go.y == 0) {
            s->go.x = 0;
            s->go.y = -1;
        }
        if (IsKeyPressed(KEY_S) && s->go.y == 0) {
            s->go.x = 0;
            s->go.y = 1;
        }
        if (IsKeyPressed(KEY_A) && s->go.x == 0) {
            s->go.x = -1;
            s->go.y = 0;
        }
        if (IsKeyPressed(KEY_D) && s->go.x == 0) {
            s->go.x = 1;
            s->go.y = 0;
        }
    } else {  
        if (IsKeyPressed(KEY_UP) && s->go.y == 0) {
            s->go.x = 0;
            s->go.y = -1;
        }
        if (IsKeyPressed(KEY_DOWN) && s->go.y == 0) {
            s->go.x = 0;
            s->go.y = 1;
        }
        if (IsKeyPressed(KEY_LEFT) && s->go.x == 0) {
            s->go.x = -1;
            s->go.y = 0;
        }
        if (IsKeyPressed(KEY_RIGHT) && s->go.x == 0) {
            s->go.x = 1;
            s->go.y = 0;
        }
    }
    
 
    for (int i = s->size; i > 0; i--) {
        s->tail[i] = s->tail[i-1];
    }
    s->tail[0] = s->head;

 
    s->head.x += s->go.x;
    s->head.y += s->go.y;


    if (s->head.x < 0) s->head.x = SCREEN_W / BOX - 1;
    if (s->head.x >= SCREEN_W / BOX) s->head.x = 0;
    if (s->head.y < 0) s->head.y = SCREEN_H / BOX - 1;
    if (s->head.y >= SCREEN_H / BOX) s->head.y = 0;

    
    for (int i = 0; i < FOODS; i++) {
        if (foods[i].active && s->head.x == foods[i].place.x && s->head.y == foods[i].place.y) {
           
            if (s->size < MAX_TAIL) {
                s->size++;
            }
            s->points++;  
            foods[i].active = 0;  
            MakeFood(foods, s, other); 
        }
    }

    
    for (int i = 1; i < s->size; i++) {
        if (s->head.x == s->tail[i].x && s->head.y == s->tail[i].y) {
            
            s->alive = 0;
            *game_over = 1;
            break;
        }
    }
    

    if (other->alive) {
        if (s->head.x == other->head.x && s->head.y == other->head.y) {
       
            s->alive = 0;
            other->alive = 0;
            *game_over = 1;
        }
        
     
        for (int i = 0; i < other->size; i++) {
            if (s->head.x == other->tail[i].x && s->head.y == other->tail[i].y) {
                s->alive = 0;
                *game_over = 1;  
                break;
            }
        }
    }
}

void ShowSnake(Snake *s) {
  
    for (int i = 0; i < s->size; i++) {
        DrawRectangle(s->tail[i].x * BOX, s->tail[i].y * BOX, BOX, BOX, s->color2);
    }
    
   
    DrawRectangle(s->head.x * BOX, s->head.y * BOX, BOX, BOX, s->color1);
}


void MakeFood(Food foods[], Snake *s1, Snake *s2) {

    int food_index = -1;
    for (int i = 0; i < FOODS; i++) {
        if (!foods[i].active) {
            food_index = i;
            break;
        }
    }
    
    if (food_index == -1) return;  
    
    int max_tries = 100; 
    int tried = 0;
    
    while (tried < max_tries) {
        tried++;
        
        int ok = 1;
        Spot new_food;
        
       
        new_food.x = GetRandomValue(0, SCREEN_W / BOX - 1);
        new_food.y = GetRandomValue(0, SCREEN_H / BOX - 1);
        
        
        if (HitSnake(new_food, s1)) {
            ok = 0;
            continue;
        }
        
        
        if (HitSnake(new_food, s2)) {
            ok = 0;
            continue;
        }
        
       
        for (int i = 0; i < FOODS; i++) {
            if (foods[i].active && foods[i].place.x == new_food.x && foods[i].place.y == new_food.y) {
                ok = 0;
                break;
            }
        }
        
        if (ok) {
        
            foods[food_index].place = new_food;
            foods[food_index].active = 1;
            foods[food_index].color = RandomFoodColor();
            break; 
        }
    }
}


void MakeAllFoods(Food foods[], Snake *s1, Snake *s2) {

    for (int i = 0; i < FOODS; i++) {
        if (!foods[i].active) {
            int max_tries = 100;
            int tried = 0;
            
            while (tried < max_tries) {
                tried++;
                
                int ok = 1;
                Spot new_food;
                
            
                new_food.x = GetRandomValue(0, SCREEN_W / BOX - 1);
                new_food.y = GetRandomValue(0, SCREEN_H / BOX - 1);
                
        
                if (HitSnake(new_food, s1)) {
                    ok = 0;
                    continue;
                }
                
                if (HitSnake(new_food, s2)) {
                    ok = 0;
                    continue;
                }
                
              
                for (int j = 0; j < FOODS; j++) {
                    if (foods[j].active && foods[j].place.x == new_food.x && foods[j].place.y == new_food.y) {
                        ok = 0;
                        break;
                    }
                }
                
                if (ok) {
                
                    foods[i].place = new_food;
                    foods[i].active = 1;
                    foods[i].color = RandomFoodColor();
                    break;  
                }
            }
        }
    }
}

int HitSnake(Spot p, Snake *s) {

    if (p.x == s->head.x && p.y == s->head.y) {
        return 1;
    }
    

    for (int i = 0; i < s->size; i++) {
        if (p.x == s->tail[i].x && p.y == s->tail[i].y) {
            return 1;
        }
    }
    
    return 0;  
}

Color RandomFoodColor() {
    int c = GetRandomValue(0, 7);
    switch(c) {
        case 0: return RED;
        case 1: return ORANGE;
        case 2: return YELLOW;
        case 3: return PINK;
        case 4: return PURPLE;
        case 5: return GOLD;
        case 6: return SKYBLUE;
        default: return LIME;
    }
}
