// Programming Assignment
// Tanvi Prakash Gavali - G20783550
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

const int row = 9;
const int column = 9;


class file {
// Access specifier
public: 
	// Variables to be used
	int map[row][column];
	int tempMap[row][column];

	// Methods to access function members
	void output();                            // Function member to display the map
	void readFile(ifstream &infile);          // Function member to read the file
	void mountainPass(int start);             // Function containing the Mountain pass
	void moveBacktoMap();                     // Function containing the temporary map
	void verticalFlip();                      // Function member to reflect the map vertically
	void diagonalFlip();                      // Function member to flip the diagonal from top-left to bottom-right
	void rotate();                            // Function member to rotate the map 90 degrees clockwise
	void floodFill(int x, int y, int depth);  // Function containing to create a lake - floodfill algorithm
	void hillClimbing(int x, int y, int height); // Function containing for the steepest ascent hill climbing
};

///////////////////////////////////FOR THE DISPLAY OF MAP TO SCREEN/////////////////////////////////////
void file::output()
{

	for (int y = 0; y < column; y++)
	{
		for (int x = 0; x < row; x++)
		{
			
			if (map[x][y] == 0)
			{
				cout << "000 ";    // The water location - map for the lake
			}
			else
			{
				cout << map[x][y] << " ";

			}

		}
		cout << endl;
	}
}

/////////////////////////////////////////////////READ A MAP FILE ////////////////////////////////////////////////////////
void file::readFile(ifstream &infile)  
{
	if (!infile)
	{
		cout << "Cannot open input file" << endl;
	}

	int r = 0;
	int c = 0;
	string in;
	infile >> noskipws;
	while (!infile.eof())
	{

		char ch;
		infile >> ch;
		if (!infile.eof())
		{
			if (ch == '\n')
			{
				if (in == "") 
				{
					in = "";
					r++;
					c = 0;
				}
				else
				{
					map[c][r] = stoi(in);     // Convert string to an int
					in = "";
					r++;
					c = 0;
				}
				
			}
			else if (ch != ' ')
			{
				in += ch;
			}
			else
			{
				map[c][r] = stoi(in);
				in = "";
				c++;
			}


		}

	}

}


///////////////////////////////MOUNTAIN PASS////////////////////////////////////////////
void file::mountainPass(int start)
{
	int lastx = start;

	cout << map[lastx][0] << " ";
	// The walker travels from top map to the bottom and take the easiest route possible
	for (int y = 0; y < column - 1; y++)
	{
		int smallest = lastx - 1;
		// The walker checks the lowest value from the default start location and cycles 
	    // through every row and choose the square which has the lowest value
		for (int x = lastx - 1; x <= lastx + 1; x++)
		{
			if (map[smallest][y + 1] > map[x][y + 1])
			{
				smallest = x;
			}
		}
		cout << map[smallest][y + 1] << " ";

		lastx = smallest;
	}

}

//////////////////////////////TEMPORARY MAP////////////////////////////////////////////
void file::moveBacktoMap()
{
	// Move the data back to the map array
	for (int x = 0; x < row; x++)
	{
		for (int y = 0; y < column; y++)
		{
			map[x][y] = tempMap[x][y];
		}
	}
}

////////////////////////////REFLECT THE MAP VERTICALLY////////////////////////////////////
void file::verticalFlip()
{
	// moved the data over to temp
	for (int x = 0; x < row; x++)
	{
		for (int y = 0; y < column; y++)
		{
			tempMap[x][y] = map[x][column - 1 - y];
		}
	}
	moveBacktoMap();
}

/////////////////////////////FLIP THE MAP DIAGONALLY/////////////////////////////////////
void file::diagonalFlip()
{
	for (int x = 0; x < row; x++)
	{
		for (int y = 0; y < column; y++)
		{
			tempMap[x][y] = map[y][x];
		}
	}
	moveBacktoMap();

}

