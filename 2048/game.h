#include <iostream>
#include <random>
#include <chrono>
#include <numeric>
#include <iomanip>
#include <string>
#include <vector>

class Game
{
public:
	Game();
	~Game();

	// delete copy & move functionality -> avoid copying bugs
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	Game(const Game&&) = delete;
	Game&& operator=(const Game&&) = delete;

	// inline functions
	inline std::string ifZero(const int& val) const;
	inline void instruct() const;
	inline void refresh() const;

	// non-inline functions
	int digitInt(const int&) const;
	void directionToLogic(const char& direction);
	void display() const;
	void gameStart();
	void input();
	void Logic(const int&, const int&, const int&, const int&);
	int randomGenerate();
	int randomGenerateTwoOrFour();

private:
	// prepare random generation
	// LCE Engine
	std::default_random_engine randEngine;
	// uniform distribution
	std::uniform_int_distribution<int> disEngine;
	// gameBoard
	int board[17];
};