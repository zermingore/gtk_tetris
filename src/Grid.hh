#ifndef GRID_HH_
# define GRID_HH_

# include <vector>
# include <array>

# include <Shaders.hh>



class Cell
{
public:
  Cell() = default;

  Cell(bool o, unsigned int l, unsigned int c)
    : occupied(o)
    , line(l)
    , col(c)
  {}

  Cell(unsigned int l, unsigned int c)
    : occupied(true)
    , line(l)
    , col(c)
  {}

  bool occupied;
  unsigned int line;
  unsigned int col;
};


class Grid
{
public:
  Grid();

  ~Grid() = default;

  void draw();

  bool fall();
  void newBlock();

  void moveLeft();
  void moveRight();

  void rotateLeft();
  void rotateRight();


private:
  auto operator[] (unsigned int column) { return _grid[column]; }

  void drawCell(const Cell &cell);
  bool checkLineCompleted();


  const unsigned int _nbLines = 18;
  const unsigned int _nbCol = 10;

  // TODO hard-coded screen size
  const float _cellSizeX = 360.f / _nbCol;
  const float _cellSizeY = 720.f / _nbLines;

  std::vector<std::vector<Cell>> _grid;
  std::array<Cell, 4> _currentBlock;
  int _currentBlockType;
  int _currentBlockRotation;
  Cell _blockMiddle;

  Shaders _shader;


  const float _blkVertices[16] = {
    0.f, 0.f, 0.0f, // bottom left
    1.f, 0.f, 0.0f, // bottom right
    1.f, 1.f, 0.0f, // top right
    0.f, 1.f, 0.0f  // top left
  };

  const unsigned int _blkVerticesIdx[6] = {
    0, 1, 3,
    1, 2, 3
  };


  const std::vector<std::vector<std::array<Cell, 4>>> _blocks
  {
    { // BLOCK_SQUARE
      {{ {0, 0}, {0, 1}, {1, 0}, {1, 1}, }}
    },

    { // BLOCK_BAR
      {{ {0, 0}, {1, 0}, {2, 0}, {3, 0}, }},
      {{ {0, 0}, {0, 1}, {0, 2}, {0, 3}, }},
    },

    { // BLOCK_L
      {{ {0, 0}, {1, 0}, {2, 0}, {2, 1}, }},
      {{ {1, 0}, {1, 1}, {1, 2}, {0, 2}, }},
      {{ {0, 0}, {0, 1}, {1, 1}, {2, 1}, }},
      {{ {0, 0}, {1, 0}, {0, 1}, {0, 2}, }}
    },

    { // BLOCK_J
      {{ {0, 1}, {1, 1}, {2, 1}, {2, 0}, }},
      {{ {0, 0}, {0, 1}, {0, 2}, {1, 2}, }},
      {{ {0, 0}, {0, 1}, {1, 1}, {2, 1}, }},
      {{ {0, 0}, {1, 0}, {0, 1}, {0, 2}, }}
    },

    { // BLOCK_T
      {{ {0, 1}, {1, 0}, {1, 1}, {1, 2}, }},
      {{ {0, 1}, {1, 0}, {1, 1}, {2, 1}, }},
      {{ {0, 0}, {0, 1}, {0, 2}, {1, 1}, }},
      {{ {0, 0}, {1, 0}, {1, 1}, {2, 0}, }}
    },

    { // BLOCK_Z
      {{ {0, 0}, {0, 1}, {1, 1}, {1, 2} }},
      {{ {0, 1}, {1, 0}, {1, 1}, {2, 0} }}
    },

    { // BLOCK_S
      {{ {0, 1}, {0, 2}, {1, 0}, {1, 1}, }},
      {{ {0, 0}, {1, 0}, {1, 1}, {2, 1}, }}
    },
  };
};


#endif /* !GRID_HH_ */