///////////////////////////ROTATE THE MAP BY 90 DEGREES CLOCKWISE////////////////////////
void file::rotate()
{
	int numberLine = 8;
	// Transpose the map
	for (int r = 0; r < row; r++)
	{
		for (int c = 0; c < column; c++)
		{
			tempMap[r][c] = map[c][r];
		}
	}
	// Rotate 90 clockwise
	for (int r = 0; r < row; r++)
	{
		for (int c = 0; c < column; c++)
		{
			map[r][c] = tempMap[numberLine - r][c];
		}
	}
}


/////////////////////////CREATE LAKE//////////////////////////////////////////////
void file::floodFill(int x, int y, int depth)
{
	if (x < 0 || x >= row || y < 0 || y >= column)     // The search dont go beyond the row column limit
	{
		return;
	}

	if (map[x][y] == 0)     
	{
		return;
	}

	// It cover any aquare below the specified dept.
	// If the water hits a square bigger than the specifies depth then it stops
	if (map[x][y] > depth)  // The map coordinates shouldnt be bigger than the depth value
	{
		return;
	}
	else
	{
		map[x][y] = 0;
	}

	///// A recurrsive function /////// For up, down, right, left and diagonally 
	floodFill(x + 1, y, depth);
	floodFill(x - 1, y, depth);
	floodFill(x, y + 1, depth);
	floodFill(x, y - 1, depth);
	floodFill(x + 1, y + 1, depth);
	floodFill(x - 1, y - 1, depth);
	floodFill(x + 1, y - 1, depth);
	floodFill(x - 1, y + 1, depth);

	return;

}

/////////////////////////////STEEPEST ASCENT HILL CLIMBING/////////////////////////////////
// storing the coordinates of the map in the struct
struct Coordinates {
	int x;
	int y;
	bool visited = false;
	Coordinates* parent;
};

