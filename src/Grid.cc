#include <random>
#include <iostream>

#include <gtk/gtk.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Grid.hh>



Grid::Grid()
  : _shader("src/shader.vs", "src/shader.fs")
{
  glm::mat4 projection = glm::mat4(1.0f);
  projection = glm::ortho(0.0f, 360.0f, 720.0f, 0.0f, -1.0f, 1.0f);
  _shader.setMat4("projection", projection);
  _shader.setMat4("view", glm::mat4(1.0f));

  // Instantiate the cells
  for (auto i{0u}; i < _nbLines; ++i)
  {
    std::vector<Cell> col(_nbCol);
    _grid.push_back(col);
  }

  // Initialize the Cells
  for (auto i{0u}; i < _nbLines; ++i)
  {
    for (auto j{0u}; j < _nbCol; ++j)
    {
      _grid[i][j].occupied = false;
      _grid[i][j].line = i;
      _grid[i][j].col = j;
    }
  }

  newBlock();
}



void Grid::draw()
{
  glClear(GL_COLOR_BUFFER_BIT);

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

  for (const auto &cell: _currentBlock)
  {
    drawCell(cell);
  }
}



bool Grid::fall()
{
  auto touched_down {false};
  auto line_completed {false};

  for (const auto &cell: _currentBlock)
  {
    /* Check for touch down: bottom reached or landing on an occupied cell */
    if (cell.line == _nbLines - 1 || _grid[cell.line + 1][cell.col].occupied)
    {
      touched_down = true;

      for (auto &c: _currentBlock)
      {
        _grid[c.line][c.col].occupied = true;
      }

      line_completed = checkLineCompleted();
      break;
    }
  }

  // game over check
  if (touched_down && !line_completed)
  {
    for (const auto cell: _currentBlock)
    {
      if (cell.line == 0)
      {
        std::cout << "Game Over" << std::endl;
        return false;
      }
    }
  }

  if (touched_down)
  {
    newBlock();
    draw();
    return true;
  }

  for (auto &cell: _currentBlock)
  {
    ++cell.line; // first line: top
  }
  ++_blockMiddle.line;

  return true;
}



void Grid::newBlock()
{
  _blockMiddle = {true, 0, _nbCol / 2};

  // Random generator initialization
  static std::random_device r;
  static std::default_random_engine engine(r());
  static std::uniform_int_distribution<int> distribution(0, 6);
  _currentBlockType = distribution(engine);
  _currentBlockRotation = 0;
  _currentBlock = _blocks[_currentBlockType][0];

  for (auto &cell: _currentBlock)
  {
    cell.line += _blockMiddle.line;
    cell.col += _blockMiddle.col;
  }
}



void Grid::drawCell(const Cell &cell)
{
  glm::mat4 model = glm::mat4(1.0f); // *must* be initialized to identity matrix

  float x = cell.col * _cellSizeX;
  float y = cell.line * _cellSizeY;

  // TODO view && projection matrices computation in the constructor
  model = glm::translate(model, glm::vec3(x, y, 0.f));
  model = glm::scale(model, glm::vec3(_cellSizeX, _cellSizeY, 0.f));
  _shader.setMat4("model", model);

  unsigned int vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(_blkVertices), _blkVertices, GL_STREAM_DRAW);

  unsigned int vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  unsigned int ebo;
  glGenBuffers(1, &ebo);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_blkVerticesIdx), _blkVerticesIdx, GL_STREAM_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}



void Grid::moveLeft()
{
  for (const auto &cell: _currentBlock)
  {
    if (cell.col <= 0 || _grid[cell.line][cell.col - 1].occupied)
    {
      return;
    }
  }

  for (auto &cell: _currentBlock)
  {
    --cell.col;
  }
  --_blockMiddle.col;
}



void Grid::moveRight()
{
  for (const auto &cell: _currentBlock)
  {
    if (cell.col >= _nbCol - 1 || _grid[cell.line][cell.col + 1].occupied)
    {
      return;
    }
  }

  for (auto &cell: _currentBlock)
  {
    ++cell.col;
  }
  ++_blockMiddle.col;
}



bool Grid::checkLineCompleted()
{
  auto ret {false};

  for (auto i{_nbLines - 1}; i > 0; --i)
  {
    auto line_complete {true};
    for (auto j{0u}; j < _nbCol; ++j)
    {
      if (!_grid[i][j].occupied)
      {
        line_complete = false;
        break;
      }
    }

    if (line_complete)
    {
      ret = true;
      for (auto k{i}; k > 0; --k)
      {
        for (auto j{0u}; j < _nbCol; ++j)
        {
          _grid[k][j].occupied = _grid[k - 1][j].occupied;
        }
      }
      ++i;
    }
  }

  return ret;
}




void Grid::rotateLeft()
{
  auto restore_rotation {_currentBlockRotation};

  ++_currentBlockRotation;
  _currentBlockRotation %= _blocks[_currentBlockType].size();

  std::array<Cell, 4> next = _blocks[_currentBlockType][_currentBlockRotation];
  std::array<Cell, 4> tmp;
  for (auto i{0u}; i < tmp.size(); ++i)
  {
    tmp[i].occupied = true;
    tmp[i].line = _blockMiddle.line + next[i].line;
    tmp[i].col = _blockMiddle.col + next[i].col;
  }

  for (const auto &cell: tmp)
  {
    if (cell.line > _nbLines - 1 || cell.col > _nbCol - 1)
    {
      _currentBlockRotation = restore_rotation;
      return;
    }
  }

  std::copy(std::begin(tmp), std::end(tmp), std::begin(_currentBlock));
}



void Grid::rotateRight()
{
  auto restore_rotation {_currentBlockRotation};

  --_currentBlockRotation;
  if (_currentBlockRotation < 0)
    _currentBlockRotation = _blocks[_currentBlockType].size() - 1;

  std::array<Cell, 4> next = _blocks[_currentBlockType][_currentBlockRotation];
  std::array<Cell, 4> tmp;
  for (auto i{0u}; i < tmp.size(); ++i)
  {
    tmp[i].occupied = true;
    tmp[i].line = _blockMiddle.line + next[i].line;
    tmp[i].col = _blockMiddle.col + next[i].col;
  }

  for (const auto &cell: tmp)
  {
    if (cell.line > _nbLines - 1 || cell.col > _nbCol - 1)
    {
      std::cout << "aborting rotation" << std::endl;
      _currentBlockRotation = restore_rotation;
      return;
    }
  }

  std::copy(std::begin(tmp), std::end(tmp), std::begin(_currentBlock));
}
