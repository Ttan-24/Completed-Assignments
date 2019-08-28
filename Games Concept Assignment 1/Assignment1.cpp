// Assignment1.cpp: A program using the TL-Engine
// Tanvi Prakash Gavali  email- TPGavali@uclan.ac.uk
#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;


void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "C:\\Tanvi\\Assignment1\\media" );

	/**** Set up your scene here ****/
	IMesh* floorMesh;
	IModel* floor;
	floorMesh = myEngine->LoadMesh("floor.x");
	floor = floorMesh->CreateModel();
	IMesh* sphereOneMesh;
	IModel* sphereOne;
	sphereOneMesh = myEngine->LoadMesh("ball.x");
	sphereOne = sphereOneMesh->CreateModel(-50,10,0);
	IMesh* sphereTwoMesh;
	IModel* sphereTwo;
	sphereTwoMesh = myEngine->LoadMesh("ball.x");
	sphereTwo = sphereTwoMesh->CreateModel(50,10,50);
	IMesh* sphereThreeMesh;
	IModel* sphereThree;
	sphereThreeMesh = myEngine->LoadMesh("ball.x");
	sphereThree = sphereThreeMesh->CreateModel(25, 10 ,0);
	IMesh* sphereFourMesh;
	IModel* sphereFour;
	sphereFourMesh = myEngine->LoadMesh("ball.x");
	sphereFour = sphereFourMesh->CreateModel(25, 10, 100);
	ICamera* myCamera;
	myCamera = myEngine->CreateCamera(kManual);
	myCamera ->SetPosition(0, 120, -200);
	myCamera->RotateLocalX(20);

	string skin1 = "blue.png";                                                   // declared and assigned to cycle the sphere skin in clockwise direction
	string skin2 = "violet.png";
	string skin3 = "indigo.png";
	string skin4 = "fawn.png";


	sphereOne->SetSkin(skin1);
	sphereTwo->SetSkin(skin2);
	sphereThree->SetSkin(skin3);
	sphereFour->SetSkin(skin4);

	const float kGameSpeed = 0.02f;                                             

	float sphereOnezCoordinate;
	float sphereTwozCoordinate;
	float sphereThreezCoordinate;
	float sphereFourzCoordinate;
	float sphereOneyCoordinate;
	float sphereTwoyCoordinate;
	float sphereThreeyCoordinate;
	float sphereFouryCoordinate;
	float sphereOnexCoordinate;
	float sphereTwoxCoordinate;
	float sphereThreexCoordinate;
	float sphereFourxCoordinate;
	
	
	float sphereSpeed = 1.0f;
	float rotationSpeed = 1.0f;
	
	float mouseMoveZ = 0.0f;   
	bool pause = false;
	bool reverse = false;
	

	enum directions {right,left,up,down};                                        //To make the movement of spheres in the clockwise and anti-clockwise ditrection
	directions sphereOnedirection = up;
	directions sphereTwodirection = down;
	directions sphereThreedirection = left;
	directions sphereFourdirection = right;
	directions sphereOneReversedirection = down;
	directions sphereTwoReversedirection = up;
	directions sphereThreeReversedirection = right;
	directions sphereFourReversedirection = left;


	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();
		sphereOnezCoordinate = sphereOne->GetZ();                               // To store the current location of the sphere
		sphereTwozCoordinate = sphereTwo->GetZ();
		sphereThreezCoordinate = sphereThree->GetZ();
		sphereFourzCoordinate = sphereFour->GetZ();
		sphereOneyCoordinate = sphereOne->GetY();
		sphereTwoyCoordinate = sphereTwo->GetY();
		sphereThreeyCoordinate = sphereThree->GetY();
		sphereFouryCoordinate = sphereFour->GetY();
		sphereOnexCoordinate = sphereOne->GetX();
		sphereTwoxCoordinate = sphereTwo->GetX();
		sphereThreexCoordinate = sphereThree->GetX();
		sphereFourxCoordinate = sphereFour->GetX();

		mouseMoveZ = myEngine->GetMouseWheelMovement();                          // Called for scrolling the mouse up and down

		/**** Update your scene each frame here ****/
		if (myEngine->KeyHit(Key_Escape))                                        // Escape key to quit the game
		{
			myEngine->Stop();
		}
		if (pause == false)                                                      // To pause and resume the movement of the spheres
		{
			if (reverse == false)                                                // To reverse the movement of spheres in an anti-clockwise direction
			{
				if (sphereOnedirection == up)                                    // Sphere1                     // Spheres Move in clockwise direction 
				{
					sphereOne->MoveZ(sphereSpeed * kGameSpeed);
					sphereOne->RotateZ(sphereSpeed * kGameSpeed * rotationSpeed);
				}
				else if (sphereOnedirection == right)
				{
					sphereOne->MoveX(sphereSpeed * kGameSpeed);
					sphereOne->RotateX(sphereSpeed * kGameSpeed * rotationSpeed);
				}
				else if (sphereOnedirection == down)
				{
					sphereOne->MoveZ(-(sphereSpeed * kGameSpeed));                             
					sphereOne->RotateZ(sphereSpeed * kGameSpeed * rotationSpeed);
				}
				else if (sphereOnedirection == left)
				{                                                                      
					sphereOne->MoveX(-(sphereSpeed * kGameSpeed));
					sphereOne->RotateX(sphereSpeed * kGameSpeed * rotationSpeed);
				}




				if (sphereOnezCoordinate > 100)                                          
				{
					sphereOne->SetZ(100);                                                //set back to correct coordinate
					sphereOnedirection = right;
					sphereOneReversedirection = left;                                    // you call the sphereOneReversedirection to not confuse which reverse it means when you tell the program to reverse, this 
              																			//  basically means when the spheredirection goes right then its reverse (opposite) is left
				}
				if (sphereOnexCoordinate > 50)
				{
					sphereOne->SetX(50);
					sphereOnedirection = down;
					sphereOneReversedirection = up;
				}
				if (sphereOnezCoordinate < 0)
				{
					sphereOne->SetZ(0);
					sphereOnedirection = left;
					sphereOneReversedirection = right;
				}
				if (sphereOnexCoordinate < -50)
				{
					sphereOne->SetX(-50);
					sphereOnedirection = up;
					sphereOneReversedirection = down;
				}


				if (sphereTwodirection == up)                                              //Sphere2
				{
					sphereTwo->MoveZ(sphereSpeed * kGameSpeed);
					sphereTwo->RotateZ(sphereSpeed * kGameSpeed * rotationSpeed);
				}
				else if (sphereTwodirection == right)
				{
					sphereTwo->MoveX(sphereSpeed * kGameSpeed);
					sphereTwo->RotateX(sphereSpeed * kGameSpeed * rotationSpeed);
				}
				else if (sphereTwodirection == down)
				{
					sphereTwo->MoveZ(-(sphereSpeed * kGameSpeed));
					sphereTwo->RotateZ(sphereSpeed * kGameSpeed * rotationSpeed);
				}
				else if (sphereTwodirection == left)
				{
					sphereTwo->MoveX(-(sphereSpeed * kGameSpeed));
					sphereTwo->RotateX(sphereSpeed * kGameSpeed * rotationSpeed);
				}


				if (sphereTwozCoordinate > 100)                                                     
				{                                                                                    
					sphereTwo->SetZ(100);
					sphereTwodirection = right;
					sphereTwoReversedirection = left;
				}
				if (sphereTwoxCoordinate > 50)
				{
					sphereTwo->SetX(50);
					sphereTwodirection = down;
					sphereTwoReversedirection = up;
				}
				if (sphereTwozCoordinate < 0)
				{
					sphereTwo->SetZ(0);
					sphereTwodirection = left;
					sphereTwoReversedirection = right;
				}
				if (sphereTwoxCoordinate < -50)
				{
					sphereTwo->SetX(-50);
					sphereTwodirection = up;
					sphereTwoReversedirection = down;
				}




				if (sphereThreedirection == up)                                             //Sphere3
				{
					sphereThree->MoveZ(sphereSpeed * kGameSpeed);
					sphereThree->RotateZ(sphereSpeed * kGameSpeed * rotationSpeed);
				}
				else if (sphereThreedirection == right)
				{
					sphereThree->MoveX(sphereSpeed * kGameSpeed);
					sphereThree->RotateX(sphereSpeed * kGameSpeed * rotationSpeed);
				}
				else if (sphereThreedirection == down)
				{
					sphereThree->MoveZ(-(sphereSpeed * kGameSpeed));
					sphereThree->RotateZ(sphereSpeed * kGameSpeed * rotationSpeed);
				}
				else if (sphereThreedirection == left)
				{
					sphereThree->MoveX(-(sphereSpeed * kGameSpeed));
					sphereThree->RotateX(sphereSpeed * kGameSpeed * rotationSpeed);
				}




				if (sphereThreezCoordinate > 100)
				{
					sphereThree->SetZ(100);
					sphereThreedirection = right;
					sphereThreeReversedirection = left;
				}
				if (sphereThreexCoordinate > 50)
				{
					sphereThree->SetX(50);
					sphereThreedirection = down;
					sphereThreeReversedirection = up;
				}
				if (sphereThreezCoordinate < 0)
				{
					sphereThree->SetZ(0);
					sphereThreedirection = left;
					sphereThreeReversedirection = right;
				}
				if (sphereThreexCoordinate < -50)
				{
					sphereThree->SetX(-50);
					sphereThreedirection = up;
					sphereThreeReversedirection = down;
				}



				if (sphereFourdirection == up)                                                          // Sphere4
				{
					sphereFour->MoveZ(sphereSpeed * kGameSpeed);
					sphereFour->RotateZ(sphereSpeed * kGameSpeed * rotationSpeed);
				}
				else if (sphereFourdirection == right)
				{
					sphereFour->MoveX(sphereSpeed * kGameSpeed);
					sphereFour->RotateX(sphereSpeed * kGameSpeed * rotationSpeed);
				}
				else if (sphereFourdirection == down)
				{
					sphereFour->MoveZ(-(sphereSpeed * kGameSpeed));
					sphereFour->RotateZ(sphereSpeed * kGameSpeed * rotationSpeed);
				}
				else if (sphereFourdirection == left)
				{
					sphereFour->MoveX(-(sphereSpeed * kGameSpeed));
					sphereFour->RotateX(sphereSpeed * kGameSpeed * rotationSpeed);
				}




				if (sphereFourzCoordinate > 100)
				{
					sphereFour->SetZ(100);
					sphereFourdirection = right;
					sphereFourReversedirection = left;
				}
				if (sphereFourxCoordinate > 50)
				{
					sphereFour->SetX(50);
					sphereFourdirection = down;
					sphereFourReversedirection = up;
				}
				if (sphereFourzCoordinate < 0)
				{
					sphereFour->SetZ(0);
					sphereFourdirection = left;
					sphereFourReversedirection = right;
				}
				if (sphereFourxCoordinate < -50)
				{
					sphereFour->SetX(-50);
					sphereFourdirection = up;
					sphereFourReversedirection = down;
				}

			}
			else
			{
				// REVERSE CODE


				if (sphereOneReversedirection == right)                                                     // Reverse Sphere1
				{
					sphereOne->MoveX(sphereSpeed * kGameSpeed);
					sphereOne->RotateZ(sphereSpeed * kGameSpeed * rotationSpeed);
				}
				else if (sphereOneReversedirection == up)
				{
					sphereOne->MoveZ(sphereSpeed * kGameSpeed);
					sphereOne->RotateX(sphereSpeed * kGameSpeed * rotationSpeed);
				}
				else if (sphereOneReversedirection == left)
				{
					sphereOne->MoveX(-(sphereSpeed * kGameSpeed));
					sphereOne->RotateZ(sphereSpeed * kGameSpeed * rotationSpeed);
				}
				else if (sphereOneReversedirection == down)
				{
					sphereOne->MoveZ(-(sphereSpeed * kGameSpeed));
					sphereOne->RotateX(sphereSpeed * kGameSpeed * rotationSpeed);
				}




				if (sphereOnexCoordinate > 50)                                    // The sphereOnedirection is called to not confuse the program 
				{
					sphereOne->SetX(50);
					sphereOneReversedirection = up;
					sphereOnedirection = down;
				}
				if (sphereOnezCoordinate > 100)
				{
					sphereOne->SetZ(100);
					sphereOneReversedirection = left;
					sphereOnedirection = right;
				}
				if (sphereOnexCoordinate < -50)
				{
					sphereOne->SetX(-50);
					sphereOneReversedirection = down;
					sphereOnedirection = up;
				}
				if (sphereOnezCoordinate < 0)
				{
					sphereOne->SetZ(0);
					sphereOneReversedirection = right;
					sphereOnedirection = left;
				}





				if (sphereTwoReversedirection == right)                                                    //Reverse Sphere2
				{
					sphereTwo->MoveX(sphereSpeed * kGameSpeed);
					sphereTwo->RotateZ(sphereSpeed * kGameSpeed * rotationSpeed);
				}
				else if (sphereTwoReversedirection == up)
				{
					sphereTwo->MoveZ(sphereSpeed * kGameSpeed);
					sphereTwo->RotateX(sphereSpeed * kGameSpeed * rotationSpeed);
				}
				else if (sphereTwoReversedirection == left)
				{
					sphereTwo->MoveX(-(sphereSpeed * kGameSpeed));
					sphereTwo->RotateZ(sphereSpeed * kGameSpeed * rotationSpeed);
				}
				else if (sphereTwoReversedirection == down)
				{
					sphereTwo->MoveZ(-(sphereSpeed * kGameSpeed));
					sphereTwo->RotateX(sphereSpeed * kGameSpeed * rotationSpeed);
				}




				if (sphereTwozCoordinate > 100)
				{
					sphereTwo->SetZ(100);
					sphereTwoReversedirection = left;
					sphereTwodirection = right;

				}
				if (sphereTwoxCoordinate > 50)
				{
					sphereTwo->SetX(50);
					sphereTwoReversedirection = up;
					sphereTwodirection = down;
				}
				if (sphereTwozCoordinate < 0)
				{
					sphereTwo->SetZ(0);
					sphereTwoReversedirection = right;
					sphereTwodirection = left;
				}
				if (sphereTwoxCoordinate < -50)
				{
					sphereTwo->SetX(-50);
					sphereTwoReversedirection = down;
					sphereTwodirection = up;
				}



				if (sphereThreeReversedirection == right)                                                   //Reverse Sphere3
				{
					sphereThree->MoveX(sphereSpeed * kGameSpeed);
					sphereThree->RotateZ(sphereSpeed * kGameSpeed * rotationSpeed);
				}
				else if (sphereThreeReversedirection == up)
				{
					sphereThree->MoveZ(sphereSpeed * kGameSpeed);
					sphereThree->RotateX(sphereSpeed * kGameSpeed * rotationSpeed);
				}
				else if (sphereThreeReversedirection == left)
				{
					sphereThree->MoveX(-(sphereSpeed * kGameSpeed));
					sphereThree->RotateZ(sphereSpeed * kGameSpeed * rotationSpeed);
				}
				else if (sphereThreeReversedirection == down)
				{
					sphereThree->MoveZ(-(sphereSpeed * kGameSpeed));
					sphereThree->RotateX(sphereSpeed * kGameSpeed * rotationSpeed);
				}




				if (sphereThreexCoordinate > 50)                                    
				{
					sphereThree->SetX(50);
					sphereThreeReversedirection = up;
					sphereThreedirection = down;
				}
				if (sphereThreezCoordinate > 100)
				{
					sphereThree->SetZ(100);
					sphereThreeReversedirection = left;
					sphereThreedirection = right;
				}
				if (sphereThreexCoordinate < -50)
				{
					sphereThree->SetX(-50);
					sphereThreeReversedirection = down;
					sphereThreedirection = up;
				}
				if (sphereThreezCoordinate < 0)
				{
					sphereThree->SetZ(0);
					sphereThreeReversedirection = right;
					sphereThreedirection = left;
				}




				if (sphereFourReversedirection == right)                                                    //Reverse Sphere4
				{
					sphereFour->MoveX(sphereSpeed * kGameSpeed);
					sphereFour->RotateZ(sphereSpeed * kGameSpeed * rotationSpeed);
				}
				else if (sphereFourReversedirection == up)
				{
					sphereFour->MoveZ(sphereSpeed * kGameSpeed);
					sphereFour->RotateX(sphereSpeed * kGameSpeed * rotationSpeed);
				}
				else if (sphereFourReversedirection == left)
				{
					sphereFour->MoveX(-(sphereSpeed * kGameSpeed));
					sphereFour->RotateZ(sphereSpeed * kGameSpeed * rotationSpeed);
				}
				else if (sphereFourReversedirection == down)
				{
					sphereFour->MoveZ(-(sphereSpeed * kGameSpeed));
					sphereFour->RotateX(sphereSpeed * kGameSpeed * rotationSpeed);
				}




				if (sphereFourxCoordinate > 50)                                    
				{
					sphereFour->SetX(50);
					sphereFourReversedirection = up;
					sphereFourdirection = down;
				}
				if (sphereFourzCoordinate > 100)
				{
					sphereFour->SetZ(100);
					sphereFourReversedirection = left;
					sphereFourdirection = right;
				}
				if (sphereFourxCoordinate < -50)
				{
					sphereFour->SetX(-50);
					sphereFourReversedirection = down;
					sphereFourdirection = up;
				}
				if (sphereFourzCoordinate < 0)
				{
					sphereFour->SetZ(0);
					sphereFourReversedirection = right;
					sphereFourdirection = left;
				}


			}
				
		

			if ((mouseMoveZ > 0) && (sphereSpeed <= 5))                    // The player can increase and decrease the speed by scrolling the mouse up and down
			{
					sphereSpeed += kGameSpeed / 10;	
			}
			if ((mouseMoveZ < 0) && (sphereSpeed >= kGameSpeed / 2))
			{
				sphereSpeed -= kGameSpeed / 10;
			}


			// The player can raise the spheres in the Y-axis while moving in clockwise and anti-clockwise direction if arrow up is pressed on the keyboard

			if (myEngine->KeyHeld(Key_Up) && sphereOneyCoordinate < 40 && sphereTwoyCoordinate < 40 && sphereThreeyCoordinate < 40 && sphereFouryCoordinate < 40) 
			{
				sphereOne->MoveY(1.0f*kGameSpeed);
				sphereTwo->MoveY(1.0*kGameSpeed);
				sphereThree->MoveY(1.0f*kGameSpeed);
				sphereFour->MoveY(1.0f*kGameSpeed);
				
			}


			// The players can lower the spheres in the Y-axis while moving in clockwise and anti-clockwise direction if arrow down is presssd on the keyboard

			if (myEngine->KeyHeld(Key_Down) && sphereOneyCoordinate > 10 && sphereTwoyCoordinate > 10 && sphereThreeyCoordinate > 10 && sphereFouryCoordinate >10)
			{
				sphereOne->MoveY(-(1.0f*kGameSpeed));
				sphereTwo->MoveY(-(1.0*kGameSpeed));
				sphereThree->MoveY(-(1.0*kGameSpeed));
				sphereFour->MoveY(-(1.0*kGameSpeed));
			}


			// The spheres stop rotating if they leave the ground and start rotating if they touch the ground.
			if (sphereOneyCoordinate > 10)
			{
				rotationSpeed = 0;
			}
			else
			{
				rotationSpeed = 1.0f;
			}


			if (myEngine->KeyHit(Key_R))          // When Key R is hit then the spheres move in an anti-clockwise direction
			{
				reverse = !reverse;               // to switch between clockwise and anti-clockwise 
			}

			if (myEngine->KeyHit(Key_E))          // When Key E is hit then the skin color of each sphere cycles clockwise by taking the skin of the sphere next to it in the clockwise direction

			{
				string test = skin1;                       // test to blue
				skin1 = skin4;                             // blue to fawn
				skin4 = skin2;                             // fawn to violet
				skin2 = skin3;                             // violet to indigo
				skin3 = test;                              // indigo to test       // string test is called so that the program does not get confused when taking the skin of the sphere next to it
				

				sphereOne->SetSkin(skin1);
				sphereTwo->SetSkin(skin2);
				sphereThree->SetSkin(skin3);
				sphereFour->SetSkin(skin4);
				
			}
		}


		if (myEngine->KeyHit(Key_P))             // The player can hit the Key P as a toggle to pause and resume the forward and backward movement of spheres
		{
			pause = !pause;                      // to switch between pause and resume 
		}

		

	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
