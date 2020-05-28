#ifndef GRID_HH_
# define GRID_HH_

# include <vector>

struct Cell;



class Grid
{
public:
  Grid();

  ~Grid() = default;

  void draw();

  void fall();


  auto operator[] (unsigned int column) { return _grid[column]; }


private:
  void drawCell(const Cell &cell);


  const unsigned int _nbCol = 10;
  const unsigned int _nbLines = 18;

  std::vector<std::vector<Cell>> _grid;
  std::vector<std::vector<Cell>> _currentBlock;
};



struct Cell
{
  bool occupied;
  unsigned int col;
  unsigned int line;
};


#endif /* !GRID_HH_ */
