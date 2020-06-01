#include <thread>
#include <chrono>
#include <memory>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>

#include <gtk/gtk.h>
#include <GL/glew.h>

#include <Grid.hh>


GObject *g_render;
std::unique_ptr<Grid> g_grid;
bool g_moved_block_horizontally = false;


static int init_render_window()
{
  GLenum res = glewInit();
  if (res != GLEW_OK) {
    fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
    return 1;
  }

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // debug

  return 0;
}



static void left_cb(GtkWidget */* unused */, gpointer /* unused */)
{
  std::cout << "main: left cb" << std::endl;
  g_grid->moveLeft();
  g_moved_block_horizontally = true;
}

static void right_cb(GtkWidget */* unused */, gpointer /* unused */)
{
  std::cout << "main: right cb" << std::endl;
  g_grid->moveRight();
  g_moved_block_horizontally = true;
}



static void rotate_left_cb(GtkWidget */* unused */, gpointer /* unused */)
{
  std::cout << "main: ROTATE left cb" << std::endl;
  g_grid->rotateLeft();
}

static void rotate_right_cb(GtkWidget */* unused */, gpointer /* unused */)
{
  std::cout << "main: ROTATE right cb" << std::endl;
  g_grid->rotateRight();
}



static gboolean draw()
{
  namespace chr = std::chrono;
  using namespace std::chrono_literals;

  static chr::time_point<chr::system_clock> last_draw = chr::system_clock::now();
  auto now = chr::system_clock::now();
  auto difficulty = 750ms;

  auto elapsed_time {chr::duration_cast<chr::milliseconds> (now - last_draw)};
  if (elapsed_time <= difficulty && !g_moved_block_horizontally)
  {
    std::this_thread::sleep_for(1ms);
    gtk_widget_queue_draw((GtkWidget*) g_render);
    return FALSE;
  }

  if (!g_moved_block_horizontally)
  {
    if (!g_grid->fall())
    {
      return FALSE;
    }
  }
  g_grid->draw();

  g_moved_block_horizontally = false;
  last_draw = chr::system_clock::now();
  gtk_widget_queue_draw((GtkWidget*) g_render);

  return TRUE;
}



int main(int argc, char **argv)
{
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

  g_grid = std::make_unique<Grid> ();

  GObject *button = gtk_builder_get_object(builder, "button_left");
  g_signal_connect(button, "clicked", G_CALLBACK(left_cb), NULL);

  button = gtk_builder_get_object(builder, "button_right");
  g_signal_connect(button, "clicked", G_CALLBACK(right_cb), NULL);

  button = gtk_builder_get_object(builder, "button_rotate_left");
  g_signal_connect(button, "clicked", G_CALLBACK(rotate_left_cb), NULL);

  button = gtk_builder_get_object(builder, "button_rotate_right");
  g_signal_connect(button, "clicked", G_CALLBACK(rotate_right_cb), NULL);

  button = gtk_builder_get_object(builder, "quit");
  g_signal_connect(button, "clicked", G_CALLBACK(gtk_main_quit), NULL);

  gtk_gl_area_set_auto_render((GtkGLArea*) g_render, TRUE);
  g_signal_connect(g_render, "render", G_CALLBACK(draw), NULL);


  g_moved_block_horizontally = true; // Forcing a draw at the very beginning
  gtk_main();

  return 0;
}
