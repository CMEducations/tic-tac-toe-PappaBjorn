#include "pch.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>

#define gridSize 3

class Game
{
	enum class Player
	{
		empty = '-',
		player = 'X',
		ai = 'O'
	};

	struct Move
	{
		unsigned int x = 0;
		unsigned int y = 0;
	};

	Player board[gridSize][gridSize];

public:
	Game()
	{
		for (unsigned int i = 0; i < 3; i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				board[i][j] = Player::empty;
			}
		}
	}

	void printBoard()
	{
		std::cout << "+-----------------+";
		for (unsigned int i = 0; i < gridSize; i++)
		{
			std::cout << "\n|";
			for (unsigned int j = 0; j < gridSize; j++)
			{
				std::cout << std::setw(gridSize) << static_cast<char>(board[i][j]) << std::setw(gridSize) << " |";
			}
		}
		std::cout << "\n+-----------------+\n";
	}

	bool isTie()
	{
		for (unsigned int i = 0; i < gridSize; i++)
		{
			if (board[i][0] == Player::empty || board[i][1] == Player::empty || board[i][2] == Player::empty)
			{
				return false;
			}
		}
		return true;
	}

	bool checkWin(Player player)
	{
		// Check for row, columns and diagonals
		for (unsigned int i = 0; i < gridSize; i++)
		{
			if (board[i][0] == player && board[i][1] == player && board[i][2] == player)
			{
				return true;
			}

			if (board[0][i] == player && board[1][i] == player && board[2][i] == player)
			{
				return true;
			}
		}

		if (board[0][0] == player && board[1][1] == player && board[2][2] == player)
		{
			return true;
		}

		if (board[0][2] == player && board[1][1] == player && board[2][0] == player)
		{
			return true;
		}

		return false;
	}

	Move minimax()
	{
		int score = std::numeric_limits<int>::max();
		Move move;

		for (unsigned int i = 0; i < gridSize; i++)
		{
			for (unsigned int j = 0; j < gridSize; j++)
			{
				if (board[i][j] == Player::empty)
				{
					board[i][j] = Player::ai;

					int temp = maxSearch();

					if (temp < score)
					{
						score = temp;
						move.x = i;
						move.y = j;
					}

					board[i][j] = Player::empty;
				}
			}
		}

		return move;
	}

	int maxSearch()
	{
		if (checkWin(Player::player)) { return 10; }
		else if (checkWin(Player::ai)) { return -10; }
		else if (isTie()) { return 0; }

		int score = std::numeric_limits<int>::min();

		for (unsigned int i = 0; i < gridSize; i++)
		{
			for (unsigned int j = 0; j < gridSize; j++)
			{
				if (board[i][j] == Player::empty)
				{
					board[i][j] = Player::player;
					score = std::max(score, minSearch());
					board[i][j] = Player::empty;
				}
			}
		}

		return score;
	}

	int minSearch()
	{
		if (checkWin(Player::player)) { return 10; }
		else if (checkWin(Player::ai)) { return -10; }
		else if (isTie()) { return 0; }

		int score = std::numeric_limits<int>::max();

		for (unsigned int i = 0; i < gridSize; i++)
		{
			for (unsigned int j = 0; j < gridSize; j++)
			{
				if (board[i][j] == Player::empty)
				{
					board[i][j] = Player::ai;
					score = std::min(score, maxSearch());
					board[i][j] = Player::empty;
				}
			}
		}

		return score;
	}

	void getHumanMove()
	{
		bool fail = true;
		unsigned int x = -1, y = -1;

		do
		{
			std::cout << "Your move: ";

			char c;
			std::cin >> c;
			x = c - '0';
			std::cin >> c;
			y = c - '0';

			fail = board[x][y] != Player::empty;

			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		} while (fail);

		board[x][y] = Player::player;
	}

	void play()
	{
		unsigned int turn = 0;
		bool exit = false;

		printBoard();
		std::cout << "Enter your move in coordinate form[row, col]. ex: 02\n";

		do
		{
			// human move
			if (turn == 0)
			{
				getHumanMove();

				if (checkWin(Player::player))
				{
					std::cout << "You win!\n";
					exit = true;
				}
			}
			else
			{
				std::cout << "\nAI move: ";

				Move aimove = minimax();
				std::cout << aimove.x << aimove.y << "\n";
				board[aimove.x][aimove.y] = Player::ai;

				if (checkWin(Player::ai))
				{
					std::cout << "AI wins!\n";
					exit = true;
				}
			}

			if (isTie())
			{
				std::cout << "\n*** It's a tie! ***\n";
				exit = true;
			}

			turn ^= 1;
			printBoard();

		} while (!exit);
	}
};

int main()
{
	Game tictactoe;
	tictactoe.play();
	std::cin.ignore();
}