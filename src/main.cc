#include <thread>
#include <chrono>
#include <memory>
#include <iostream>

#include <gtk/gtk.h>
#include <GL/glew.h>

#include <Grid.hh>


GObject *g_render;
GRand *g_rand;
std::unique_ptr<Grid> g_grid;



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
  g_grid->moveLeft();
}



static void right_cb(GtkWidget */* unused */, gpointer /* unused */)
{
  g_grid->moveRight();
}



static gboolean draw()
{
  // using namespace std::chrono_literals;
  // std::this_thread::sleep_for(1s);

  g_grid->fall();
  g_grid->draw();
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

  button = gtk_builder_get_object(builder, "quit");
  g_signal_connect(button, "clicked", G_CALLBACK(gtk_main_quit), NULL);

  gtk_gl_area_set_auto_render((GtkGLArea*) g_render, TRUE);
  g_signal_connect(g_render, "render", G_CALLBACK(draw), NULL);

  // g_timeout_add(16, draw, NULL); // fps limit try

  g_rand = g_rand_new_with_seed(123456);
  gtk_main();

  return 0;
}
