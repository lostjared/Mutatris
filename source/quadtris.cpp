
#include"quadtris.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>


namespace puzzle {

    Block::Block(): x{0}, y{0}, color{0} {}


    Block::Block(int x, int y, int color) {
        this->x = x;
        this->y = y;
        this->color =  color;
    }

    Block::Block(const Block &b)  :x{b.x}, y{b.y}, color{b.color} {

    }
    Block::Block(Block&& b) :x{b.x}, y{b.y}, color{b.color} {

    }

	Block &Block::operator=(const Block &b) {
        color = b.color;
        x = b.x;
        y =  b.y;
        return *this;
    }
	
    Block &Block::operator=(Block&& b) {
        color = b.color;
        x = b.x;
        y =  b.y;
        return *this;
    }

	bool Block::operator==(const Block &b) const {
        if(b.color == color && b.x == x  && b.y == y)
            return true;

        return false;
    }
	
    bool Block::operator==(const int &color) const {
        if(this->color == color)
            return true;
        return false;
    }

    Piece::Piece(GameGrid *g) : x{0}, y{0}, grid{g} {}
	
    void Piece::reset() {
        x = grid->width()/2;
        y = 0;
        for(int i = 0; i < 3; ++i) {
            blocks[i].x = grid->width()/2;
            blocks[i].y = i;
        }
        do {
            blocks[0].color = rand()%grid->numBlocks();
            blocks[1].color = rand()%grid->numBlocks();
            blocks[2].color = rand()%grid->numBlocks();
        } while(blocks[0].color == blocks[1].color && blocks[0].color == blocks[2].color);
    }

    void Piece::shiftColors() {
        Block b[3];
        b[0] = blocks[0];
        b[1] = blocks[1];
        b[2] = blocks[2];
        blocks[0] = b[2];
        blocks[1] = b[0];
        blocks[2] = b[1];
    }

    GameGrid::GameGrid()  : blocks{nullptr}, grid_w{0}, grid_h{0}, game_piece{this} {
    }

    GameGrid::~GameGrid() {
        releaseGrid();
    }
    
    void GameGrid::releaseGrid() {
        if(blocks != nullptr) {
            std::cout << "Mutatris: Releasing Grid...\n";
            for(int i = 0; i < grid_w; ++i) {
                delete [] blocks[i];
            }
            delete [] blocks;
            blocks = nullptr;
        }
    }

    void GameGrid::initGrid(int size_x, int size_y) {
        if(blocks != nullptr)
            releaseGrid();

        blocks = new Block*[size_x];
        for(int i = 0; i < size_x; ++i) {
            blocks[i] = new Block[size_y];
        }
        grid_w = size_x;
        grid_h = size_y;
        game_piece.reset();
    }
    
    Block *GameGrid::at(int x, int y) {
        if(x >= 0  && x < grid_w && y >= 0 && y < grid_h && blocks != nullptr) {
            return &blocks[x][y];
        }
        return nullptr;
    }

    int GameGrid::width() const {
        return grid_w;
    }

    int GameGrid::height() const {
        return grid_h;
    }

    PuzzleGame::PuzzleGame() {
        grid[0].initGrid(8, 17);
        grid[1].initGrid(8, 17);
        grid[2].initGrid(8, 17);
        grid[3].initGrid(8, 17);
    }


}