#ifndef PUZZLE_BLOCK_H
#define PUZZLE_BLOCK_H
#include <array>
#include <vector>

namespace puzzle {

	class Block {
	public:
		int x,y,color;
		Block();
		Block(int x, int y, int color);
		Block(const Block &b);
		Block(Block &&b);
		Block &operator=(const Block &b);
		Block &operator=(Block &&b);
		bool operator==(const Block &b) const;
		bool operator==(const int &c) const;
	};


	class GameGrid  {
	public:
		GameGrid();
		~GameGrid();
		void initGrid(int size_x, int size_y);
		void releaseGrid();
		Block *at(int x, int y);
	protected:
		Block **blocks = nullptr;
		int grid_w = 0, grid_h = 0;
	};

	class PuzzleGame  {
	public:
		PuzzleGame();

	protected:
		GameGrid grid[4];
	};

}

#endif