void file::hillClimbing(int x, int y, int height)
{
	vector <Coordinates*> openlist;                // The list is addded in the openlist for all the points it can move to
	vector <Coordinates*> closedlist;
	Coordinates* biggest = new Coordinates;        // Holds the memory where the value is
	
	biggest->x = x;
	biggest->y = y;
	biggest->visited = true;

	while (true)
	{
		
		// Check till you get the specific height or default height which is 410
		if (map[biggest->x][biggest->y] == height)
		{
			return;
		}

		Coordinates* temp = new Coordinates;

		/////////////////////////////Adding all the directions around the value to the list with parents///////////////////////////////////////////
		// If the value added is biggest than set the number as the biggest value from the possible values

		temp->x = biggest->x + 1;
		temp->y = biggest->y;
		if (temp->x < row)
		{
			bool found = false;
			for (int i = 0; i < openlist.size(); i++)
			{
				if (openlist[i]->x == temp->x && openlist[i]->y == temp->y)
				{

					found = true;
				}
			}

			if (!found)
			{
				temp->parent = biggest;
			openlist.push_back(temp);
			}
		}

		temp = new Coordinates;
		temp->x = biggest->x;
		temp->y = biggest->y + 1;
		if (temp->y < column)
		{
			bool found = false;
			for (int i = 0; i < openlist.size(); i++)
			{
				if (openlist[i]->x == temp->x && openlist[i]->y == temp->y)
				{
					found = true;
				}
			}

			if (!found)
			{
				temp->parent = biggest;

				openlist.push_back(temp);
			}
		}

		temp = new Coordinates;
		temp->x = biggest->x - 1;
		temp->y = biggest->y;
		if (temp->x >= 0)
		{
			bool found = false;
			for (int i = 0; i < openlist.size(); i++)
			{
				if (openlist[i]->x == temp->x && openlist[i]->y == temp->y)
				{
					found = true;
				}
			}

			if (!found)
			{
				temp->parent = biggest;

				openlist.push_back(temp);
			}
		}

		temp = new Coordinates;
		temp->x = biggest->x;
		temp->y = biggest->y - 1;
		if (temp->y >= 0)
		{
			bool found = false;
			for (int i = 0; i < openlist.size(); i++)
			{
				if (openlist[i]->x == temp->x && openlist[i]->y == temp->y)
				{
					found = true;
				}
			}

			if (!found)
			{
				temp->parent = biggest;

				openlist.push_back(temp);
			}
		}

		///////////////////////////// Checking of diagonally possible values ///////////////////////////
		temp = new Coordinates;
		temp->x = biggest->x + 1;
		temp->y = biggest->y + 1;
		if (temp->y < column && temp->x < row)
		{
			bool found = false;
			for (int i = 0; i < openlist.size(); i++)
			{
				if (openlist[i]->x == temp->x && openlist[i]->y == temp->y)
				{
					found = true;
				}
			}

			if (!found)
			{
				temp->parent = biggest;

				openlist.push_back(temp);
			}
		}

		temp = new Coordinates;
		temp->x = biggest->x - 1;
		temp->y = biggest->y - 1;
		if (temp->y >= 0 && temp->x >= 0)
		{
			bool found = false;
			for (int i = 0; i < openlist.size(); i++)
			{
				if (openlist[i]->x == temp->x && openlist[i]->y == temp->y)
				{
					found = true;
				}
			}

			if (!found)
			{
				temp->parent = biggest;
				temp->visited = true;

				openlist.push_back(temp);
			}
		}

		temp = new Coordinates;
		temp->x = biggest->x + 1;
		temp->y = biggest->y - 1;
		if (temp->y >= 0 && temp->x < row)
		{
			bool found = false;
			for (int i = 0; i < openlist.size(); i++)
			{
				if (openlist[i]->x == temp->x && openlist[i]->y == temp->y)
				{
					found = true;
				}
			}

			if (!found)
			{
				temp->parent = biggest;

				openlist.push_back(temp);
			}
		}

		temp = new Coordinates;
		temp->x = biggest->x - 1;
		temp->y = biggest->y + 1;
		if (temp->y < column && temp->x >= 0)
		{
			bool found = false;
			for (int i = 0; i < openlist.size(); i++)
			{
				if (openlist[i]->x == temp->x && openlist[i]->y == temp->y)
				{
					found = true;
				}
			}

			if (!found)
			{
				temp->parent = biggest;

				openlist.push_back(temp);
			}
		}

		// the value is visited and will go through all the possible values through the list skiping visited 
		// and will find the biggest from those possible values and use the biggest value ad the next point
		// adding adjacent that are not on the list with parent that is the current point 
		// and will loop through till the specific height is found
		biggest->visited = true;        
		int pos = 0;

		Coordinates* next = new Coordinates;

		for (int i = 0; i < openlist.size(); i++)
		{
			if (!openlist[i]->visited)
			{
			next = openlist[i];
			break;
			}
		}

		for (int i = 0; i < openlist.size(); i++)
		{
			if (map[next->x][next->y] < map[openlist[i]->x][openlist[i]->y] && !openlist[i]->visited)
			{
				next = new Coordinates;
				next = openlist[i];
				
			}
		}

		biggest = next;
	cout << map[biggest->x][biggest->y] << endl;
	}
}

