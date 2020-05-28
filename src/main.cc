#include <gtk/gtk.h>
#include <GL/glew.h>

#include <Shaders.hh>


const int GRID_NB_LINES = 18;
const int GRID_NB_COLUMNS = 10;

int grid[18][10] = { 0, };


GObject *g_render;

int g_current_block_col = 5;
int g_current_block_line = 18;
int g_current_block_nb_cols = 2;
int g_current_block_nb_lines = 2;

static int init_render_window()
{
  GLenum res = glewInit();
  if (res != GLEW_OK) {
    fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
    return 1;
  }

  return 0;
}


static void left_cb(GtkWidget *widget, gpointer data)
{
  if (g_current_block_col > 0)
    --g_current_block_col;
}



static void right_cb(GtkWidget *widget, gpointer data)
{
  if (g_current_block_col < 10 - g_current_block_nb_cols)
    ++g_current_block_col;
}


float vertices[] = {
  -0.5f, -0.5f, 0.0f, // bottom left
  0.5f,  -0.5f, 0.0f, // bottom right
  0.5f,  0.5f,  0.0f, // top right
  -0.5f, 0.5f,  0.0f  // top left
};


GRand *g_rand;
static gboolean draw()
{
  for (int i = 0; i < GRID_NB_LINES; ++i)
  {
    for (int j = 0; j < GRID_NB_COLUMNS; ++j)
    {
      if (grid[i][j])
      {
        float vertices[] = {
          -0.5f, -0.5f, 0.0f, // bottom left
          0.5f,  -0.5f, 0.0f, // bottom right
          0.5f,  0.5f,  0.0f, // top right
          -0.5f, 0.5f,  0.0f  // top left
        };

//        drawCell(i, j);
      }
    }
  }


  static float offset = 0.f;
  if (offset < 0.9)
  {
    offset += 0.01;
    vertices[1] -= 0.01;
    vertices[4] -= 0.01;
    vertices[7] -= 0.01;
    vertices[10] -= 0.01;
  }


  vertices[0] = -1 + g_current_block_col * 0.2;
  vertices[9] = -1 + g_current_block_col * 0.2;

  vertices[3] = vertices[0] + g_current_block_nb_cols * 0.2;
  vertices[6] = vertices[0] + g_current_block_nb_cols * 0.2;


  unsigned int idx[] = {
    0, 1, 3,
    1, 2, 3
  };

  static Shaders shader("src/shader.vs", "src/shader.fs");

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

  gtk_widget_queue_draw((GtkWidget*) g_render);

  return TRUE;
}



int main(int argc, char **argv)
{
  GtkApplication *app = gtk_application_new(
    "com.nous.test", G_APPLICATION_FLAGS_NONE);

  gtk_init(&argc, &argv);

  GtkBuilder *builder = gtk_builder_new();
  GError *error = NULL;
  if (!gtk_builder_add_from_file(builder, "ui.xml", &error)) // 0 on error
  {
    g_printerr("Error loading file: %s\n", error->message);
    g_clear_error(&error);
    return 1;
  }

  GObject *window = gtk_builder_get_object(builder, "window");
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  g_render = gtk_builder_get_object(builder, "render");
  // phone 720x1440 -> 720x1360 (buttons free space)
  gtk_widget_set_size_request((GtkWidget*) g_render, 360, 720);
  gtk_gl_area_make_current((GtkGLArea*) g_render);
  if (init_render_window())
  {
    g_print("Failure initializing the rendering zone. Aborting...");
    return 1;
  }

  GObject *button = gtk_builder_get_object(builder, "button_left");
  g_signal_connect(button, "clicked", G_CALLBACK(left_cb), NULL);

  button = gtk_builder_get_object(builder, "button_right");
  g_signal_connect(button, "clicked", G_CALLBACK(right_cb), NULL);

  button = gtk_builder_get_object(builder, "quit");
  g_signal_connect(button, "clicked", G_CALLBACK(gtk_main_quit), NULL);

  gtk_gl_area_set_auto_render((GtkGLArea*) g_render, TRUE);
  g_signal_connect(g_render, "render", G_CALLBACK(draw), NULL);

  // g_timeout_add(16, draw, NULL); // fps limit try

  g_rand = g_rand_new_with_seed(123456);
  gtk_main();

  return 0;
}
