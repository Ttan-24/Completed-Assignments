// Assignment 2.cpp: A program using the TL-Engine
// Tanvi Prakash Gavali

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <vector>
using namespace tle;

struct GridForGameStruct     // Positions for grid
{
	float x;
	float y;
	float z;
};

enum EMarbleStates { Ready, Firing, Contact, Over };   // Types of marbles movement so that the player can perform burst firing

struct MarbleStruct             // Storing each marbles movement
{
	float x;
	float y;
	float z;
	EMarbleStates state;
	IModel* model;
};

enum EDirection { Left, Right };     // To make the worm segments move across the grid in the direction as asked in gdd

struct WormSegmentStruct       // For each worm segment movement independently 
{
	int x;
	int y;
	int z;
	EDirection direction;  // enums used inside structs - for the direction of each worm segment 
	IModel* model;        // used model to store movements and directions in each worm segments individually
};

////////Making a grid of 16 by 20 in the game//// used to place the mushrooms and worms correctly as in gdd/////////////////////////////
GridForGameStruct GridToWorld(int x, int y);

const GridForGameStruct kGridWidth = { 16.0f , 20.0f };
// Position of grid 0,0
const GridForGameStruct kGridOrigin = { -((kGridWidth.x / 2) * 10) + 5 ,0.0f, 195.0f };       // This shows the grid of 16 by 20 with z axis at 200

enum EBoxSide { leftSide, rightSide, frontSide, backSide, noSide };    // List of possible values it can have ///// Used in for the box-sphere collision  

enum EMushroomStates { Cold, Hot, Veryhot, Removed };   //// States when marble hits the mushroom

struct MushroomStruct                         //  Each mushrooms position and movement independently
{
	int x;
	int z;
	EMushroomStates State;
	IModel* model;
};

enum EWormStates { Cold1, Hot1, Veryhot1, Removed1 };    //// States when marble hits the worms

enum EGameStates { Playing, Win, Lost };

bool sphere2sphere(float sphere1xPos, float sphere1zPos, float sphere1rad, float sphere2xPos, float sphere2zPos, float sphere2rad);   /// Function declaration   // Sphere-Sphere Colllision

EBoxSide sphere2box(float spherexPos, float spherezPos, float oldspherexPos, float oldspherezPos, float sphererad, float boxxPos, float boxzPos, float boxwidth, float boxdepth);  // Function declaration // Box-Sphere Collision

