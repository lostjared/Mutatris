
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
        srand(static_cast<int>(time(0)));
        x = grid->width()/2;
        y = 0;
        for(int i = 0; i < 3; ++i) {
            blocks[i].x = grid->width()/2;
            blocks[i].y = i;
        }
        do {
            blocks[0].color = 1+(rand()%(grid->numBlocks()-1));
            blocks[1].color = 1+(rand()%(grid->numBlocks()-1));
            blocks[2].color = 1+(rand()%(grid->numBlocks()-1));
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

    Block *Piece::at(int index) {
        if(index >= 0 && index <= 2)
            return &blocks[index];
        return nullptr;
    }

    void Piece::moveLeft() {

        if(checkLocation(x-1,y)) {
            if(x > 0) {
                x --;
            }
        }
    }

		
    void Piece::moveRight() {
        if(checkLocation(x+1, y)) {
            if(x < grid->width()-1) {
                x ++;
            }
        }
    }

		
    void Piece::moveDown() {
        if(checkLocation(x, y+1)) {
            if(y < grid->height()-3) {
                y++;
            } else {
                setBlock();
            }
        } else {
            setBlock();
        }
    }

    void Piece::setCallback(Callback switch_) {
        switch_grid = switch_;
    }

    void Piece::setBlock() {
        Block *b[3];

        switch(direction) {
            case 0:
                b[0] = grid->at(x, y);
                b[1] = grid->at(x, y+1);
                b[2] = grid->at(x, y+2);
                break;
            case 1:
                b[0] = grid->at(x, y);
                b[1] = grid->at(x+1, y);
                b[2] = grid->at(x+2, y);
                break;
            case 2:
                b[0] = grid->at(x, y);
                b[1] = grid->at(x-1, y);
                b[2] = grid->at(x-2, y);
                break;
        }

        if(b[0] != nullptr &&  b[1] != nullptr && b[2] != nullptr) {
            b[0]->color = blocks[0].color;
            b[1]->color = blocks[1].color;
            b[2]->color = blocks[2].color;
            reset();
            switch_grid();
        }
    }

    bool Piece::checkLocation(int x, int y) {

        Block *b[3];
        switch(direction) {
            case 0:
                b[0] = grid->at(x, y);
                b[1] = grid->at(x, y+1);
                b[2] = grid->at(x, y+2);
                break;
            case 1:
                b[0] = grid->at(x, y);
                b[1] = grid->at(x+1, y);
                b[2] = grid->at(x+2, y);
                break;
            case 2:
                b[0] = grid->at(x, y);
                b[1] = grid->at(x-1, y);
                b[2] = grid->at(x-2, y);
                break;
        }

        if(b[0] != nullptr && b[1] != nullptr && b[2] != nullptr) {
            if(b[0]->color == 0 && b[1]->color == 0 && b[2]->color == 0)
                return true;
        }
        return false;
    }

    void Piece::shiftDirection() {
        int old_dir = direction;
        direction ++;
        if(direction > 2)
            direction = 0;

        if(!checkLocation(x, y)) {
            direction = old_dir;
        }
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
        grid[0].initGrid(12, (720/16/2)+1);
        grid[1].initGrid(12, 29);
        grid[2].initGrid(12, 720/16/2);
        grid[3].initGrid(12, 28);
    }

    void PuzzleGame::setCallback (Callback callback) {
        grid[0].game_piece.setCallback(callback);
        grid[1].game_piece.setCallback(callback);
        grid[2].game_piece.setCallback(callback);
        grid[3].game_piece.setCallback(callback);
    }


}