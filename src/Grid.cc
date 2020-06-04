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
  glClearColor(.95, .95, .95, 0);
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
    if (cell.occupied)
    {
      drawCell(cell);
    }
  }
}



bool Grid::fall()
{
  auto touched_down {false};
  auto line_completed {false};

  for (const auto &cell: _currentBlock)
  {
    if (!cell.occupied)
      continue;

    /* Check for touch down: bottom reached or landing on an occupied cell */
    if (cell.line == _nbLines - 1 || _grid[cell.line + 1][cell.col].occupied)
    {
      touched_down = true;

      for (auto &c: _currentBlock)
      {
        if (!c.occupied)
          continue;

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
    if (!cell.occupied)
      continue;

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
  switch (_currentBlockType)
  {
    case 0:
      _currentBlock = BLOCK_SQUARE[0];
      break;

    case 1:
      _currentBlock = BLOCK_BAR[0];
      break;

    case 2:
      _currentBlock = BLOCK_L[0];
      break;

    case 3:
      _currentBlock = BLOCK_J[0];
      break;

    case 4:
      _currentBlock = BLOCK_T[0];
      break;

    case 5:
      _currentBlock = BLOCK_Z[0];
      break;

    case 6:
      _currentBlock = BLOCK_S[0];
      break;

    default:
      std::cerr << "Unexpected random value [" << _currentBlockType << "]\n";
      _currentBlock = BLOCK_SQUARE[0];
      break;
  }

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
    if (!cell.occupied)
      continue;

    if (cell.col <= 0 || _grid[cell.line][cell.col - 1].occupied)
    {
      return;
    }
  }

  for (auto &cell: _currentBlock)
  {
    if (!cell.occupied)
      continue;

    --cell.col;
  }
  --_blockMiddle.col;
}



void Grid::moveRight()
{
  for (const auto &cell: _currentBlock)
  {
    if (!cell.occupied)
      continue;

    if (cell.col >= _nbCol - 1 || _grid[cell.line][cell.col + 1].occupied)
    {
      return;
    }
  }

  for (auto &cell: _currentBlock)
  {
    if (!cell.occupied)
      continue;

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
  std::cout << "rotate left" << std::endl;
  std::array<Cell, 4> next;

  switch (_currentBlockType)
  {
    case 0:
      _currentBlockRotation = (_currentBlockRotation + 1) % BLOCK_SQUARE.size();
      next = BLOCK_SQUARE[_currentBlockRotation];
      break;

    case 1:
      _currentBlockRotation = (_currentBlockRotation + 1) % BLOCK_BAR.size();
      next = BLOCK_BAR[_currentBlockRotation];
      break;

    case 2:
      _currentBlockRotation = (_currentBlockRotation + 1) % BLOCK_L.size();
      next = BLOCK_L[_currentBlockRotation];
      _currentBlock = BLOCK_L[0];
      break;

    case 3:
      _currentBlockRotation = (_currentBlockRotation + 1) % BLOCK_J.size();
      next = BLOCK_J[_currentBlockRotation];
      break;

    case 4:
      _currentBlockRotation = (_currentBlockRotation + 1) % BLOCK_T.size();
      next = BLOCK_T[_currentBlockRotation];
      break;

    case 5:
      _currentBlockRotation = (_currentBlockRotation + 1) % BLOCK_Z.size();
      next = BLOCK_Z[_currentBlockRotation];
      break;

    case 6:
      _currentBlockRotation = (_currentBlockRotation + 1) % BLOCK_S.size();
      next = BLOCK_S[_currentBlockRotation];
      break;

    default:
      std::cerr << "Unexpected _currentBlockType value [" << _currentBlockType << "]\n";
      _currentBlockRotation = (_currentBlockRotation + 1) % BLOCK_BAR.size();
      next = BLOCK_BAR[_currentBlockRotation];
      break;
  }


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
      return;
    }
  }

  std::copy(std::begin(tmp), std::end(tmp), std::begin(_currentBlock));
}


void Grid::rotateRight()
{
  std::cout << "rotate right" << std::endl;
}
