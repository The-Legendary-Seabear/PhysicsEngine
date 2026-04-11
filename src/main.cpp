
#include "raylib.h"
#include "raymath.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include <vector>
#include <string>

#include "../Body.h"
#include "../World.h"
#include "../Random.h"
#include "../Point_Effector.h"
#include "../GravitationalEffector.h"

int main ()
{
SetRandomSeed(5);
	Random random;
	World world;

	//SetTargetFPS(60);
	float timeAccum = 0.0f;
	float fixedTimeStep = 1.0f / 60.0f; // 0.016 * 60.0 = 1.0


	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");
	
	//world.AddEffector(new PointEffector(Vector2{ 200,200 }, 100, 30000.0f));
	//world.AddEffector(new GravitationalEffector(10000.0f));

	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		float dt = GetFrameTime();

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || (IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)))
		{
			Body body;
			body.bodyType = (IsKeyDown(KEY_LEFT_ALT)) ? BodyType::Static : BodyType::Dynamic;
			body.position = GetMousePosition();
			float angle = random.GetRandomFloat() * (2 * PI);
			Vector2 direction;
			direction.x = cosf(angle);
			direction.y = sinf(angle);

			body.AddForce(direction * (50.0f + (random.GetRandomFloat() * 500)), ForceMode::VelocityChange);

			//body.velocity = Vector2Scale(direction, random.GetRandomFloat() * 300);//direction * (GetRandomFloat() * 500);
			body.acceleration = Vector2{ 0, 0 };
			body.size = 5.0f + (random.GetRandomFloat() * 20.0f);
			body.restitution = 0.75f + (random.GetRandomFloat() * 0.5f);
			body.mass = body.size;
			body.inverseMass = (body.bodyType == BodyType::Static) ? 0 : 1.0f / body.mass;
			body.gravityScale = 1.0f;
			body.damping = 2.5f;

			world.AddBody(body);
		}

		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
			Vector2 position = GetMousePosition();
			for (auto& body : world.bodies) {
				Vector2 direction = position - body.position;
				if (Vector2Length(direction) <= 100.0f) {
					Vector2 force = Vector2Normalize(direction) * 10000.0f;
					body.AddForce(force);
				}
			}
			DrawCircleLinesV(position, 100, WHITE);
		}

		timeAccum += dt;
		while (timeAccum > fixedTimeStep) 
		{
			world.Step(fixedTimeStep);
			timeAccum -= fixedTimeStep;
		}

		//DRAW

		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		std::string fpsText = "FPS: ";
		fpsText += std::to_string(GetFPS());

		// draw some text using the default font
		DrawText(fpsText.c_str(), 100, 100, 20, WHITE);

		
		world.Draw();
		
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(wabbit);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
