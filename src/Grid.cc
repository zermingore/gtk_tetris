#include <gtk/gtk.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Grid.hh>

#include <iostream>



Grid::Grid()
  : _shader("src/shader.vs", "src/shader.fs")
{
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
  for (auto i{0u}; i < _nbLines; ++i)
  {
    for (auto j{0u}; j < _nbCol; ++j)
    {
      //if (_grid[i][j].occupied)
      {
        drawCell(_grid[i][j]);
      }
    }
  }

  std::cout << "current block pos:\n";
  for (const auto &cell: _currentBlock)
  {
    drawCell(cell);
    std::cout << "  " << cell.line << ", " << cell.col << "\n";
  }
}



void Grid::fall()
{
  for (const auto &cell: _currentBlock)
  {
    /* Check for touch down */
    if (cell.line >= _nbLines - 1 || _grid[cell.line + 1][cell.col].occupied)
    {
      std::cout << "fall done" << std::endl;
      // TODO Check for line completeness
      newBlock();
      return;
    }
  }
  std::cout << "fall" << std::endl;


  for (auto &cell: _currentBlock)
  {
    _grid[cell.line][cell.col].occupied = false;
    ++cell.line; // first line: top
    _grid[cell.line][cell.col].occupied = true;
  }
}



void Grid::newBlock()
{
  _currentBlock.clear();

  _currentBlock.push_back({true, 0, 2});
  // _currentBlock.push_back({true, 0, 3});

  // TODO Handle block boundaries
  // _currentBlock.push_back({true, 16, 2});
  // _currentBlock.push_back({true, 16, 3});
}



void Grid::drawCell(const Cell &cell)
{
  // TODO proper transformation
  static float vertices[] = {
    0, 0, 0.0f, // bottom left
     1.f, 0, 0.0f, // bottom right
     1.f,  1.f, 0.0f, // top right
    0,  1.f, 0.0f  // top left
  };

  unsigned int idx[] = {
    0, 1, 3,
    1, 2, 3
  };


  glm::mat4 model = glm::mat4(1.0f); // *must* be initialized to identity matrix
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);

  float x = cell.col * _cellSizeX;
  float y = cell.line * _cellSizeY;

  // TODO view && projection matrices computation in the constructor
  model = glm::translate(model, glm::vec3(x, y, 0.f));
  model = glm::scale(model, glm::vec3(_cellSizeX, _cellSizeY, 0.f));
  projection = glm::ortho(0.0f, 360.0f, 720.0f, 0.0f, -1.0f, 1.0f);

  // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -18.f));
  //projection = glm::perspective(glm::radians(45.0f), 360.f / 720.f, 0.1f, 100.0f);

  _shader.setMat4("model", model);
  _shader.setMat4("view", view);
  _shader.setMat4("projection", projection);


  unsigned int vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

  unsigned int vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  unsigned int ebo;
  glGenBuffers(1, &ebo);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STREAM_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glClearColor(.95, .95, .95, 0);
  glClear(GL_COLOR_BUFFER_BIT);

  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}



void Grid::moveLeft()
{
  for (const auto &cell: _currentBlock)
  {
    if (cell.col <= 0)
      return;
  }

  for (auto &cell: _currentBlock)
  {
    std::cout << "moving block" << std::endl;
    --cell.col;
  }
}



void Grid::moveRight()
{
  for (const auto &cell: _currentBlock)
  {
    if (cell.col >= _nbCol - 1)
      return;
  }

  for (auto &cell: _currentBlock)
  {
    std::cout << "moving block" << std::endl;
    ++cell.col;
  }
}


// Grid::rotateLeft / rotateRight
