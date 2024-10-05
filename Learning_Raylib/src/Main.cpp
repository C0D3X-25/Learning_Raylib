/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2013-2023 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h" 

typedef short int int16_t;              // -32,768 to 32,767
typedef unsigned short int u_int16_t;   // 0 to 65,535

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    const u_int16_t WINDOW_WIDTH = 800;
    const u_int16_t WINDOW_HEIGHT = 800;

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

	int16_t player_x{ 400 };
	int16_t player_y{ 400 };
	int16_t player_speed{ 5 };

    // Main game loop
    while (!WindowShouldClose()) {

        // Update
		//----------------------------------------------------------------------------------
        if (IsKeyDown(KEY_W)) {
            player_y -= player_speed;
        }
        if (IsKeyDown(KEY_S)) {
            player_y += player_speed;
        }

        if (IsKeyDown(KEY_A)) {
            player_x -= player_speed;
        }
        if (IsKeyDown(KEY_D)) {
            player_x += player_speed;
        }
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

		DrawCircle(player_x, player_y, 10, RED);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}