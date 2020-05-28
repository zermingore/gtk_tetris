#include <Shaders.hh>

#include <sstream>
#include <fstream>
#include <iostream>

#include <GL/glew.h>



Shaders::Shaders(const char *pathVertex, const char *pathFragment)
{
  // Vertex Shader
  std::string strVertex = getFileContent(pathVertex);
  const char *vShaderCode = strVertex.c_str();
  unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);
  checkCompilation(vertex, _shader_type_vertex);

  // Fragment Shader
  std::string strFragment = getFileContent(pathFragment);
  const char *fShaderCode = strFragment.c_str();
  int fragment = glCreateShader(GL_FRAGMENT_SHADER);
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

  use();
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
  try
  {
    constexpr auto read_size {std::size_t{4096}};
    auto stream {std::ifstream{path}};
    stream.exceptions(std::ios_base::badbit);

    std::string out;
    auto buf {std::string(read_size, '\0')};
    while (stream.read(&buf[0], read_size)) {
      out.append(buf, 0, stream.gcount());
    }
    out.append(buf, 0, stream.gcount());
    return out;
  }
  catch (std::ifstream::failure &exc)
  {
    std::cerr << "Exception reading [" << path << "]: " << exc.what() << "\n";
    throw exc;
  }
  catch (...)
  {
    std::stringstream sstr;
    sstr << "Unknown exception reading [" << path << "]\n";
    std::cerr << sstr.str();
    throw sstr.str();
  }
}



void Shaders::checkCompilation(unsigned int shader, unsigned int type)
{
  int success;
  char msg[_errMsgMaxLen];
  if (type != _shader_type_program)
  {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(shader, _errMsgMaxLen, NULL, msg);
      std::stringstream sstr;
      sstr << "Shader " << shader << " compilation error (" << type << "):\n" << msg;
      std::cerr << sstr.str();
      throw std::runtime_error(sstr.str().c_str());
    }
  }
  else
  {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(shader, _errMsgMaxLen, NULL, msg);
      std::cerr << "Shader " << shader << " linking error (" << type << "):\n" << msg;
      throw;
    }
  }
}
