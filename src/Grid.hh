#ifndef GRID_HH_
# define GRID_HH_

# include <vector>
# include <array>

# include <Shaders.hh>



struct Cell
{
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
  std::array<Cell, 9> _currentBlock;

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


  const std::array<Cell, 9> BLOCK_SQUARE = {{
    {false, _nbLines, _nbCol},
    {false, _nbLines, _nbCol},
    {false, _nbLines, _nbCol},
    {false, _nbLines, _nbCol},
    {true,  0, 2},
    {true,  0, 3},
    {true,  1, 2},
    {true,  1, 3},
    {false, _nbLines, _nbCol},
  }};

  const std::array<Cell, 9> BLOCK_BAR = {{
    {false, _nbLines, _nbCol},
    {false, _nbLines, _nbCol},
    {false, _nbLines, _nbCol},
    {false, _nbLines, _nbCol},
    {true,  0, 2},
    {true,  1, 2},
    {true,  2, 2},
    {true,  3, 2},
    {false, _nbLines, _nbCol},
  }};

  const std::array<Cell, 9> BLOCK_L = {{
    {false, _nbLines, _nbCol},
    {true, 0, 2},
    {false, _nbLines, _nbCol},
    {false, _nbLines, _nbCol},
    {true,  1, 2},
    {false, _nbLines, _nbCol},
    {false, _nbLines, _nbCol},
    {true,  2, 2},
    {true,  2, 3},
  }};

  const std::array<Cell, 9> BLOCK_R = {{
    {false, _nbLines, _nbCol},
    {true, 0, 2},
    {false, _nbLines, _nbCol},
    {false, _nbLines, _nbCol},
    {true,  1, 2},
    {false, _nbLines, _nbCol},
    {true,  2, 1},
    {true,  2, 2},
    {false, _nbLines, _nbCol},
  }};

  const std::array<Cell, 9> BLOCK_T = {{
    {false, _nbLines, _nbCol},
    {false, _nbLines, _nbCol},
    {false, _nbLines, _nbCol},
    {false, _nbLines, _nbCol},
    {true, 0, 2},
    {false, _nbLines, _nbCol},
    {true,  1, 1},
    {true,  1, 2},
    {true,  1, 3},
  }};

  const std::array<Cell, 9> BLOCK_C = {{
    {false, _nbLines, _nbCol},
    {false, _nbLines, _nbCol},
    {false, _nbLines, _nbCol},
    {true,  0, 2},
    {true,  0, 3},
    {false, _nbLines, _nbCol},
    {false, _nbLines, _nbCol},
    {true,  1, 3},
    {true,  1, 4},
  }};

  const std::array<Cell, 9> BLOCK_S = {{
    {false, _nbLines, _nbCol},
    {false, _nbLines, _nbCol},
    {false, _nbLines, _nbCol},
    {false, _nbLines, _nbCol},
    {true,  0, 2},
    {true,  0, 3},
    {true,  1, 1},
    {true,  1, 2},
  }};
};


#endif /* !GRID_HH_ */
