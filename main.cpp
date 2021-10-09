#pragma warning(disable:4996)
//Headers
#include <iostream>
#include <cstdlib>
#include <fstream>

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

//Function headers
void AllocateMaze(); 
void AllocateMazeVisited();
void GetMaze();
void GetVisited();
void DisplayMaze();
void DisplayVisitedMaze();
void AddCrumbs();
int depthFirstSearch(int row, int col);
void delMaze();

//Global Varibales
char **maze; //2D array for maze
int **visited; //Visited 2D array dor maze
int rows; // # of rows
int cols; // # of cols
char s_row; //String row size
char s_col; //String col size

//Enum for different maze states
enum mazechar {
	blank,
	wall,
	finish,
	crumb
};

//Dynamic memory allocation for 2D maze array;
void AllocateMaze() 
{
	//2d matrix
	maze = new char*[rows];

	for (int i = 0; i < rows; i++)
	{
		maze[i] = new char[cols];
	}
}

//Dynamic memory allocation for 2D maze array;
void AllocateMazeVisited()
{
	visited = new int*[rows];

	for (int i = 0; i < rows; i++)
	{
		visited[i] = new int[cols];
	}
}

//Reading the Maze from file (rows & cols)
void GetMaze()
{
	char c; //Used to iterate through the file's char(s)
	char rows_string[3] = { '\0' };
	char cols_string[3] = { '\0' };
	int rows_i = 0;
	int cols_i = 0;
	int swap = 0; //Swap from rows - cols
	char fileName[30]; //FileName no bigger than 30 char(s)

	//User can enter the maze name
	cout << "Please Enter Your FileName:" << endl;
	cin >> fileName;

	FILE* file = fopen(fileName, "r");

	//If theres an error opening file. Print error and exit program
	if (file == NULL) {
		printf("Error opening file...");
		exit(EXIT_FAILURE);
	}
	else
	{
		while ((c = getc(file)) != EOF)
		{
			//If new line break out
			if (c == '\n')
			{
				break;
			}
			//If Comma swap from rows - cols
			else if (c == ',')
			{
				swap = 1;
			}
			else if (swap == 0)
			{
				rows_string[rows_i] = c;
				rows_i++;
			}
			else
			{
				cols_string[cols_i] = c;
				cols_i++;
			}
		}
	}

	rows = atoi(rows_string);
	cols = atoi(cols_string);

	AllocateMaze();

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			c = getc(file);

			if (c == '\n') 
			{
				c = getc(file);
			}

			maze[i][j] = c;

			//Set the row and col start when 's' found
			if (c == 'S')
			{
				s_row = i;
				s_col = j;
			}
		}
	}
	//Close file after reading finished
	fclose(file);
}


//Used to loop through maze to check what chars 
//to see what enum val they hold.
void GetVisited() 
{
	AllocateMazeVisited();

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (maze[i][j] == '0')
			{
				visited[i][j] = wall;
			}
			else if (maze[i][j] == 'F')
			{
				visited[i][j] = finish;
			}
			else
			{
				visited[i][j] = blank;
			}
		}
	}
}


//Display the maze to console
void DisplayMaze() 
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			cout << maze[i][j];
		}
		cout << endl;
	}
	cout << endl;
}


//Display the visited maze to consle
void DisplayVisitedMaze()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			cout << visited[i][j];
		}
		cout << endl;
	}
	cout << endl;
}


//Deapth First Search implimentation (clockwise direction)
int depthFirstSearch(int row, int col)
{
	int* current = &visited[row][col];
	if (*current == finish) 
	{
		cout << "Found finish line" << endl;

		return 1;
	}

	if (*current == blank)
	{
		*current = crumb;
		//Check up
		if (depthFirstSearch(row, col - 1))
		{
			*current = crumb;
			return 1;
		}
		//check right
		if (depthFirstSearch(row +1, col))
		{
			*current = crumb;
			return 1;
		}
		//Check down
		if (depthFirstSearch(row, col + 1))
		{
			*current = crumb;
			return 1;
		}
		//Check left
		if (depthFirstSearch(row - 1, col))
		{
			*current = crumb;
			return 1;
		}
	}
	//Return true if hit wall
	return 0;
}

//Add crumbs / plot marked points
void AddCrumbs()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (maze[i][j] != 'S')
			{
				if (visited[i][j] == crumb)
				{
					maze[i][j] = '.';
				}
			}
		}
	}
}

void delMaze()
{
	cout << "Deleting maze memory..." << endl;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			delete &maze[rows][cols];
		}
	}
}


int main(int argc, char *argv[]) 
{
	char quit = ' ';

	while (quit != 'q')
	{

		int result = 0;
		cout << "====================================" << endl;
		cout << "Press '1' To Enter a Maze.txt:" << endl;
		cout << "Press '2' To Show Complete Maze:" << endl;
		cout << "Press '3' To Show Visited Maze:" << endl;
		cout << "Press '4' To Quit:" << endl;
		cout << "Press '5' To Delete maze memory:" << endl;
		cout << "====================================" << endl;

		cin >> result;

		switch (result)
		{
		case 1:
			//Get maze from file
			GetMaze();
			//Display maze
			DisplayMaze();

			getchar();
			break;
		case 2:
			//Get Visted (copy from maze)
			GetVisited();
			//Recursive function of DFS and adding crumbs
			depthFirstSearch(s_row, s_col);
			AddCrumbs();

			//RePrint the maze
			DisplayMaze();
			getchar();
			break;
		case 3:
			//Get Visted (copy from maze)
			GetVisited();
			//Recursive function of DFS and adding crumbs
			depthFirstSearch(s_row, s_col);
			AddCrumbs();

			//RePrint the maze
			DisplayVisitedMaze();
			getchar();
			break;
		case 4:
			quit = 'q';
			printf("Quitting...");
			exit(EXIT_FAILURE); //Quit out of program
			break;
		case 5:
			delMaze();
			break;
		default:
			getchar();
			break;
		}
	}


	getchar();

	return 0;
}