int main()
{

	file map;   // Declare an object of class file
	cout << "Welcome" << endl;
	string selected;


	while (true)
	{
		cout << "Select one of the operations: " << endl;
		cout << endl;
		system("pause");
		system("cls");

		cout << "[1] Display the map " << endl;
		cout << "[2] Quit the program" << endl;
		cout << "[3] Mountain pass" << endl;
		cout << "[4] Rotate the map by 90 degrees clockwise" << endl;
		cout << "[5] Reflect the map vertically" << endl;
		cout << "[6] Flip map over the diagonal" << endl;
		cout << "[7] Create lake" << endl;
		cout << "[8] Steepest Ascent Hill Climbing" << endl;
		cout << endl;
		system("pause");
		system("cls");

		int numberOne = 1;
		int numberTwo = 2;
		int numberThree = 3;
		int numberFour = 4;
		int numberFive = 5;
		int numberSix = 6;
		int numberSeven = 7;
		int numberEight = 8;

		int menuInput;
		cin >> menuInput;
		if (menuInput == numberOne)            // Display of map
		{
			cout << "Choose Map:" << endl;     // Calls any of the three maps the user inputs
			cin >> selected;

			ifstream infile(selected);

			map.readFile(infile);  // accessing readFile member function to read the map 
			map.output();          // outputing the map
			infile.close();
		}
		else if (menuInput == numberTwo)      // Exit from the program
		{
			return 0;
		}
		else if (menuInput == numberThree)      // Mountain Pass
		{
			cout << "[1] Default" << endl;
			cout << "[2] User specify" << endl;
			int enterInput;
			cin >> enterInput;

			if (enterInput == numberOne)
			{
				int defaultx = 4;
			    ifstream infile(selected);
			    map.readFile(infile);
			    map.mountainPass(defaultx);                    // accessing member mountain pass function by default
			} 
			else if (enterInput == numberTwo)
			{

				ifstream infile(selected);
				map.readFile(infile);
				cout << "Choose location x: " << endl;         // Lets user enter coordinate x - the starting point for the mountain pass
				int xlocation;
				cin >> xlocation;
				map.mountainPass(xlocation);                   // accessing mountain pass member function choosing the square with lowest value
			}
		}
		else if (menuInput == numberFour)       // rotate the map 90 degrees clockwise
		{
			ifstream infile(selected);
			map.readFile(infile);  
			map.rotate();           // accessing rotate member function to rotate the map 90 degrees clockwise
			map.output();           // acceseeing the output member function displaying the rotation of the map
		}
		else if (menuInput == numberFive)       // Reflect the map vertically
		{
			ifstream infile(selected);
			map.readFile(infile);
			map.output();
			cout << endl;
			map.verticalFlip();      // accessing verticalFlip member function vertically reflecting
			map.output();            // acceseeing the output member function displaying the reflection of map vertically
		}
		else if (menuInput == numberSix)       // Flip map diagonally
		{
			ifstream infile(selected);
			map.readFile(infile);
			map.diagonalFlip();           // accessing diagonalFlip member function flipping the map diagonally from top-left to bottom-right
			map.output();                 // acceseeing the output member function displaying the rotation of the map
		}
		else if (menuInput == numberSeven)        // Lake - floodfill algorithm
		{
			cout << "[1] Default" << endl;
			cout << "[2] User specify" << endl;
			int enterInput;
			cin >> enterInput;

			if (enterInput == numberOne)              // It outputs the default value 
			{
				int defaultx = 4;
				int defaulty = 4;
				int defaultDepth = 300;
				ifstream infile(selected);
				map.floodFill(defaultx, defaulty, defaultDepth);    // accessing member floodfill function by default
				map.output();
			}
			else if (enterInput == numberTwo)           // It outputs the specified value given by the user for location x, y, and depth
			{

				ifstream infile(selected);
				cout << "Choose location x: " << endl;        // Lets user enter the coordinate x
				int xlocation;
				cin >> xlocation;
				cout << "Choose location y: " << endl;        // Lets user enter the coordinate y
				int ylocation;
				cin >> ylocation;
				cout << "Choose depth" << endl;                // Lets user enter the depth
				int userdepth;
				cin >> userdepth;
				map.floodFill(xlocation, ylocation, userdepth);  // accessing member floodfill function allowing user specification
				map.output();
			}

		}
		else if (menuInput == numberEight)       // Steepest ascent hill climbing
		{
			cout << "[1] Default" << endl;
			cout << "[2] User specify" << endl;
			int enterInput;
			cin >> enterInput;

			if (enterInput == numberOne)
			{
				int defaultx = 4;
				int defaulty = 0;
				int defaultHeight = 410;
			    ifstream infile(selected);
			    map.hillClimbing(defaultx, defaulty, defaultHeight); // accessing member hillClimbing function by default
			}
			else if (enterInput == numberTwo)                // It outputs the specified value given by the user for location x, y, and height
			{

				ifstream infile(selected);
				cout << "Choose location x: " << endl;         // Lets user enter the coordinate x
				int xlocation;
				cin >> xlocation;
				cout << "Choose location y: " << endl;         // Lets user enter the coordinate y
				int ylocation;
				cin >> ylocation;
				cout << "Choose height" << endl;              // Lets user enter the height
				int userheight;
				cin >> userheight;
				map.hillClimbing(xlocation, ylocation, userheight);    // accessing member hillClimbing function allowing user specification
			}
			
		}
		

	}
	system("pause");
}