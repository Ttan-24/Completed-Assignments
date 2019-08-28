// Assignment 3 version 1.2.cpp: A program using the TL-Engine
#include <TL-Engine.h>	// TL-Engine include file and namespace
/*#include ""*/
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace tle;
using namespace std;

struct Vec3
{
	int x;
	int y;
	int z;
};

enum EGameCheckPoints { Checkpoint1, Checkpoint2, Checkpoint3, Checkpoint4, Checkpoint5 };

struct CheckPointStruct                         //  Each Checkpoint 
{
	Vec3 position;
	EGameCheckPoints state;
	IModel* model;
	IModel* Strut[2];
};

struct IsleStruct                         //  Each Isle
{
	Vec3 position;
	IModel* model;
};

struct WallStruct                         //  Each Wall
{
	Vec3 position;
	IModel* model;
};

struct LargeTankStruct
{
	Vec3 position;
	IModel* model;
};

struct SmallTankStruct
{
	Vec3 position;
	IModel* model;
};

struct CrossStruct
{
	Vec3 position;
	IModel* model;
};

enum EGameHoverCar { start, checkpoint1, checkpoint2, checkpoint3, checkpoint4, checkpoint5, restart };

struct HoverCarStruct                         //  Each Wall
{
	Vec3 position;
	EGameHoverCar state = EGameHoverCar::start;
	IModel* model;
};

struct TreeStruct
{
	Vec3 position;
	IModel* model;
};

enum EGameDummyCar { check1, check2, check3, check4, check5};

struct DummyLocStruct
{
	Vec3 position;
	EGameDummyCar state = EGameDummyCar::check1;
	IModel* model;
};

struct vector2D
{
	float x;
	float z;
};

vector2D scalar(float s, vector2D v)
{
	return{ s * v.x, s * v.z };
}

vector2D sum3(vector2D v1, vector2D v2, vector2D v3)
{
	return{ v1.x + v2.x + v3.x, v1.z + v2.z + v3.z };
}

//vector2D multiply(vector2D )

enum EGameMoveState { Start, Go, Restart };

enum EBoxSide { leftSide, rightSide, frontSide, backSide, noSide };    // List of possible values it can have ///// Used in for the box-sphere collision  

bool sphere2sphere(float sphere1xPos, float sphere1zPos, float sphere1rad, float sphere2xPos, float sphere2zPos, float sphere2rad);   /// Function declaration   // Sphere-Sphere Colllision

EBoxSide sphere2box(float spherexPos, float spherezPos, float oldspherexPos, float oldspherezPos, float sphererad, float boxxPos, float boxzPos, float boxwidth, float boxdepth);  // Function declaration // Box-Sphere Collision



const float kGameSpeed = 0.1;
const float kHoverSpeed = 50.0f;            // Speed of the hovercar
const EKeyCode cameraForward = Key_Up;      // arrow up moves the camera forward side
const EKeyCode cameraBackward = Key_Down;   // arrow back moves the camera backward side
const EKeyCode cameraLeft = Key_Left;       // arrow left moves the camera left side
const EKeyCode cameraRight = Key_Right;     // arrow right moves the camera right side
const EKeyCode forwardKey = Key_W;          // to move the car foward when pressed W
const EKeyCode backwardKey = Key_S;         // to move the car backwards when pressed S
const EKeyCode clockwiseKey = Key_D;        // Car moves horizontally clockwise - Right when pressed D
const EKeyCode anticlockwiseKey = Key_A;    // Car moves horixontally anti-clockwise - Left when pressed A
const EKeyCode CameraPositionKey = Key_1;   // The camera return to its original position
const EKeyCode CameraFirstPersonKey = Key_2; // First person view shifting
const EKeyCode SpaceKey = Key_Space;         // Press Space to start the  game
const EKeyCode RestartKey = Key_Return;      // Press Enter to restart the game

// Camera control constants
const float kCameraMove = 20.0f;              // distance for the direction keys x and z axis
const float kMouseWheelMove = 10.0f;          // distance for wheel movement z axis
const float kMouseRotation = 1.0f;            // distance (in degrees) for rotation of the camera
const float kHoverCarRadius = 4.0f;           // the radius of the hovercar
const float kCheckPointsRadius = 1.0f;        // checkpoint radius
const float kDummyRadius = 1.0f;              // dummy radius
const float kTreeRadius = 10.0f;              // tree radius
const float kCheckpointWidth = 10.0f;         // width of checkpoint
const float kCheckpointDepth = 2.0f;          // depth of checkpoint
const float kLargeTankRadius = 4.0f;          // tank radius
const float kSmallTankRadius = 3.0f;          // tank radius
const int kModel = 1;                         // scale constant
const float kKmPerHour = 3.6;                 // conversion of 3600/1000 = 3.6



