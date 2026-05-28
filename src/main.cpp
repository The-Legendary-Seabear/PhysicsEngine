
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
#include "../Area_Effector.h"
#include "../Drag_Effector.h"
#include "../Spring.h"
#include "../world_camera.h"

#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#define GUI_PHYSICS_IMPLEMENTATION
#pragma warning ( push )
#pragma warning ( disable : 4576)
#include "gui_physics.h"
#pragma warning ( pop )

void AddBody(World& world,Random random, WorldCamera& camera);
void AddEffector(World& world, WorldCamera& camera);
GuiPhysicsState state;

int main ()
{
SetRandomSeed(5);
	World world;
	//WorldCamera world_camera(Vector2{GetScreenWidth() / 2.0f}, Vector2{GetScreenHeight() / 2.0f}
	WorldCamera world_camera(Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f }, 50);
	// set min (left-bottom) boundary(0, screen height) and max (right, top) boundary(screen width, 0)
	world.SetBounds(world_camera.ScreenToWorld({ 0, (float)GetScreenHeight() }), world_camera.ScreenToWorld({ (float)GetScreenWidth(), 0 }));
	Random random;

	Body* selectedBody = nullptr;



	//SetTargetFPS(60);
	float timeAccum = 0.0f;
	bool simulate = true;


	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Hello Raylib");

	// Get GUI state
	state = InitGuiPhysics();
	GuiLoadStyle("raygui/styles/dark/style_dark.rgs");


	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");
	
	//world.AddEffector(new PointEffector(Vector2{ 300,200 }, 200, 10.0f));
	//world.AddEffector(new GravitationalEffector(Vector2{ 900, 600 }, 200, 300.0f));
	//world.AddEffector(new AreaEffector(Vector2{ 900, 200 }, 200, 0 , 10.0f));
	//world.AddEffector(new DragEffector(Vector2{ 300, 600 }, 200, 1.0f));

	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		float fixedTimeStep = 1.0f / state.FPSValue; // 0.016 * 60.0 = 1.0
		float dt = fminf(GetFrameTime(), 0.1f);

		if (IsKeyPressed(KEY_SPACE)) state.SimulateActive = !state.SimulateActive;
		if (IsKeyPressed(KEY_TAB)) state.PhysicsPanelActive = !state.PhysicsPanelActive;
		World::SetGravity(Vector2{ 0.0f, state.GravityValue });

		bool mouseOverGui = state.PhysicsPanelActive && CheckCollisionPointRec(GetMousePosition(), Rectangle{state.anchor02.x, state.anchor02.y, 304, 664});
		if (!mouseOverGui) {
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || (IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)))
			{
				if (IsKeyDown(KEY_LEFT_SHIFT))
				{
					AddEffector(world, world_camera);
				}
				else
				{
					AddBody(world, random, world_camera);
				}
			}

			if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) 
			{
				//selectedBody = world.GetBodyIntersect(GetMousePosition());
				selectedBody = world.GetBodyIntersect(world_camera.ScreenToWorld(GetMousePosition()));
			}

			if (selectedBody) 
			{
				if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && IsKeyDown(KEY_LEFT_CONTROL)) 
				{
					Vector2 position = world_camera.ScreenToWorld(GetMousePosition());
					//Vector2 position = GetMousePosition();
					Vector2 force = Spring::GetSpringForce(position, selectedBody->position, 1.0f, 3.0f);
					selectedBody->AddForce(force);

					DrawLineV(world_camera.WorldToScreen(position), world_camera.WorldToScreen(selectedBody->position), WHITE);
					//DrawLineV(position, selectedBody->position, WHITE);
				}
			}
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

		if (state.SimulateActive) {
			timeAccum += dt;
			while (timeAccum > fixedTimeStep)
			{
				world.Step(fixedTimeStep);
				timeAccum -= fixedTimeStep;
			}
		}
		

		//DRAW

		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		std::string fpsText = "FPS: ";
		fpsText += std::to_string(GetFPS());


		world_camera.Begin(); // set world camera




		// draw some text using the default font
		DrawText(fpsText.c_str(), 100, 100, 20, WHITE);

		//world_camera.Begin();
		world.Draw();
		world_camera.End(); // remove world camera
		//DrawCircleLinesV(world_camera.ScreenToWorld(GetMousePosition()), state.BodySizeValue, BLUE);
		DrawCircleLinesV(GetMousePosition(), state.BodySizeValue, BLUE);
		GuiPhysics(&state);
		
		if (selectedBody) 
		{
			DrawCircleLinesV(selectedBody->position, selectedBody->size * 1.05f, RED);
		}
		
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

void AddBody(World& world, Random random, WorldCamera& camera)
{
	Body body;

	body.bodyType = (BodyType)state.BodyTypeActive;
	body.position = camera.ScreenToWorld(GetMousePosition());
	float angle = random.GetRandomFloat() * (2 * PI);
	Vector2 direction;
	direction.x = cosf(angle);
	direction.y = sinf(angle);

	body.AddForce(direction * state.BodyVelocityValue, ForceMode::VelocityChange);//(50.0f + (random.GetRandomFloat() * 500)), ForceMode::VelocityChange);

	//body.velocity = Vector2Scale(direction, random.GetRandomFloat() * 300);//direction * (GetRandomFloat() * 500);
	//body.acceleration = Vector2{ 0, 0 };
	body.size = state.BodySizeValue;//5.0f + (random.GetRandomFloat() * 20.0f);
	body.restitution = state.BodyRestitutionValue;//0.75f + (random.GetRandomFloat() * 0.5f);
	body.mass = body.size * state.BodyMassValue;
	body.inverseMass = (body.bodyType == BodyType::Static) ? 0 : 1.0f / body.mass;
	body.gravityScale = state.BodyGravityValue;
	body.damping = state.BodyDampingValue;

	world.AddBody(body);
}

void AddEffector(World& world, WorldCamera& camera)
{
	Vector2 position = camera.ScreenToWorld(GetMousePosition());
	// using body size instead of effect body size
	float size = state.BodySizeValue;
	Effector* effector = nullptr;
	switch ((EffectorType)state.EffectorTypeActive)
	{
	case(EffectorType::Area) :
		effector = (new AreaEffector(position, 200, 0, 10.0f));
		break;
	case(EffectorType::Drag) :
			effector = (new DragEffector(position, 200, 1.0f));
			break;
	case(EffectorType::Gravitation) :
		effector = (new GravitationalEffector(position, 200, 300.0f));
		break;
	case(EffectorType::Point) :
		effector = (new PointEffector(position, 200, 10.0f));
		break;
	default:
		break;
	}

	if (effector) world.AddEffector(effector);
	//world.AddEffector(new PointEffector(Vector2{ 300,200 }, 200, 10.0f));
	//world.AddEffector(new GravitationalEffector(Vector2{ 900, 600 }, 200, 300.0f));
	//world.AddEffector(new AreaEffector(Vector2{ 900, 200 }, 200, 0 , 10.0f));
	//world.AddEffector(new DragEffector(Vector2{ 300, 600 }, 200, 1.0f));
}
