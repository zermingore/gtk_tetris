#ifndef GRID_HH_
# define GRID_HH_

# include <vector>

# include <Shaders.hh>



struct Cell;



class Grid
{
public:
  Grid();

  ~Grid() = default;

  void draw();

  void fall();
  void newBlock();

  void moveLeft();
  void moveRight();


private:
  auto operator[] (unsigned int column) { return _grid[column]; }

  void drawCell(const Cell &cell);


  const unsigned int _nbLines = 18;
  const unsigned int _nbCol = 10;

  // TODO hard-coded screen size
  const float _cellSizeX = 360.f / _nbCol;
  const float _cellSizeY = 720.f / _nbLines;

  std::vector<std::vector<Cell>> _grid;
  std::vector<Cell> _currentBlock;

  Shaders _shader;
};



struct Cell
{
  bool occupied;
  unsigned int line;
  unsigned int col;
};


#endif /* !GRID_HH_ */
