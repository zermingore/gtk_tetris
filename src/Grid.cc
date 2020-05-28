#include "Grid.hh"



Grid::Grid()
{
  for (auto i{0u}; i < _nbLines; ++i)
  {
    for (auto j{0u}; j < _nbCol; ++j)
    {
      _grid[i][j].occupied = false;
    }
  }
}



void Grid::draw()
{
  for (auto i{0u}; i < _nbLines; ++i)
  {
    for (auto j{0u}; j < _nbCol; ++j)
    {
      if (_grid[i][j].occupied)
      {
        drawCell(_grid[i][j]);
      }
    }
  }
}



void Grid::fall()
{
  // for (const auto &cell: _currentBlock)
  // {
  //   _grid[cell.line][cell.col].occupied = false;
  //   --cell.line;
  //   _grid[cell.line][cell.col].occupied = true;
  //   // TODO if (cell.line == 0) set next_block flag
  // }
}




void Grid::drawCell(const Cell &cell)
{
  if (!cell.occupied)
    return;

  // TODO proper transformation

  // float vertices[] = {
  //   -0.5f, -0.5f, 0.0f, // bottom left
  //   0.5f,  -0.5f, 0.0f, // bottom right
  //   0.5f,  0.5f,  0.0f, // top right
  //   -0.5f, 0.5f,  0.0f  // top left
  // };
}



// Grid::moveLeft / moveRight
// Grid::rotateLeft / rotateRight
