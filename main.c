#include <gtk/gtk.h>
#include <GL/glew.h>



static void left_cb(GtkWidget *widget, gpointer data)
{
  g_print("Left\n");
}



static void right_cb(GtkWidget *widget, gpointer data)
{
  g_print("Right\n");
}



static gboolean draw()
{
  // may try legacy versions (1.10? / ES?)
  const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

  const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(.2f, 0.2f, 0.2f, 1.0f);\n"
    "}\n\0";


  // shader build
  int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  char err[512];
  int success;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, err);
    g_print("Shader vertex compilation failed %s\n", err);
    return FALSE;
  }

  // fragment shader
  int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, err);
    g_print("Shader fragment compilation failed: %s\n", err);
    return FALSE;
  }

  // link shaders
  int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(shaderProgram, 512, NULL, err);
    g_print("Shader program link failure %s\n", err);
    return FALSE;
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  float vertices[] = {
    -0.5f, -0.5f, 0.0f, // bottom left
    0.5f,  -0.5f, 0.0f, // bottom right
    0.5f,  0.5f,  0.0f, // top right
    -0.5f, 0.5f,  0.0f  // top left
  };

  unsigned int idx[] = {
    0, 1, 3,
    1, 2, 3
  };

  unsigned int vbo;
  unsigned int vao;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  unsigned int ebo;
  glGenBuffers(1, &ebo);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);


  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);


  // loop
  glClearColor(.95, .95, .95, 0);
  glClear(GL_COLOR_BUFFER_BIT);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glUseProgram(shaderProgram);
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);


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

  GObject *render = gtk_builder_get_object(builder, "render");


  // phone 720x1440 -> 720x1360 (buttons free space)
  gtk_widget_set_size_request((GtkWidget*) render, 360, 720);

  gtk_gl_area_make_current((GtkGLArea*) render);
  GLenum res = glewInit();
  if (res != GLEW_OK) {
    fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
    return 1;
  }


  GObject *button = gtk_builder_get_object(builder, "button_left");
  g_signal_connect(button, "clicked", G_CALLBACK(left_cb), NULL);

  button = gtk_builder_get_object(builder, "button_right");
  g_signal_connect(button, "clicked", G_CALLBACK(right_cb), NULL);

  button = gtk_builder_get_object(builder, "quit");
  g_signal_connect(button, "clicked", G_CALLBACK(gtk_main_quit), NULL);

  g_signal_connect(render, "render", G_CALLBACK(draw), NULL);

  gtk_main();

  return 0;
}
