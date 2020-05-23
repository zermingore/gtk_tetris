#include <gtk/gtk.h>



static void left_cb(GtkWidget *widget, gpointer data)
{
  g_print("Left\n");
}



static void right_cb(GtkWidget *widget, gpointer data)
{
  g_print("Right\n");
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


  GObject *render = gtk_builder_get_object(builder, "render");

  // phone 720x1440 -> 720x1360 (buttons free space)
  gtk_widget_set_size_request((GtkWidget*) render, 360, 720);


  GObject *button = gtk_builder_get_object(builder, "button_left");
  g_signal_connect(button, "clicked", G_CALLBACK(left_cb), NULL);

  button = gtk_builder_get_object(builder, "button_right");
  g_signal_connect(button, "clicked", G_CALLBACK(right_cb), NULL);

  button = gtk_builder_get_object(builder, "quit");
  g_signal_connect(button, "clicked", G_CALLBACK(gtk_main_quit), NULL);


  gtk_main();

  return 0;
}
