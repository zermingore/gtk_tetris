#ifndef SHADERS_HH_
# define SHADERS_HH_

# include <string>


class Shaders
{
public:
  // build the shader (throws!)
  Shaders(const char *pathVertex, const char *pathFragment);

  ~Shaders() = default;


  auto id() { return _id; }

  // use/activate the shader
  void use();

  // utility uniform functions
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setBool(const std::string &name, bool value) const;


private:
  std::string getFileContent(const char *path); // throws!
  void checkCompilation(unsigned int shader, unsigned int type); // throws!

  mutable unsigned int _id; // program id

  const unsigned int _errMsgMaxLen = 4096;
  const int _shader_type_program = 1;
  const int _shader_type_vertex = 2;
  const int _shader_type_fragment = 3;
};

#endif /* !SHADERS_HH_ */
