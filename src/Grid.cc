#include <gtk/gtk.h>
#include <GL/glew.h>

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
  // if (!cell.occupied)
  //   return;

  // TODO proper transformation
  static float vertices[] = {
    -0.5f, -0.5f, 0.0f, // bottom left
    0.5f,  -0.5f, 0.0f, // bottom right
    0.5f,  0.5f,  0.0f, // top right
    -0.5f, 0.5f,  0.0f  // top left
  };


  static float offset = 0.f;
  if (offset < 0.9)
  {
    offset += 0.01;
    vertices[1] -= 0.01;
    vertices[4] -= 0.01;
    vertices[7] -= 0.01;
    vertices[10] -= 0.01;
  }


  // vertices[0] = -1 + g_current_block_col * 0.2;
  // vertices[9] = -1 + g_current_block_col * 0.2;
  // vertices[3] = vertices[0] + g_current_block_nb_cols * 0.2;
  // vertices[6] = vertices[0] + g_current_block_nb_cols * 0.2;

  vertices[0] = -1 + 2 * 0.2;
  vertices[9] = -1 + 2 * 0.2;
  vertices[3] = vertices[0] + 2 * 0.2;
  vertices[6] = vertices[0] + 2 * 0.2;


  unsigned int idx[] = {
    0, 1, 3,
    1, 2, 3
  };

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



// Grid::moveLeft / moveRight
// Grid::rotateLeft / rotateRight
