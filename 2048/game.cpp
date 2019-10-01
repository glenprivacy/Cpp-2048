#include "game.h"

// either print the non-zero value or a blank
inline std::string Game::ifZero(const int& val) const { return (val > 0 ? std::to_string(val) : " "); }
inline void Game::instruct() const { std::cout << "Plz use w/s/a/d to control the movement of the gameboard" << std::endl; };
// refresh the console, which works well in both linux and windows
inline void Game::refresh() const { if (system("CLS")) system("clear"); };

Game::Game() : board{ 0 }, randEngine(static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count())), disEngine(1, 16)
{
	std::cout << "Game Start!" << std::endl;
}

Game::~Game()
{
	std::cout << "Game Over!" << std::endl;
}

int Game::digitInt(const int& origin) const
{
	if (origin < 10)
		return 1;
	else if (origin < 100)
		return 2;
	else if (origin < 1000)
		return 3;
	else
		return 4;
}

void Game::directionToLogic(const char& direction)
{
	switch (direction)
	{
	case 'w':
		Logic(1, 1, 1, 4);
		break;
	case 'a':
		Logic(1, 4, 1, 1);
		break;
	case 's':
		Logic(13, 1, -1, 4);
		break;
	case 'd':
		Logic(4, 4, -1, 1);
		break;
	default:
		break;
	}
}

void Game::display() const
{
	std::cout << "-----------------------------" << std::endl;
	for (int i = 1; i < 17; i++)
	{
		std::string temp;
		switch (digitInt(board[i]))
		{
		case 1:
			temp += "|   ";
			temp += ifZero(board[i]);
			temp += "  ";
			break;
		case 2:
			temp += "|  ";
			temp += ifZero(board[i]);
			temp += "  ";
			break;
		case 3:
			temp += "|  ";
			temp += ifZero(board[i]);
			temp += " ";
			break;
		case 4:
			temp += "| ";
			temp += ifZero(board[i]);
			temp += " ";
			break;
		default:
			break;
		}
		std::cout << temp;
		if (i % 4 == 0)
		{
			std::cout << '|' << std::endl;
			std::cout << "-----------------------------" << std::endl;
		}
	}
}


void Game::gameStart()
{
	instruct();
	// for the opening gameboard
	for(int i = 0; i < 2; i++)
	{
		bool hasFilled = false;
		while (!hasFilled)
		{
			auto key = randomGenerate();
			if (board[key] == 0)
			{
				board[key] = randomGenerateTwoOrFour();
				hasFilled = true;
			}
		}
	}
	// first display
	display();

	// gameLoop
	while(true)
	{
		bool gameOver = true, changed = false;
		int i;
		int boardTemp[17];
		for (i = 0; i < 17; i++)
			boardTemp[i] = board[i];

		// ask for valid input
		input();
		
		for (i = 0; i < 17; i++)
		{
			// if there is any changes to the gameBoard
			if (boardTemp[i] != board[i])
				changed = true;
			// as long as there is a full gameBoard after control, the game is stuck, i.e. the player loses
			if (boardTemp[i] == 0)
				gameOver = false;

		}
		// if gameOver
		if (gameOver)
			return;
		// if there is no change
		if (!changed)
		{
			instruct();
			continue;
		}
		bool hasFilled = false;
		// randomly pick a empty block
		while (!hasFilled)
		{
			// control first, then display, so the new block could appear after the control state
			auto key = randomGenerate();
			if (board[key] == 0)
			{
				// init a new board with 2 or 4
				board[key] = randomGenerateTwoOrFour();
				hasFilled = true;
			}
		}
		refresh();
		display();
	}
}

void Game::input()
{
	char direction;
	while (true)
	{
		if (std::cin >> direction)
			break;
		else
			instruct();
	}
	directionToLogic(direction);
}


void Game::Logic(const int& idx, const int& increment, const int& sign, const int& add)
{
	// for every row
	for (int j = 0; j <= 3; j++)
	{
		int i = idx + (increment * j);
		std::vector<int> vec;
		for (int j = 0; j <= 3; ++j)
			if (board[i + (sign * add * j)] != 0)
				vec.push_back(i + (sign * add * j));

		if (vec.empty())
			continue;
		// after confirming that there is value(s) in this line
		else if (vec.size() == 1)
		{
			// if the block is the first one(to-do)
			if (vec[0] != i)
			{
				board[i] = board[vec[0]];
				board[vec[0]] = 0;
			}
		}
		else if (vec.size() == 2)
		{
			// if they have the same value
			if (board[vec[0]] == board[vec[1]])
			{
				board[i] = 2 * board[vec[0]];
				board[i + (sign * add)] = 0;
				board[i + (sign * add) * 2] = 0;
				board[i + (sign * add) * 3] = 0;
			}
			else
			{
				board[i] = board[vec[0]];
				board[i + (sign * add)] = board[vec[1]];
				board[i + (sign * add) * 2] = board[i + (sign * add) * 3] = 0;
			}
		}
		else if (vec.size() == 3)
		{
			// if the first two blocks have the same value
			if (board[vec[0]] == board[vec[1]])
			{
				board[i] = 2 * board[vec[0]];
				board[i + (sign * add)] = board[vec[2]];
				board[i + (sign * add) * 2] = board[i + (sign * add) * 3] = 0;
			}
			// if the last two blocks have the same value
			else if (board[vec[1]] == board[vec[2]])
			{
				board[i] = board[vec[0]];
				board[i + (sign * add)] = 2 * board[vec[1]];
				board[i + (sign * add) * 2] = board[i + (sign * add) * 3] = 0;
			}
			else
			{
				board[i] = board[vec[0]];
				board[i + (sign * add)] = board[vec[1]];
				board[i + (sign * add) * 2] = board[vec[2]];
				board[i + (sign * add) * 3] = 0;
			}
		}
		else if (vec.size() == 4)
		{
			// if the first two blocks have the same value
			if (board[vec[0]] == board[vec[1]])
			{
				board[i] = 2 * board[vec[0]];
				// if the last two blocks also have the same value 
				if (board[vec[2]] == board[vec[3]])
				{
					board[i + (sign * add)] = 2 * board[vec[2]];
					board[i + (sign * add) * 2] = board[i + (sign * add) * 3] = 0;
				}
				else
				{
					board[i + (sign * add)] = board[vec[2]];
					board[i + (sign * add) * 2] = board[vec[3]];
					board[i + (sign * add) * 3] = 0;
				}
			}
			// if the mid two blocks have the same value
			else if (board[vec[1]] == board[vec[2]])
			{
				board[i] = board[vec[0]];
				board[i + (sign * add)] = 2 * board[vec[1]];
				board[i + (sign * add) * 2] = board[vec[3]];
				board[i + (sign * add) * 3] = 0;
			}
			// if the last two blocks have the same value
			else if (board[vec[2]] == board[vec[3]])
			{
				board[i] = board[vec[0]];
				board[i + (sign * add)] = board[vec[1]];
				board[i + (sign * add) * 2] = 2 * board[vec[2]];
				board[i + (sign * add) * 3] = 0;
			}
			// else the line is full and unable to be compressed
			else
				continue;
		}
	}
}

int Game::randomGenerate()
{
	return disEngine(randEngine);
}

int Game::randomGenerateTwoOrFour()
{
	if ((disEngine(randEngine) % 2) == 0)
		return 2;
	else
		return 4;
}