void main()
{

	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("C:\\Tanvi\\Assignment version 1.4\\Media");

	/**** Set up your scene here ****/

	//Const Variables
	const int   kNumMushrooms = 10; // number of mushrooms in the game
	const int kNumBushes = 5; // number of bushes in the game
	const int kNumBarriers = 11; // number of barriers on one side
	const int kNumWormSegments = 10; //Number of segments to make a worm
	const int kNumMarbles = 3;      // number of marbles in the game
	const float kMarbleSpeed = 10.0f;    // Speed at which the marble slides left and right 
	const float kMarblesMovement = 150.0f; // The speed at which marbles move(roll) when fired 
	const float kDummyMarbleRotation = 5.0f;  // The speed of dummy where the arrow is rotated at 45/-45 degree
	const float kDummyMarbleSpeed = 5.0f;     // The speed of dummy model 
	const float kDummyCameraRotation = 3.0f;  // The speed of rotation of camera
	const float kDummyCameraSpeed = 3.0f;     // The speed of camera movement 
	const float kMushroomSpeed = 15.0f;       // The speed of the mushroom when going up and down at Y-axis
	const float kMarbleRadius = 2.0f;          
	const float kMushroomRadius = 4.1f;
	const float kWormRadius = 7.0f;
	const int kRotationMax = 360;
	const int kGridZ = 15;
	const int kCameraDegreeUp = 45;             // The angle of camera to up
	const int kCameraDegreeDown = -35;          // The angle of camera to down
	const int kMushroomYAxis = -10;            
	const int kMarbleDummyRight = 45;         // The angle of arrow rotating right
	const int kMarbleDummyLeft = -45;         // The angle of arrow rotating left
	const int kMarbleCount = 3;

	GridForGameStruct kMarblePos[kMarbleCount]       // Positioning the marbles in the 16 by 20 grid 
	{
		{ 0.0f,2.0f,0.0f },
		{ 2.0f,2.0f,-4.0f },
		{ 5.0f,2.0f,-4.0f }
	};              


	// Load meshes
	IMesh* groundMesh = myEngine->LoadMesh("Ground.x");
	IMesh* skyboxMesh = myEngine->LoadMesh("Skybox_SciFi.x");
	IMesh* mushroomMesh = myEngine->LoadMesh("Mushroom.x");
	IMesh* marbleMesh = myEngine->LoadMesh("Marble.x");
	IMesh* arrowMesh = myEngine->LoadMesh("Arrow.x");
	IMesh* bushMesh = myEngine->LoadMesh("Bush.x");
	IMesh* barrierMesh = myEngine->LoadMesh("Barrier.x");
	IMesh* dummyMesh = myEngine->LoadMesh("dummy.x");
	IMesh* wormMesh = myEngine->LoadMesh("Segment.x");

	// Create models
	IModel* ground = groundMesh->CreateModel(0.0f, 0.0f, 0.0f);
	IModel* skybox = skyboxMesh->CreateModel(0.0f, -1000.0f, 0.0f);
	IModel* arrow = arrowMesh->CreateModel(0.0f, 0.0f, -10.0f);
	IModel* dummyMarble = dummyMesh->CreateModel(0.0f, 2.0f, 0.0f);
	
	// Player control keys defined:
	const EKeyCode kQuitKey = Key_Escape;	 // Quit game
	const EKeyCode LeftKey = Key_Z;          // The marble slides left 
	const EKeyCode RightKey = Key_X;         // The marble slides right 
	const EKeyCode SpaceKey = Key_Space;     // The marble hits the mushrooms or worms                 
	const EKeyCode RotateRightKey = Key_Period; // The marble rotates till 45 degrees right 
	const EKeyCode RotateLeftKey = Key_Comma;   // The marble rotates till 45 degrees left
	const EKeyCode CameraUpKey = Key_Up;        // The camera moves up 
	const EKeyCode CameraDownKey = Key_Down;    // The camera moves down under a limit where it does not go under the ground 
	const EKeyCode CameraPositionKey = Key_C;   // The camera return to its original position

	// Variables
	float xAxisRight = 75.0f;   // The X axis right side range in the game
	float xAxisLeft = -75.0f;   // The X axis left side range in the game
	float zAxisRange = 210.0f;  // The Z axis range in the game
	float marbleAngle = 0.0f;   // Angle of the marble in which it rotates // To show the limit of the marble between 45/-45
	float arrowAngle = 0.0f;    // Angle of the arrow in which it rotates //  To show the limit of the mabrle between 45/-45
	float dummyAngle = 0.0f;    // Angle of dummy at which it needs to move
	float barrierWidth = 2.0f;  
	float barrierDepth = 20.0f;   
	float xBarrierPosRight = 83;  // Right side of barrier position
	float xBarrierPosLeft = -83;  // Left side of barrier position
	float xGapBarrier = 18.0f;   // Gap between each barriers when placed
	int sectionBarrier = 4;
	float vectorMoveZ;  
	float vectorMoveX;
	float matrix[4][4];
	float wormMoveTimer = 0.0f;
	float marbleMoveTimer = 0.0f;


	////////////////////////////////////Placing the mushroom//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const int kMushroomCount = 10;
	// Storing x and y
	const int kAxisCount = 2;

	const int kMushroomStarts[kMushroomCount][kAxisCount] =   // Positions of mushrooms as per in gdd
	{
		{1,11},
		{2,1},
		{3,16},
		{5,6},
		{6,14},
		{9,4},
		{11,11},
		{12,18},
		{13,1},
		{15,13},
	};

	
	vector<MushroomStruct> mushrooms;    // vector is used so that we can use as many mushrooms we want other than the mushrooms positioned as per in gdd //  eg. Used when worms are hit and mushroom grows


	for (int mushroomPos = 0; mushroomPos < kMushroomCount; mushroomPos++) {

		MushroomStruct mushroom;
		mushroom.x = kMushroomStarts[mushroomPos][0];
		mushroom.z = kMushroomStarts[mushroomPos][1];

		GridForGameStruct mushroomWorld = GridToWorld(mushroom.x, mushroom.z);        // Mushrooms are placed in the 16 by 20 grid 

		mushroom.State = Cold;                                            

		mushroom.model = mushroomMesh->CreateModel(mushroomWorld.x, 0.0f, mushroomWorld.z);                 

		mushrooms.push_back(mushroom);     // To create many other mushrooms as one can 
	}

	EMushroomStates presentState = Cold;

	//////////////////////////////////////////Placing the bushes////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	IModel* bush[kNumBushes];

	float bushXs[kNumBushes] = { -70.0f, -30.0f, 10.0f, 50.0f, 70.0f };       // positioning the bushes at x axis

	for (int k = 0; k < kNumBushes; k++)

	{
		bush[k] = bushMesh->CreateModel(bushXs[k], 0.0f, zAxisRange);   // Creating bush model along the rear of the playing area (positioning 210 on z axis)
		int scaleMin = 30;
		int scaleMax = 50;
		int random = scaleMin + (rand() % (scaleMax - scaleMin));
		bush[k]->Scale(random);                 // Scale each bush differently random scale factor between 30 and 50
		int rotateY = rand() % kRotationMax;
		bush[k]->RotateY(rand() % kRotationMax);         // Rotation of each model different on Y axis
	}


	///////////////////////////////////Placing the barriers////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	IModel* barrier[kNumBarriers][2];     // using 2-D arrays to place the 11 bushes per right and left side 

	for (int j = 0; j < kNumBarriers; j++)
	{
		barrier[j][0] = barrierMesh->CreateModel(xBarrierPosRight, 0.0f, j* xGapBarrier);       // aligning the position as per gdd // setting each barrier 18 units apart in Z direction
		barrier[j][1] = barrierMesh->CreateModel(xBarrierPosLeft, 0.0f, j* xGapBarrier);

		if (j >= sectionBarrier)                                      // Setting skins provided to the barriers as per gdd // black and yellow wasp strips for the 4 sections to the nearest of the camera
		{
			barrier[j][0]->SetSkin("barrier1a.bmp");
			barrier[j][1]->SetSkin("barrier1a.bmp");
		}
	}

	////////////////////////////////////Placing the worm/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	IModel* worm[kNumWormSegments];         

	WormSegmentStruct worm_segment_array[kNumWormSegments];

	const int kWormCount = 10;
	// Storing x and y
	const int kGridCount = 2;

	const int kWorms[kWormCount][kGridCount] = {                  // Positioning of the worms as per the gdd 
			{0,0},
			{1,0},
			{2,0},
			{3,0},
			{4,0},
			{5,0},
			{6,0},
			{7,0},
			{8,0},
			{9,0}
	};

	for (int wormPos = 0; wormPos < kWormCount; wormPos++) {
		int x = kWorms[wormPos][0];
		int z = kWorms[wormPos][1];

		GridForGameStruct wormWorld = GridToWorld(x, z);      // Setting of the worms in the grid 16 by 20 

		worm_segment_array[wormPos].model = wormMesh->CreateModel(wormWorld.x, 10.0f, wormWorld.z);
		worm_segment_array[wormPos].direction = EDirection::Right;      // The worm segments start moving from right in the game
		worm_segment_array[wormPos].x = x;                             // positioning them and calling their x and z positions whenever we want to 
		worm_segment_array[wormPos].z = z;
	}



	EWormStates wormHit[kNumWormSegments];
	for (int j = 0; j < kNumWormSegments; j++)
	{
		wormHit[j] = Cold1;      // The worms state before the marble hits 
	}

	///////////////////////////////////////////////// Marbles ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	IModel* marble[kNumMarbles];

	MarbleStruct array_marbles[kNumMarbles];

	

	for (int m = 0; m < kNumMarbles; m++)
	{
		array_marbles[m].model = marbleMesh->CreateModel(kMarblePos[m].x, kMarblePos[m].y, kMarblePos[m].z);
		array_marbles[m].state = EMarbleStates::Ready;              // The marbles are inn ready state in the start of the game
		array_marbles[m].model->AttachToParent(dummyMarble);       // The marbles are attached to the dummy marble so that it can attach and detach the arrow whenever

	}

	// Arrow is attached to the dummy marble, so that arrow moves when the marble moves
	arrow->AttachToParent(dummyMarble);         
	

	//////////////////////////////////////////////////////Camera ///////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	float CameraAngle = 0.0f;   // Angle of the camera at which it will move up and down within the limits

	IModel* dummyCamera = dummyMesh->CreateModel(0.0f, 0.0f, -10.0f);

	// Manual camera at location and angle as specified in GDD
	ICamera* myCamera = myEngine->CreateCamera(kManual, 0.0f, 75.0f, -75.0f);
	myCamera->RotateX(25.0f);    // so that it is suspended in the air and looking slightly downwards towards the ground
	myCamera->AttachToParent(dummyCamera);           // The camera is attached to the dummy camera so that it move up and down

	float defaultCameraMatrix[16];
	dummyCamera->GetMatrix(defaultCameraMatrix);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	IFont* myFont = myEngine->LoadFont("Comic Sans MS", 53);     // A text is displayed to show when game wins and lose
	EGameStates state = Playing;
	//int currentPosition = 0;

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		if (state == Playing)                                   // The game is in playiing state
		{
			float deltaTime = myEngine->Timer();         // Implementation of frame timing, so the game speed adjusts to the speed of the PC and doesn’t need kGameSpeed tweaking any more.
			// 1.0f = 1 seccond;

			wormMoveTimer += deltaTime;                  // To move the worm segments
			if (wormMoveTimer > 0.1f)
			{
				bool wormDestroyed = true;
				for (int a = 0; a < kNumWormSegments; a++)
				{
					wormMoveTimer -= 0.1f; // to not lose .x seccond, eg, resetting 1.2f to 0, loosing .2 seccond

					if (worm_segment_array[a].model != nullptr)
					{
						wormDestroyed = false;

						// If any segment reaches the final square the game is over and the remaining mushrooms turn purple or purple_hot if they are damaged.

						if (worm_segment_array[a].model->GetZ() <= 0.0f)    
						{
							for (int b = 0; b < mushrooms.size(); b++)
							{
								mushrooms[b].model->SetSkin("purple_mushroom.png");

								if (mushrooms[b].State > 0)
								{
									mushrooms[b].model->SetSkin("purple_mushroom_hot.png");
								}
								state = EGameStates::Lost;
							}
						}
						

						// If the worm segment movement reaches the left direction (i.e., 0 in 16 by 20 grid ) it moves one row forward and then move in the opposite direction along the next row
						if (worm_segment_array[a].direction == EDirection::Left)
						{
							if (worm_segment_array[a].x == 0)
							{
								worm_segment_array[a].z++;
								worm_segment_array[a].direction = EDirection::Right;
							}
							else
							{
								// If the worm segment lands on which contains mushroom then the segment move forward and reverse its horizontal direction
								// And when a segment is turned into a mushroom, all consecutive segments in rear of the mushroom changes direction – effectively creating a new independent worm.
								bool ChangedDirection = false;
								for (int i = 0; i < mushrooms.size(); i++)
								{


									if (worm_segment_array[a].x - 1 == mushrooms[i].x)
									{

										if (worm_segment_array[a].z == mushrooms[i].z)
										{
											ChangedDirection = true;
											worm_segment_array[a].z++;
											if (worm_segment_array[a].direction == Right)
											{
												worm_segment_array[a].direction = Left;
											}
											else if (worm_segment_array[a].direction == Left)
											{
												worm_segment_array[a].direction = Right;
											}


										}
									}

								}
								if (!ChangedDirection)
								{
									worm_segment_array[a].x--;
								}
							}

						}
						// If the worm segment movement reaches the right direction (i.e., 16 in 16 by 20 grid ) it moves one row forward and then move in the opposite direction along the next row
						else if (worm_segment_array[a].direction == EDirection::Right)
						{
							if (worm_segment_array[a].x == kGridZ)
							{
								worm_segment_array[a].z++;
								worm_segment_array[a].direction = EDirection::Left;
							}
							else
							{
								// Similarly, if the worm segment lands on which contains mushroom then the segment move forward and reverse its horizontal direction
								// And when a segment is turned into a mushroom, all consecutive segments in rear of the mushroom changes direction – effectively creating a new independent worm.
								bool ChangedDirection = false;
								for (int i = 0; i < mushrooms.size(); i++)
								{
									if (worm_segment_array[a].x + 1 == mushrooms[i].x)
									{
										if (worm_segment_array[a].z == mushrooms[i].z)
										{
											ChangedDirection = true;
											worm_segment_array[a].z++;
											if (worm_segment_array[a].direction == Right)
											{
												worm_segment_array[a].direction = Left;
											}
											else if (worm_segment_array[a].direction == Left)
											{
												worm_segment_array[a].direction = Right;
											}
										}
									}
								}

								if (!ChangedDirection)
								{

									worm_segment_array[a].x++;
								}
							}
						}
						// Sets the worm segments
						GridForGameStruct wormWorld = GridToWorld(worm_segment_array[a].x, worm_segment_array[a].z);
						worm_segment_array[a].model->SetX(wormWorld.x);
						worm_segment_array[a].model->SetZ(wormWorld.z);
					}
				}

				if (wormDestroyed)          // If all the worm segments are destroyed then the player wins the game
				{
					state = EGameStates::Win;
				}
			}

			if (myEngine->KeyHeld(CameraUpKey) && CameraAngle < kCameraDegreeUp)   // Camera rotates at 45 degrees up 
			{
				dummyCamera->RotateX(kDummyCameraSpeed * deltaTime * kDummyCameraRotation);
				CameraAngle += kDummyCameraSpeed * deltaTime * kDummyCameraRotation;
			}

			if (myEngine->KeyHeld(CameraDownKey) && CameraAngle > kCameraDegreeDown)    // Camera rotates at 35 degrees down so that it cannot go down under the floor
			{
				dummyCamera->RotateX(-kDummyCameraSpeed * deltaTime * kDummyCameraRotation);
				CameraAngle -= kDummyCameraSpeed * deltaTime * kDummyCameraRotation;
			}

			if (myEngine->KeyHit(CameraPositionKey))      // The camera returns to its original position.
			{
				dummyCamera->SetMatrix(defaultCameraMatrix);
				CameraAngle = 0.0f;       
			}

			int MarbleFire = 0;

			for (int j = 0; j < kNumMarbles; j++)       // The Firing of the three marbles starts at the ready state
			{
				if (array_marbles[j].state == EMarbleStates::Ready)
				{
					MarbleFire = j;
					break;
				}
			}

			bool collision = false;
			
			for (int i = 0; i < mushrooms.size(); i++)
			{
				if (mushrooms[i].State != Removed)
				{

					if (mushrooms[i].model->GetY() < 0.0f)     // the mushroom created is below 0.0f so that it can slowly come up 
					{
						mushrooms[i].model->MoveY(kMushroomSpeed * deltaTime);    // Moves the mushroom slowly up the ground once the worm segments are destroyed 
					}
				}
				if (mushrooms[i].State == Removed)
				{
					mushrooms[i].model->MoveY(-kMushroomSpeed * deltaTime);    // Moves the mushroom slowly down the ground 
					if (mushrooms[i].model->GetY() < kMushroomYAxis)    // Once the mushroom moves down the mushroom gets deleted completely
					{
						mushroomMesh->RemoveModel(mushrooms[i].model);   // Removes the mushroom model

						mushrooms.erase(mushrooms.begin() + i);   // Remove the mushroom from the array permanently 
					}
				}
			}


			// When worm segment is destroyed, a new mushroom is grown in place of that worm segment and that worm segment is permanently removed
			for (int i = 0; i < kNumWormSegments; i++)
			{
				if (wormHit[i] == Removed && worm_segment_array[i].model != nullptr)
				{
					MushroomStruct temp;
					temp.x = worm_segment_array[i].x;
					temp.z = worm_segment_array[i].z;

					temp.State = Cold;
					GridForGameStruct wormWorld = GridToWorld(temp.x, temp.z);
					temp.model = mushroomMesh->CreateModel(wormWorld.x, -10.0f, wormWorld.z);

					mushrooms.push_back(temp);     // To create many other mushrooms as one can 

					wormMesh->RemoveModel(worm_segment_array[i].model);   // The worm segment model is removed
					worm_segment_array[i].model = nullptr;      // The worm segment is permanently removed 
				}
			}

			/**** Update your scene each frame here ****/
			for (int m = 0; m < kNumMarbles; m++)
			{
				switch (array_marbles[m].state)
				{
				case  EMarbleStates::Ready:
				{
					if (array_marbles[m].state != EMarbleStates::Firing)         // The marble does not move sideways while it is firing state
					{

						if (myEngine->KeyHeld(LeftKey) && dummyMarble->GetX() > xAxisLeft)   // The marble slide left when pressed Key Z within the limits of the playing area
						{
							dummyMarble->MoveX(-kMarbleSpeed * deltaTime);
						}

						if (myEngine->KeyHeld(RightKey) && dummyMarble->GetX() < xAxisRight)     // The marble slide right when pressed Key X within the limits of the playing area
						{
							dummyMarble->MoveX(kMarbleSpeed*deltaTime);
						}

					}

					if (myEngine->KeyHeld(RotateRightKey) && dummyAngle < kMarbleDummyRight)   // The marble rotates right when pressed Key Period (full-stop) within +45 degrees
					{
						// The rotation takes the same time as it takes to slide the marble from one side to the other
						dummyMarble->RotateY(kDummyMarbleSpeed * deltaTime * kDummyMarbleRotation);
						dummyAngle += kDummyMarbleSpeed * deltaTime * kDummyMarbleRotation;
					}

					if (myEngine->KeyHeld(RotateLeftKey) && dummyAngle > kMarbleDummyLeft)   // The marble rotates left when pressed Key Comma within -45 degrees
					{
						// The rotation takes the same time as it takes to slide the marble from one side to the other
						dummyMarble->RotateY(-kDummyMarbleSpeed * deltaTime * kDummyMarbleRotation);
						dummyAngle -= kDummyMarbleSpeed * deltaTime * kDummyMarbleRotation;
					}


					if (myEngine->KeyHit(SpaceKey))     // Hiting the Space Key causes a transiion to the Firing State
					{
						
						array_marbles[MarbleFire].model->DetachFromParent();       // The marble fired detaches from the arrow 
						array_marbles[MarbleFire].model->SetPosition(dummyMarble->GetX(), 2, dummyMarble->GetZ());
						array_marbles[MarbleFire].model->RotateY(dummyAngle);
						array_marbles[MarbleFire].state = EMarbleStates::Firing;
						array_marbles[MarbleFire].model->GetMatrix(&matrix[0][0]);     // The marble bounces in proper angle when it hits the barrier
						array_marbles[MarbleFire].x = matrix[2][0];
						array_marbles[MarbleFire].z = matrix[2][2];
		
					}

					break;
				}

				case  EMarbleStates::Firing:
				{

					if (array_marbles[m].model->GetZ() < zAxisRange)    // The marble should be within the Z axis range (210) 
					{
						array_marbles[m].model->Move(array_marbles[m].x * kMarblesMovement * deltaTime, 0, array_marbles[m].z * kMarblesMovement * deltaTime);
						array_marbles[m].model->RotateLocalX(array_marbles[m].z * kMarblesMovement * deltaTime);
					}
					else
					{
						array_marbles[m].model->SetPosition(dummyMarble->GetX(), 2, dummyMarble->GetZ());
						array_marbles[m].state = EMarbleStates::Contact;
					}


					// Check for collision

					for (int i = 0; i < mushrooms.size(); i++)
					{
						if (mushrooms[i].State != Removed)
						{
							// Marbles and mushroom collisions -  When a marble hits the mushroom
							if (sphere2sphere(array_marbles[m].model->GetX(), array_marbles[m].model->GetZ(), kMarbleRadius, mushrooms[i].model->GetX(), mushrooms[i].model->GetZ(), kMushroomRadius))
							{
								collision = true;
								array_marbles[m].state = EMarbleStates::Contact;

								// Once the mushroom is hit by the marble, the mushroom changes color according to the number of hits the mushroom gets until the mushroom is completely destroyed
								switch (mushrooms[i].State)          
								{
								case Cold:
								{
									mushrooms[i].State = Hot;
									mushrooms[i].model->SetSkin("mushroom_hot.png");

									break;
								}
								case Hot:
								{
									mushrooms[i].State = Veryhot;
									mushrooms[i].model->SetSkin("mushroom_very_hot.png");

									break;
								}
								case Veryhot:
								{
									mushrooms[i].State = Removed;
									break;
								}

								case Removed:
								{
									break;
								}

								}

							}

						}

					}

					for (int j = 0; j < kNumWormSegments; j++)
					{
						if (worm_segment_array[j].model != nullptr)
						{
							if (wormHit[j] != Removed1)
							{
								// Collision between marbles and worm segments - When marbles hit the worm segments
								if (sphere2sphere(array_marbles[m].model->GetX(), array_marbles[m].model->GetZ(), kMarbleRadius, worm_segment_array[j].model->GetX(), worm_segment_array[j].model->GetZ(), kWormRadius))
								{
									array_marbles[m].state = EMarbleStates::Contact;

									// Once a worm segment is hit by the marble, the worm segment changes color according to the number of hits the worm segment gets until the worm segment is completely destroyed
									switch (wormHit[j])
									{
									case Cold1:
									{
										wormHit[j] = Hot1;
										worm_segment_array[j].model->SetSkin("wormskin_hot.jpg");
										break;
									}
									case Hot1:
									{
										wormHit[j] = Veryhot1;
										worm_segment_array[j].model->SetSkin("wormskin_very_hot.jpg");

										break;
									}
									case Veryhot1:
									{
										wormHit[j] = Removed1;

										break;
									}

									}

								}

							}

						}

					}


					if (myEngine->KeyHeld(LeftKey) && dummyMarble->GetX() > xAxisLeft)   // The marble slide left when pressed Key Z within the limits of the playing area
					{
						dummyMarble->MoveX(-kMarbleSpeed * deltaTime);
					}


					if (myEngine->KeyHeld(RightKey) && dummyMarble->GetX() < xAxisRight)     // The marble slide right when pressed Key X within the limits of the playing area
					{
						dummyMarble->MoveX(kMarbleSpeed*deltaTime);
					}

					if (myEngine->KeyHeld(RotateRightKey) && dummyAngle < kMarbleDummyRight)   // The marble rotates right when pressed Key Period (full-stop) within +45 degrees
					{
						// The rotation takes the same time as it takes to slide the marble from one side to the other
						dummyMarble->RotateY(kDummyMarbleSpeed * deltaTime * kDummyMarbleRotation);
						dummyAngle += kDummyMarbleSpeed * deltaTime * kDummyMarbleRotation;
					}

					if (myEngine->KeyHeld(RotateLeftKey) && dummyAngle > kMarbleDummyLeft)   // The marble rotates left when pressed Key Comma within -45 degrees
					{
						// The rotation takes the same time as it takes to slide the marble from one side to the other
						dummyMarble->RotateY(-kDummyMarbleSpeed * deltaTime * kDummyMarbleRotation);
						dummyAngle -= kDummyMarbleSpeed * deltaTime * kDummyMarbleRotation;
					}


					float oldZ = array_marbles[m].model->GetZ();
					float oldX = array_marbles[m].model->GetX();

					// Check for collision between marble and barriers
					for (int i = 0; i < kNumBarriers; i++)
					{
						EBoxSide collision = sphere2box(array_marbles[m].model->GetX(), array_marbles[m].model->GetZ(), oldX, oldZ, kMarbleRadius, barrier[i][0]->GetX(), barrier[i][0]->GetZ(), barrierWidth, barrierDepth);
						if (collision == noSide)
						{

							collision = sphere2box(array_marbles[m].model->GetX(), array_marbles[m].model->GetZ(), oldX, oldZ, kMarbleRadius, barrier[i][1]->GetX(), barrier[i][1]->GetZ(), barrierWidth, barrierDepth);
						}


						// Resolve Collision
						// When marble hits the barriers it bounces off at an appropriate angle following a collision
						if (collision == frontSide || collision == leftSide || collision == rightSide || collision == backSide)
						{
							array_marbles[m].x = -array_marbles[m].x;
							array_marbles[m].model->Move(array_marbles[m].x, 0.0f, array_marbles[m].z);
						}
					}

					break;
				}

				case  EMarbleStates::Contact:
				{
					int currentPosition = 0;
					for (int j = 0; j < kNumMarbles; j++)
					{
						if (array_marbles[j].state == EMarbleStates::Ready)
						{
							currentPosition++;     // The three marbles can shoot in a burst firing mode one after another
						}

					}

					// Resetting the positions for all three marbles

					if (currentPosition == 0)
					{
						array_marbles[m].model->ResetOrientation();
						array_marbles[m].model->AttachToParent(dummyMarble);
						array_marbles[m].model->SetPosition(dummyMarble->GetX(), dummyMarble->GetY(), dummyMarble->GetZ());

					}
					else if (currentPosition == 1)
					{
						array_marbles[m].model->ResetOrientation();
						array_marbles[m].model->AttachToParent(dummyMarble);
						array_marbles[m].model->SetLocalPosition(kMarblePos[1].x, kMarblePos[1].y, kMarblePos[1].z);

					}

					else if (currentPosition == 2)
					{
						array_marbles[m].model->ResetOrientation();
						array_marbles[m].model->AttachToParent(dummyMarble);
						array_marbles[m].model->SetLocalPosition(kMarblePos[2].x, kMarblePos[2].y, kMarblePos[2].z);
					}


					
					// Reset the state of the marble
					array_marbles[m].state = EMarbleStates::Ready;
					break;
				}

				case Over:
				{
					// Reset the state of the marble
					array_marbles[m].state = EMarbleStates::Ready;
					
					break;
				}
				}
			}
		}
		else if (state == EGameStates::Lost)  // The player loses the game i.e., the worm segment reaches the final square
		{
			myFont->Draw("GameOver", 200, 100);

		}
		else if (state == EGameStates::Win)   // The player wins the game i.e., all the worm segments destroyed
		{
			
			myFont->Draw("Congratulations! You Won", 200, 100);  
			for (int i = 0; i < kNumMarbles; i++)
			{
				array_marbles[i].model->SetSkin("glass_green.jpg");
			}
			
		}
		// Test for quit
		if (myEngine->KeyHit(kQuitKey)) myEngine->Stop(); // stop the engine (will exit the game loop)

	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}