void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("C:\\ProgramData\\TL-Engine\\Media");
	myEngine->AddMediaFolder(".\\Models3");
	myEngine->AddMediaFolder(".\\Media");
	myEngine->AddMediaFolder(".\\Models");

	//////////////////////////////////////Reading the .txt file///////////////////////////////////
	ifstream infile("Positions.txt");
	string fileLine = "checkpoint   0   0     0";

	stringstream ss(fileLine);
	string part;
	while (getline(infile, fileLine))
	{
		cout << fileLine << endl;

	}
	

	/**** Set up your scene here ****/
	/////////// Strings
	string Grass = "Grass1.jpg";
	string Header = "ui_backdrop.jpg";
	string CarColor = "car2.jpg";
	string Font = "Comic Sans MS";
	string HitSpace = "Hit Space to Start";
	string TimerOne = "1";
	string TimerTwo = "2";
	string TimerThree = "3";
	string Wait = "Wait for 5 seconds";
	string BoostZero = "Boost: 0";
	string BoostOne = "Boost: 1";
	string BoostTwo = "Boost: 2";
	string BoostThree = "Boost: 3";
	string FPS = "FPS: ";
	string Speed = "Speed:  ";
	string CheckPointOne = "Checkpoint 1";
	string CheckPointTwo = "Checkpoint 2";
	string CheckPointThree = "Checkpoint 3";
	string CheckPointFour = "Checkpoint 4";
	string CheckPointFive = "Checkpoint 5";
	string CarLaps = "Laps : ";
	string HealthCar = "Health : ";
	string Enter = "Press Enter to restart";

	///////Const Variables - positioning of the models

	const int kNumStruts = 2;
	const int kNumCheckPoints = 5;
	const Vec3 kCheckpointPositions[kNumCheckPoints]{
		{ 0, 0, 0 },
		{ 0, 0, 100 },
		{ 70, 0, 250 },
		{ 180, 0, 120 },
		{ 180, 0, 20 }
	};

	const int kNumCrosses = 5;
	const Vec3 kCrossesPositions[kNumCrosses]{
		{0, -8, 0},
	    { 0, -8, 100 },
		{ 70, -8, 250 },
		{ 180, -8, 120 },
		{ 180, -8, 20 }
	};
	
	const int kNumIsles = 12;
	const Vec3 kIslePositions[kNumIsles]{
		{-20, 0, 30},
		{20, 0, 30},
		{-20, 0, 56},
		{20, 0, 56},
		{-20, 0, 80},
		{20, 0, 80},
		{-15, 0, 145},
		{15, 0, 145},
		{-15, 0, 170},
		{15, 0, 170},
		{-15, 0, 200},
		{15, 0, 200},
	};

	const int kNumWalls = 8;
	const Vec3 kWallPositions[kNumWalls]{
		{-20, 0, 43},
		{20, 0, 43},
		{-20, 0, 64},
		{20, 0, 64},
		{15, 0, 158},
		{-15, 0, 158},
		{15, 0, 184},
		{-15, 0, 184},
	};

	const int kNumHoverCar = 2;
	const Vec3 kHoverCarPositions[kNumHoverCar]{
		{ 0, 0, 0 },
		{5, 0, 0},
	};

	const int kNumDummy = kNumCheckPoints * 2;

	const int kNumLargeTanks = 2;
	const Vec3 kLargeTankPositions[kNumLargeTanks]
	{
		{0, 0, 270},
	    {180, 0, 250}
	};

	const int kNumSmallTanks = 1;
	const Vec3 kSmallTankPositions[kNumSmallTanks]
	{
		{12, -5, 130},
	};

	const int kNumTrees = 8;
	const Vec3 kTreePositions[kNumTrees]
	{
		{10, 0, 270},
		{15, 0, 270},
		{20, 0, 270},
		{25, 0, 270},
		{30, 0, 270},
		{35, 0, 270},
		{40, 0, 270},
		{45, 0, 270}
	};

	const int kNumDummiesCar = 27;
	const Vec3 kDummyPositions[kNumDummiesCar]
	{
		{5, 0, 0},
		{5, 0, 210},
		{5, 0, 230},
		{5, 0, 233},
		{5, 0, 235},
		{7, 0, 237},
		{12, 0, 240},
		{16, 0, 243},
		{17, 0, 244},
		{18, 0, 245},
		{20, 0, 248},
		{25, 0, 250},
		{30, 0, 253},
		{35, 0, 260},
		{37, 0, 262},
		{40, 0, 270},
		{60, 0, 272},
		{80, 0, 263},
		{100, 0, 274},
		{130, 0, 280},
	    {140, 0, 290},
	    {180, 0, 200},
	    {185, 0, 150},
	    {190, 0, 10},
	    {150, 0, -50},
	    {50, 0, -100},
	    {5, 0, 0}
	};

	

	// Load meshes
	IMesh* skyboxMesh = myEngine->LoadMesh("Skybox 07.x");
	IMesh* hovercarMesh = myEngine->LoadMesh("race2.x");
	IMesh* checkpointMesh = myEngine->LoadMesh("Checkpoint.x");
	IMesh* groundMesh = myEngine->LoadMesh("ground.x");
	IMesh* watertankOneMesh = myEngine->LoadMesh("TankSmall1.x");
	IMesh* watertankTwoMesh = myEngine->LoadMesh("TankSmall2.x");
	IMesh* wallendMesh = myEngine->LoadMesh("IsleStraight.x");
	IMesh* wallsectionMesh = myEngine->LoadMesh("Wall.x");
	IMesh* treeMesh = myEngine->LoadMesh("Bush.x");
	IMesh* redCrossMesh = myEngine->LoadMesh("Cross.x");
	//IMesh* bombMesh = myEngine->LoadMesh("Flare.x");
	IMesh* dummyMesh = myEngine->LoadMesh("dummy.x");

	//Create models
	
	IModel* skybox = skyboxMesh->CreateModel(0.0f, -960.0f, 0.0f);
	IModel* ground = groundMesh->CreateModel(0.0f, 0.0f, 0.0f);


	ground->SetSkin(Grass);

	ISprite* backdrop = myEngine->CreateSprite(Header, 5, 655);
	backdrop->MoveX(0.1f);


	///////////////////////////Variables////////////////////////
	
	float wallWidth = 10.0f;
	float wallDepth = 25.0f;
	float timer = 3.0f;    // the start timer when space hit is pressed
	float boostTimer = 5.0f; // to make the car boost for 3 secs
	float coolDowmTimer = 5.0f; // to make the car cool down after boost for 5 secs
	bool StartPressed = false;  
	bool boost = false;
	bool boostOverHeat = false;
	bool Health = false;
	bool movingDown = false;  // bouncing of the car
	float crossTimer[5] = { 0,0,0,0,0 }; // the cross will be there for 5 secs after the car moves past it
	bool crossCheckpoint = false;       
	int CarCounter = 0;           
	int CarLapCounter = 0;  // Lap counter for the AI hover car 
	int kHoverCarHealth = 100;  // The HP of hover car 
	int Lap = 0;              
	bool LapTrack = false;
	int CarOneCounter = 0;   
	int CarTwoCounter = 0;	//cycle through to move the ai car to different checkpoints
	int AngleNinety = 90;
	int AngleForty = 45;
	int checkX = 14;
	float ScaleNo = 2.0f;
	float ScaleTen = 10.0f;
	float cameraY = 10.0f;
	float cameraZ = -30.0f;
	float cameraYOne = 5.0f;
	float cameraZOne = -5.0f;
	int FontOne = 36;
	int FontTwo = 150;
	int FontXOne = 240;
	int FontYOne = 650;
	int FontXTwo = 640;
	int FontYTwo = 300;
	int FontXThree = 270;
	int FontYThree = 150;
	int FontXFour = 200;
	int FontYFour = 200;
	int FontXFive = 200;
	int FontYFive = 100;
	int FontXSix = 240;
	int FontYSix = 500;
	int FontXSeven = 350;
	int FontYSeven = 650;
	int One = 1;
	int Two = 2;
	int Three = 3;
	int Four = 4;
	int SpeedY = 2;
	int SpeedJump = 3;
	float RotateSpeed = 30.0f;
	int TimerNo = 5;
	int CrossYOne = -9;
	int CrossYTwo = 5;
	float ConvertNo = 3.6;
	int treeRadius = 2;
	int TotalHealth = 100;
	int LessHealth = 30;
	float doubleThrust = 1.5f;
	float currentRotation = 0.0f;
	bool Rotate = true;

	//////////////////////////////////////////////HOVERCAR/////////////////////////////////////////////////////
	vector<HoverCarStruct> hoverCars;

	for (int hovercarPos = 0; hovercarPos < kNumHoverCar; hovercarPos++)
	{
		HoverCarStruct hoverCar;
		hoverCar.position = kHoverCarPositions[hovercarPos];
		hoverCar.model = hovercarMesh->CreateModel(hoverCar.position.x, hoverCar.position.y, hoverCar.position.z);
		hoverCars.push_back(hoverCar);
	}

    hoverCars[1].state == EGameDummyCar::check1;
	hoverCars[1].model->SetSkin(CarColor);    // Set the AI hoverCar a different color


	////////////////////////////////////// Cross/////////////////////////////////////////////
	vector<CrossStruct> crosses;

	for (int crossPos = 0; crossPos < kNumCrosses; crossPos++)
	{
		CrossStruct cross;
		cross.position = kCrossesPositions[crossPos];
		cross.model = redCrossMesh->CreateModel(cross.position.x, cross.position.y, cross.position.z);
		crosses.push_back(cross);
	}

	crosses[2].model->RotateY(AngleNinety);   

	//////////////////////////////////////////////CHECKPOINTS//////////////////////////////////////////////////
	vector<CheckPointStruct> checkpoints;

	for (int checkpointPos = 0; checkpointPos < kNumCheckPoints; checkpointPos++)
	{
		CheckPointStruct checkpoint;
		checkpoint.position = kCheckpointPositions[checkpointPos];
		checkpoint.model = checkpointMesh->CreateModel(checkpoint.position.x, checkpoint.position.y, checkpoint.position.z);
		checkpoints.push_back(checkpoint);
		checkpoints[checkpointPos].state = EGameCheckPoints::Checkpoint1;
		bool horizontal = false;
		if (checkpointPos == 0 || checkpointPos == One || checkpointPos == Three || checkpointPos == Four) horizontal = true;
		if (horizontal)
		{
			checkpoints[checkpointPos].Strut[0] = dummyMesh->CreateModel(checkpoint.position.x + checkX, checkpoint.position.y, checkpoint.position.z);
			checkpoints[checkpointPos].Strut[1] = dummyMesh->CreateModel(checkpoint.position.x - checkX, checkpoint.position.y, checkpoint.position.z);

		}
		else
		{
			checkpoints[checkpointPos].Strut[0] = dummyMesh->CreateModel(checkpoint.position.x, checkpoint.position.y, checkpoint.position.z + checkX);
			checkpoints[checkpointPos].Strut[1] = dummyMesh->CreateModel(checkpoint.position.x, checkpoint.position.y, checkpoint.position.z - checkX);
		}
	}
	
	checkpoints[2].model->RotateY(AngleNinety);

	checkpoints[0].state = EGameCheckPoints::Checkpoint1;
	checkpoints[1].state = EGameCheckPoints::Checkpoint2;
	checkpoints[2].state = EGameCheckPoints::Checkpoint3;
	checkpoints[3].state = EGameCheckPoints::Checkpoint4;
	checkpoints[4].state = EGameCheckPoints::Checkpoint5;

	////////////Scaling
	checkpoints[0].model->Scale(ScaleNo);
	checkpoints[1].model->Scale(ScaleNo);
	checkpoints[2].model->Scale(ScaleNo);
	checkpoints[3].model->Scale(ScaleNo);
	checkpoints[4].model->Scale(ScaleNo);


	//////////////////////////////////////////////Isles////////////////////////////////////////////////////////
	vector<IsleStruct> isles;


	for (int islePos = 0; islePos < kNumIsles; islePos++)
	{
		IsleStruct isle;
		isle.position = kIslePositions[islePos];
		isle.model = wallendMesh->CreateModel(isle.position.x, isle.position.y, isle.position.z);
		isles.push_back(isle);
	}

	////////////Scaling
	isles[0].model->Scale(ScaleNo);
	isles[1].model->Scale(ScaleNo);
	isles[2].model->Scale(ScaleNo);
	isles[3].model->Scale(ScaleNo);
	isles[4].model->Scale(ScaleNo);
	isles[5].model->Scale(ScaleNo);
	isles[6].model->Scale(ScaleNo);
	isles[7].model->Scale(ScaleNo);
	isles[8].model->Scale(ScaleNo);
	isles[9].model->Scale(ScaleNo);
	isles[10].model->Scale(ScaleNo);
	isles[11].model->Scale(ScaleNo);

	////////////////////////////////////////////////Wall////////////////////////////////////////////////////////
	vector<WallStruct> walls;


	for (int wallPos = 0; wallPos < kNumWalls; wallPos++)
	{
		WallStruct wall;
		wall.position = kWallPositions[wallPos];
		wall.model = wallsectionMesh->CreateModel(wall.position.x, wall.position.y, wall.position.z);
		walls.push_back(wall);
	}

	////////////Scaling
	walls[0].model->Scale(ScaleNo);
	walls[1].model->Scale(ScaleNo);
	walls[2].model->Scale(ScaleNo);
	walls[3].model->Scale(ScaleNo);
	walls[4].model->Scale(ScaleNo);
	walls[5].model->Scale(ScaleNo);
	walls[6].model->Scale(ScaleNo);
	walls[7].model->Scale(ScaleNo);

	///////////////////////////////////////////////LARGETANKS////////////////////////////////////////////////////////
	vector<LargeTankStruct> largeTanks;
	
	for (int largeTankPos = 0; largeTankPos < kNumLargeTanks; largeTankPos++)
	{
		LargeTankStruct largeTank;
		largeTank.position = kLargeTankPositions[largeTankPos];
		largeTank.model = watertankOneMesh->CreateModel(largeTank.position.x, largeTank.position.y, largeTank.position.z);
		largeTanks.push_back(largeTank);
	}

	/////////////////////////////////////////////////SMALLTANKS///////////////////////////////////////////////////////
	vector<SmallTankStruct> smallTanks;

	for (int smallTankPos = 0; smallTankPos < kNumSmallTanks; smallTankPos++)
	{
		SmallTankStruct smallTank;
		smallTank.position = kSmallTankPositions[smallTankPos];
		smallTank.model = watertankTwoMesh->CreateModel(smallTank.position.x, smallTank.position.y, smallTank.position.z);
		smallTanks.push_back(smallTank);
	}

	smallTanks[0].model->RotateZ(AngleForty);


	/////////////////////////////////////////////TREES///////////////////////////////////////////////////////////////
	vector<TreeStruct> trees;

	for (int treePos = 0; treePos < kNumTrees; treePos++)
	{
		TreeStruct tree;
		tree.position = kTreePositions[treePos];
		tree.model = treeMesh->CreateModel(tree.position.x, tree.position.y, tree.position.z);
		trees.push_back(tree);
	}

	///////////////////////////////////Scaling of the trees//////////////////////
	trees[0].model->Scale(ScaleTen);
	trees[1].model->Scale(ScaleTen);
	trees[2].model->Scale(ScaleTen);
	trees[3].model->Scale(ScaleTen);
	trees[4].model->Scale(ScaleTen);
	trees[5].model->Scale(ScaleTen);
	trees[6].model->Scale(ScaleTen);
	trees[7].model->Scale(ScaleTen);

	/////////////////////////////////////////////Dummy-Non player racing car////////////////////////////////////
	vector<DummyLocStruct> dummies;

	for (int dummyPos = 0; dummyPos < kNumDummiesCar; dummyPos++)
	{
		DummyLocStruct dummy;
		dummy.position = kDummyPositions[dummyPos];
		
		dummy.model = dummyMesh->CreateModel(dummy.position.x, dummy.position.y, dummy.position.z);
//		dummies[dummyPos].state = EGameDummyCar::check1;
		dummies.push_back(dummy);
	}



	//////////////////////////////////////////////Camera////////////////////////////////////////////////////////
	IModel* dummyCamera = dummyMesh->CreateModel(0.0f, 0.0f, 0.0f);
	IModel* dummyHoverCar = dummyMesh->CreateModel(0.0f, 0.0f, 0.0f);

	ICamera* myCamera = myEngine->CreateCamera(kManual, 0.0f, cameraY, cameraZ);
	myCamera->AttachToParent(hoverCars[0].model);

	



	bool mouseCaptureActive = true; // state of mouse capture

	bool chaseCamActive = false;	// state of camera


	int mouseMoveX = 0; // for storing cumulative total of mouse movement on x axis
	int mouseMoveY = 0; // for storing cumulative total of mouse movement on y axis
	//float CameraAngle = 0.0f;


	//myCamera->AttachToParent(dummyCamera);           // The camera is attached to the dummy camera so that it move up and down

	float defaultCameraMatrix[16];
	myCamera->GetMatrix(defaultCameraMatrix);
	//dummyCamera->GetMatrix(defaultCameraMatrix);

	//////////////////////////////

	EGameMoveState presentState = Start;                  

	////////////////////////Fonts for the display//////////////////////////////////////
	IFont* myFont = myEngine->LoadFont(Font, FontOne);                    
	IFont* largeFont = myEngine->LoadFont(Font, FontTwo);
	IFont* checkpointFont = myEngine->LoadFont(Font, FontOne);
	IFont* hoverCarHealthFont = myEngine->LoadFont(Font, FontOne);


	/////////////////////////////////////////// 

	vector2D momentum{ 0.0f, 0.0f };
	vector2D thrust{ 0.0f, 0.0f };
	vector2D drag{ 0.0f, 0.0f };

	float matrix[4][4];
	const float thrustFactor = 0.075f;
	const float dragCoefficient = -0.001f;


	float CameraAngle;
	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();
		float deltaTime = myEngine->Timer();

							 
		if (presentState == Start)
		{
			if (!presentState == Start)                           // To restart the game after the game is over
			{
				presentState == EGameMoveState::Restart;
				StartPressed = false;
				
			}
			if (myEngine->KeyHit(SpaceKey))                     
			{
				StartPressed = true;
			}

			if (!StartPressed)                                  // when space Key is pressed the game will start in 3 secs. Till then one cannot move anuything inside the game
			{

				myFont->Draw(HitSpace, FontXOne, FontYOne, kRed, kCentre);

			}
			else
			{
				timer -= deltaTime;                                      // dialog flashes the 3 secs before the game starts 

				if (timer <= 0) 
				{
					presentState = Go;
				}
				else if (timer < One)
				{
					largeFont->Draw(TimerOne, FontXTwo, FontYTwo, kRed, kCentre);
				}
				else if (timer < Two)
				{
					largeFont->Draw(TimerTwo, FontXTwo, FontYTwo, kRed, kCentre);
				}
				else if (timer < Three)
				{
					largeFont->Draw(TimerThree, FontXTwo, FontYTwo, kRed, kCentre);
				}

			}
		}
		else if (presentState == Go)
		{
			

			// keyboard controlled camera movement
			if (myEngine->KeyHeld(cameraForward))                           // the camera moves forward at a given particular speed     
			{
				myCamera->MoveLocalZ(kCameraMove * deltaTime);
			}
			if (myEngine->KeyHeld(cameraBackward))                         // the camera moves backward at a given particular speed 
			{
				myCamera->MoveLocalZ(-kCameraMove * deltaTime);
			}
			if (myEngine->KeyHeld(cameraRight))                       // the camera moves right at a given particular speed 
			{
				myCamera->MoveLocalX(kCameraMove * deltaTime);
			} 
			if (myEngine->KeyHeld(cameraLeft))                        // the camera moves left at a given particular speed 
			{
				myCamera->MoveLocalX(-kCameraMove * deltaTime);  
			}
			// mouse-controlled camera movement when captured
			// it allows the camera move right and left but not up and down - it rotates but is not able to wriggle up and down with the mouse 
			if (mouseCaptureActive && !chaseCamActive)
			{
				mouseMoveX += myEngine->GetMouseMovementX(); // maintain cumulative total of movement
				mouseMoveY += myEngine->GetMouseMovementY();

				myCamera->ResetOrientation(); // reset the camera before moving... avoids nasty turning upside-down-ness
				myCamera->RotateLocalY(mouseMoveX * kMouseRotation);            // the MouseRotation reduces the rotation speed

				float mouseMoveWheel = myEngine->GetMouseWheelMovement();
				myCamera->MoveLocalZ(mouseMoveWheel * kMouseWheelMove);
			}

			if (myEngine->KeyHit(CameraPositionKey))      // The camera returns to its original position.
			{
				myCamera->DetachFromParent();
				myCamera->SetPosition(0.0f, cameraY, cameraZ);
				myCamera->AttachToParent(hoverCars[0].model);
	
			}
			if (myEngine->KeyHit(CameraFirstPersonKey))         // implementation of the first-person camera view
			{
				
				myCamera->SetPosition(hoverCars[0].model->GetX(), hoverCars[0].model->GetY(), hoverCars[0].model->GetZ());
				myCamera->MoveLocalZ(-1.0f);
				myCamera->MoveLocalY(6.0f);
				
			}

			// the car gently bobb;les up and down as it moves
			if (movingDown == true)       
			{
				hoverCars[0].model->MoveY(-SpeedY * deltaTime);     // moving down a certain speed
			}
			else
			{
				hoverCars[0].model->MoveY(SpeedY * deltaTime);       // moving up a certain speed
			}

			if (hoverCars[0].model->GetY() > SpeedJump)            // the hovercar should jump up and down a certain limit 
			{
				movingDown = true;
			}
			else if (hoverCars[0].model->GetY() < 0)         // the hovercar while bouncing shouldn't go inside the ground
			{
				movingDown = false;
			}

			// get facing vector
			hoverCars[0].model->GetMatrix(&matrix[0][0]);                               // &matrix gives the address of the matrix
			vector2D facingVector = { matrix[2][0], matrix[2][2] };

			// calculate thrust ( based on keyboard input )


			if (myEngine->KeyHeld(forwardKey))        // when the car is pressed forward by W, it applies thrust - thus applying the neccessary force needed to move the car
			{
				thrust = scalar(thrustFactor * deltaTime, facingVector);
			}
			else if (myEngine->KeyHeld(backwardKey))   // the same way the thrust is subtracted so that when presssed backward key S it moves back with the necessary force
			{
				thrust = scalar(-thrustFactor / 2* deltaTime, facingVector);     // the backward thrust is half the maximum forward thrust as per the gdd
			}
			else
			{
				thrust = { 0.0f, 0.0f }; 
			}	


			

			if (myEngine->KeyHeld(clockwiseKey))                    // the car turns clockwise a particular speed applying thrust
			{
				hoverCars[0].model->RotateY(kHoverSpeed * deltaTime);
				//currentRotation = -RotateSpeed * deltaTime;
				//hoverCars[0].model->RotateLocalZ(currentRotation);
				dummyHoverCar->RotateZ(RotateSpeed * deltaTime);
			}
			if (myEngine->KeyHeld(anticlockwiseKey))                  // the car turns anti-clockwise a particular speed applying thrust
			{ 
				hoverCars[0].model->RotateY(-kHoverSpeed * deltaTime);
				//currentRotation = RotateSpeed * deltaTime;
				//hoverCars[0].model->RotateLocalZ(currentRotation);
				dummyHoverCar->RotateZ(RotateSpeed * deltaTime);
			}

			if (Health = true)
			{
				// Implementation of the boost to make the hoverCar accelerate more quickly
				if (myEngine->KeyHeld(SpaceKey) && boostOverHeat == false)    
				{
					boost = true;
					boostTimer -= deltaTime;                                                 // timer needed for the boost to activate for only 3 secs, als0 the 1.2 sec will round to 1 sec
					thrust = scalar(thrustFactor * deltaTime * doubleThrust, facingVector);          // the thrust applied to the car is 50% greater when the boost activated

					if (boostTimer <= 0)                                                    // the boost will not be usable for a period of 5 secs after the boost is activated for 3 seconds
					{
						myFont->Draw(Wait, FontXThree, FontYThree, kRed, kCentre);
						boostOverHeat = true;
						drag.x *= 2;                                                        // the drag is doubled making the car decelerate quickly if the boost is active for long and it overheats
						drag.z *= 2;
					}
					else if (boostTimer < One)
					{
						myFont->Draw(BoostZero, FontXThree, FontYThree, kRed, kCentre);
					}
					else if (boostTimer < One)
					{
						myFont->Draw(BoostOne, FontXThree, FontYThree, kRed, kCentre);
					}
					else if (boostTimer < Two)
					{
						myFont->Draw(BoostTwo, FontXThree, FontYThree, kRed, kCentre);
					}
					else if (boostTimer > Three)
					{
						boost = false;
						myFont->Draw(BoostThree, FontXThree, FontYThree, kRed, kCentre);

					}


				}

				drag = scalar(dragCoefficient, momentum);
			
				if (boostOverHeat == true)        // if boost overheats then one cannot activate the boost for 5 seconds 
				{
					coolDowmTimer -= deltaTime;
					if (coolDowmTimer <= 0)
					{
						coolDowmTimer = TimerNo;
						boostTimer = TimerNo;
						boostOverHeat = false;
						boost = false;
					}

					drag.x *= Two;                 // the car decelerates quickly
					drag.z *= Two;

				}
			}


			////////////////////////////////Timer for the cross where it disappears after a short while when the player has passed throught the checkpoint////////////////
			for (int k = 0; k < kNumCrosses; k++)
			{
				if (crossTimer[k] <= 0)                                     // the cross when comes up when the car has crossed the checkpoint and the timer for the cross starts
				{
					crosses[k].model->SetY(CrossYOne);
				}
				else if (crossTimer[k] > 0)                                 // the cross is up for 5 secs and then goes down (disappears after a short while)       
				{
					crossTimer[k] -= deltaTime;
					crosses[k].model->SetY(CrossYTwo);
				}
			}

			// calculate momentum ( based on thrust, drag and previous momentum )

			momentum = sum3(momentum, thrust, drag);

			myFont->Draw(FPS + to_string(1 / deltaTime), FontXFour, FontYFour, kRed, kCentre);

			stringstream SpeedText;
			float dist = sqrt((momentum.x *momentum.x) + (momentum.z*momentum.z)); // 1 tl engine = 1m  // momentums vector length 
			float dist2 = sqrt((thrust.x *thrust.x) + (thrust.z*thrust.z));        // 1 tl engine = 1m
			
			int speed = static_cast<int>(abs(dist * kModel)* ConvertNo * (1 / deltaTime));      // the speed in kilometres per hour 
			SpeedText << Speed << speed;
			myFont->Draw(SpeedText.str(), FontXFive, FontYFive, kRed, kCentre);

			stringstream thrustText;                                           //  as per in the documentation in the gdd the maximum thrust force applied (length of thrust vector)
			thrustText << "thrust: "<< dist2 / deltaTime;
			myFont->Draw(thrustText.str(), 250, 400, kRed, kCentre);

			// Collision-HoverCaR and isle
			// the old x and z position needed for the calculation of the collison
			float oldX = hoverCars[0].model->GetX(); 
			float oldZ = hoverCars[0].model->GetZ();

			// move the hover car (according to new momentum)
			hoverCars[0].model->Move(momentum.x, 0.0f, momentum.z);

			/////////////////////////Non-player hover car/////////////////////////////
			if (CarCounter < kNumDummiesCar)
			{
				if (dummies[CarCounter].state == EGameDummyCar::check1)
				{													 
					CarTwoCounter++;								 
				}													 
				if (dummies[CarCounter].state == EGameDummyCar::check2)
				{													 
					CarTwoCounter++;								 
				}													 
				if (dummies[CarCounter].state == EGameDummyCar::check3)
				{													 
					CarTwoCounter++;								 
				}													 
				if (dummies[CarCounter].state == EGameDummyCar::check4)
				{													 
					CarTwoCounter++;								 
				}													 
				if (dummies[CarCounter].state == EGameDummyCar::check5)
				{
					CarTwoCounter++;
				}

			////////////////////use of array of dummy models which acts as waypoints around the track where the non-pplayer hover car look at the dummies and moves around the track

				hoverCars[1].model->MoveLocalZ(kHoverSpeed * deltaTime);
				hoverCars[1].model->LookAt(dummies[CarCounter].model);
				if (sphere2sphere(hoverCars[1].model->GetX(), hoverCars[1].model->GetZ(), kHoverCarRadius, dummies[CarCounter].model->GetX(), dummies[CarCounter].model->GetZ(), kDummyRadius))
				{
					CarCounter++;
				}
				if (CarLapCounter <= Two)                 // if the AI car wins comes first then it shows that the Car2 has won
				{ 
					
					if (CarCounter == kNumDummiesCar)
					{
						CarCounter = 0;
						CarLapCounter++;
					}

				}
				if (CarLapCounter == Two)
				{
					if (CarTwoCounter > CarOneCounter)
					{
						CarCounter = kNumDummiesCar;
						largeFont->Draw("Car2 Won", FontXTwo, FontYTwo, kRed, kCentre);
					}
					else
					{
						largeFont->Draw("Car1 Won", 640, 300, kRed, kCentre);
					}
				}
			}
			
			 // collision between player and the non-player hover car
			if (sphere2sphere(hoverCars[1].model->GetX(), hoverCars[1].model->GetZ(), kHoverCarRadius, hoverCars[0].model->GetX(), hoverCars[0].model->GetZ(), kHoverCarRadius))
			{
				momentum.x = -momentum.x;
				momentum.z = -momentum.z;
				hoverCars[0].model->Move(momentum.x, 0.0f, momentum.z);
			}



			///////////////////////////Collision between hovercar and the walls

			for (int j = 0; j < kNumWalls; j++)
			{

				EBoxSide collision = sphere2box(hoverCars[0].model->GetX(), hoverCars[0].model->GetZ(), oldX, oldZ, kHoverCarRadius, walls[j].model->GetX(), walls[j].model->GetZ(), wallWidth, wallDepth);



				if (collision == leftSide || collision == rightSide)
				{
					momentum.x = -momentum.x;
					hoverCars[0].model->Move(momentum.x, 0.0f, momentum.z);
					kHoverCarHealth--;
					break;

				}
				else if (collision == frontSide || collision == backSide)
				{
					momentum.z = -momentum.z;
					hoverCars[0].model->Move(momentum.x, 0.0f, momentum.z);
					kHoverCarHealth--;
					break;
				}

			}


			////Collision between hovercar and the checkpoint struts
			for (int k = 0; k < kNumCheckPoints; k++)
			{
				for (int m = 0; m < kNumStruts; m++)
				{
					if (sphere2sphere(hoverCars[0].model->GetX(), hoverCars[0].model->GetZ(), kHoverCarRadius, checkpoints[k].Strut[m]->GetX(), checkpoints[k].Strut[m]->GetZ(), kCheckPointsRadius))
					{
						momentum.x = -momentum.x;
						momentum.z = -momentum.z;
						hoverCars[0].model->Move(momentum.x, 0.0f, momentum.z);

						kHoverCarHealth--;
						break;
					}
				}
			}


			//////When the hovercar passes the checkpoints
			if (hoverCars[0].state == EGameHoverCar::checkpoint1)
			{
				checkpointFont->Draw(CheckPointOne, FontXOne, 650, kRed, kCentre);
				CarOneCounter++;
			}
			if (hoverCars[0].state == EGameHoverCar::checkpoint2)
			{
				checkpointFont->Draw(CheckPointTwo, FontXOne, FontYOne, kRed, kCentre);
				CarOneCounter++;
			}
			if (hoverCars[0].state == EGameHoverCar::checkpoint3)
			{
				checkpointFont->Draw(CheckPointThree, FontXOne, FontYOne, kRed, kCentre);
				CarOneCounter++;
			}
			if (hoverCars[0].state == EGameHoverCar::checkpoint4)
			{
				checkpointFont->Draw(CheckPointFour, FontXOne, FontYOne, kRed, kCentre);
				CarOneCounter++;
			}
			if (hoverCars[0].state == EGameHoverCar::checkpoint5)
			{
				checkpointFont->Draw(CheckPointFive, FontXOne, FontYOne, kRed, kCentre);
				CarOneCounter++;
			}

			////////////////////Collision between hovercar and the checkpoints/////////////////////
			for (int m = 0; m < kNumCheckPoints; m++)
			{

				EBoxSide collision = sphere2box(hoverCars[0].model->GetX(), hoverCars[0].model->GetZ(), oldX, oldZ, kHoverCarRadius, checkpoints[m].model->GetX(), checkpoints[m].model->GetZ(), kCheckpointWidth, kCheckpointDepth);

				if (collision == frontSide || collision == leftSide || collision == rightSide || collision == backSide)
				{

					switch (hoverCars[0].state)
					{
					case EGameHoverCar::start:
					{
						if (m == 0)
						{

							hoverCars[0].state = EGameHoverCar::checkpoint1;
							checkpointFont->Draw(CheckPointOne, FontXOne, FontYOne, kRed, kCentre);
							crossTimer[m] = TimerNo;                                                          // when the hover car passes the checkpoints the cross is triggered and it appears for 5 secs
							CarOneCounter++;
							

						}
						break;
					}

					case EGameHoverCar::checkpoint1:
					{
						if (m == One)
						{

							hoverCars[0].state = EGameHoverCar::checkpoint2;
							checkpointFont->Draw(CheckPointTwo, FontXOne, FontYOne, kRed, kCentre);
							crossTimer[m] = TimerNo;
							CarOneCounter++;
							


						}
						break;
					}

					case EGameHoverCar::checkpoint2:
					{
						if (m == Two)
						{
							hoverCars[0].state = EGameHoverCar::checkpoint3;
							checkpointFont->Draw(CheckPointThree, FontXOne, FontYOne, kRed, kCentre);
							crossTimer[m] = TimerNo;
							CarOneCounter++;
						
						}

						break;
					}

					case  EGameHoverCar::checkpoint3:
					{
						if (m == Three)
						{
							hoverCars[0].state = EGameHoverCar::checkpoint4;
							checkpointFont->Draw(CheckPointFour, FontXOne, FontYOne, kRed, kCentre);
							crossTimer[m] = TimerNo;
							CarOneCounter++;
							
						}

						break;
					}

					case EGameHoverCar::checkpoint4:
					{
						if (m == Four)
						{

							hoverCars[0].state = EGameHoverCar::start;
							checkpointFont->Draw(CheckPointFive, FontXSix, FontYSix, kRed, kCentre);
							crossTimer[m] = TimerNo;
							
						//	LapTrack = true;
							Lap++;
					       
							if (Lap >= Two)
							{
								presentState = EGameMoveState::Restart;
								LapTrack = true;
								CarOneCounter++;
								
							    if (CarOneCounter > CarTwoCounter)
							    {
							    	largeFont->Draw("Car1 Won", FontXTwo, FontYTwo, kRed, kCentre);
							    }
								else if (CarTwoCounter > CarOneCounter)
								{
									largeFont->Draw("Car2 Won", FontXTwo, FontYTwo, kRed, kCentre);
								}
							    
							}
							else
							{
								m = 0;
							}
							if (LapTrack == true)
							{
								Lap = One;
							}

						}

						break;
					}

			
					break;
					}

				}
			}
			myFont->Draw(CarLaps + to_string(Lap), FontXSeven, FontYSeven, kRed);            //the number of laps in the race 
			

			// Collision for hovercar and large tanks
			for (int n = 0; n < kNumLargeTanks; n++)
			{

				if (sphere2sphere(hoverCars[0].model->GetX(), hoverCars[0].model->GetZ(), kHoverCarRadius, largeTanks[n].model->GetX(), largeTanks[n].model->GetZ(), kLargeTankRadius))
				{
					momentum.x = -momentum.x;
					momentum.z = -momentum.z;
					hoverCars[0].model->Move(momentum.x, 0.0f, momentum.z);
					kHoverCarHealth--;
					break;
				}

			}

			// Collision for hovercar and small tanks
			for (int p = 0; p < kNumSmallTanks; p++)
			{

				if (sphere2sphere(hoverCars[0].model->GetX(), hoverCars[0].model->GetZ(), kHoverCarRadius, smallTanks[p].model->GetX(), smallTanks[p].model->GetZ(), kSmallTankRadius))
				{
					momentum.x = -momentum.x;
					momentum.z = -momentum.z;
					hoverCars[0].model->Move(momentum.x, 0.0f, momentum.z);
					kHoverCarHealth--;
					break;
				}

			}

			///collision for hovercar and trees
			for (int t = 0; t < kNumTrees; t++)
			{
				if (sphere2sphere(hoverCars[0].model->GetX(), hoverCars[0].model->GetZ(), kHoverCarRadius, trees[t].model->GetX(), trees[t].model->GetZ(), treeRadius))
				{
					momentum.x = -momentum.x;
					momentum.z = -momentum.z;
					hoverCars[0].model->Move(momentum.x, 0.0f, momentum.z);
					kHoverCarHealth--;
				}
			}
			

			// car health 
			hoverCarHealthFont->Draw(HealthCar + to_string(kHoverCarHealth), 100, 550, kRed);
			if (kHoverCarHealth <= TotalHealth)
			{
				boostOverHeat = false;
				boost = false;
			}
			else if (kHoverCarHealth <= LessHealth)
			{
				Health = false;
			}

		}
		else if (presentState == Restart) /// after the game is finished - one can restart the game
		{
			myFont->Draw(Enter, FontXOne, FontYOne, kRed, kCentre);
		    if (myEngine->KeyHeld(RestartKey))
		    {
				presentState = EGameMoveState::Start;
				StartPressed = false;
				LapTrack = false;
				hoverCars[0].model->SetX(0);
				hoverCars[0].model->SetZ(0);
				hoverCars[1].model->SetX(5);
				hoverCars[1].model->SetZ(0);
				momentum = {0.0f, 0.0f};
				thrust = { 0.0f, 0.0f };
				drag = { 0.0f, 0.0f };

				//hoverCars[0].model->RotateLocalZ(45);
				
			}
		}

		// Close the program
		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();
		}

		/**** Update your scene each frame here ****/

	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
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
		else if (oldspherezPos < minZ) result = frontSide;
		else result = backSide;
	}

	return(result);
}
