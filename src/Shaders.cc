#include <Shaders.hh>

#include <sstream>
#include <fstream>
#include <iostream>

#include <GL/glew.h>



Shaders::Shaders(const char *pathVertex, const char *pathFragment)
{
  // Vertex Shader
  const char* vShaderCode = getFileContent(pathVertex).c_str();
  unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);
  checkCompilation(vertex, _shader_type_vertex);

  // Fragment Shader
  const char* fShaderCode = getFileContent(pathFragment).c_str();
  unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);
  checkCompilation(fragment, _shader_type_fragment);

  // Program
  _id = glCreateProgram();
  glAttachShader(_id, vertex);
  glAttachShader(_id, fragment);
  glLinkProgram(_id);
  checkCompilation(fragment, _shader_type_fragment);

  // Free temporary objects
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}



void Shaders::use()
{
  glUseProgram(_id);
}



void Shaders::setInt(const std::string& name, int value) const
{
  glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
}

void Shaders::setFloat(const std::string& name, float value) const
{
  glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
}

void Shaders::setBool(const std::string& name, bool value) const
{
  setInt(name.c_str(), static_cast<int> (value));
}



std::string Shaders::getFileContent(const char *path)
{
  std::stringstream sstr;
  try
  {
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    file.open(path);

    sstr << file.rdbuf();
    file.close();
  }
  catch (std::ifstream::failure &exc)
  {
    std::cerr << "Exception trying to read [" << path << "]: " << exc.what() << "\n";
    throw exc;
  }

  return sstr.str();
}



void Shaders::checkCompilation(unsigned int shader, unsigned int type)
{
  int error;
  char msg[_errMsgMaxLen];
  if (type != _shader_type_program)
  {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &error);
    if (error)
    {
      glGetShaderInfoLog(shader, _errMsgMaxLen, NULL, msg);
      std::cerr << "Shader compilation error (" << type << ")\n" << msg;
      throw;
    }
  }
  else
  {
    glGetProgramiv(shader, GL_LINK_STATUS, &error);
    if (error)
    {
      glGetProgramInfoLog(shader, _errMsgMaxLen, NULL, msg);
      std::cerr << "Shader linking error (" << type << ")\n" << msg;
      throw;
    }
  }
}