// Converts grid position to world coordinate
GridForGameStruct GridToWorld(int x, int z)
{
	GridForGameStruct result;

	result.x = kGridOrigin.x + (x * 10);

	result.z = kGridOrigin.z - (z * 10);

	return result;
}

// Sphere-sphere collision function definition 
bool sphere2sphere(float sphere1xPos, float sphere1zPos, float sphere1rad, float sphere2xPos, float sphere2zPos, float sphere2rad)    // Function definition
{
	float distX = sphere2xPos - sphere1xPos;
	float distZ = sphere2zPos - sphere1zPos;
	float distance = sqrt(distX*distX + distZ * distZ);

	return (distance < (sphere1rad + sphere2rad));
}

// Sphere-box collision function definition
EBoxSide sphere2box(float spherexPos, float spherezPos, float oldspherexPos, float oldspherezPos, float sphererad, float boxxPos, float boxzPos, float boxwidth, float boxdepth)
{
	// Calculate the boundaries of the square collision box
	float minX = boxxPos - boxwidth / 2 - sphererad;
	float maxX = boxxPos + boxwidth / 2 + sphererad;
	float minZ = boxzPos - boxdepth / 2 - sphererad;
	float maxZ = boxzPos + boxdepth / 2 + sphererad;

	EBoxSide result = noSide;

	// Check to see if the sphere is inside the box
	if (spherexPos > minX && spherexPos < maxX && spherezPos > minZ && spherezPos < maxZ)
	{
		if (oldspherexPos < minX) result = leftSide;
		else if (oldspherexPos > maxX) result = rightSide;
		else if (oldspherezPos < minX) result = frontSide;
		else result = backSide;
	}

	return(result);
}



