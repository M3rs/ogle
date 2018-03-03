#ifndef OGLE_FONT_HPP
#define OGLE_FONT_HPP

#include <string>
#include <map>

#include <glad/glad.h>
#include "ogle/character.hpp"

namespace ogle {

class Font {
public:
  Font(int size);

  bool loadFromFile(const std::string &filename);
  const std::map<GLchar, Character>& get_characters() const;

private:
  int size;
  std::map<GLchar, Character> characters;
};
  
}

#endif // OGLE_FONT_HPP
