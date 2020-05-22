#include <gtk/gtk.h>



static void left_cb(GtkWidget *widget, gpointer data)
{
  g_print("Left\n");
}



static void right_cb(GtkWidget *widget, gpointer data)
{
  g_print("Right\n");
}



static void activate(GtkApplication *app, gpointer user_data)
{
  GtkWidget *window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Test Window");
  gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);


  GtkWidget *grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), grid);

  /* GtkWidget *button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL); */
  /* gtk_container_add (GTK_CONTAINER(window), button_box); */

  // Add buttons
  GtkWidget *button = gtk_button_new_with_label("<-");
  g_signal_connect(button, "clicked", G_CALLBACK(left_cb), NULL);
  gtk_grid_attach (GTK_GRID (grid), button, 0, 0, 1, 1);

  button = gtk_button_new_with_label("->");
  g_signal_connect(button, "clicked", G_CALLBACK(right_cb), NULL);
  gtk_grid_attach (GTK_GRID (grid), button, 1, 0, 1, 1);

  button = gtk_button_new_with_label("Exit");
  g_signal_connect_swapped(
    button, "clicked", G_CALLBACK(gtk_widget_destroy), window);
  gtk_grid_attach (GTK_GRID (grid), button, 0, 1, 2, 1);


  gtk_widget_show_all(window);
}



int main(int argc, char **argv)
{
  GtkApplication *app = gtk_application_new(
    "com.nous.test", G_APPLICATION_FLAGS_NONE);

  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  int status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app); // free app

  return status;
}
