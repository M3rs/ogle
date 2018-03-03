#include "ogle/font.hpp"

#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "ogle/texture.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace {
  
void load_font(FT_Face face, std::map<GLchar, ogle::Character>& Characters)
{
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment

  for (GLubyte c = 0; c < 128; c++) {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
      std::cout << "ERROR::FREETYPE: Failed to load Glyph (" << (char)c << ")"
                << std::endl;
      continue;
    }

    // store characters
    ogle::Character character{
        // texture,
        ogle::Texture{face->glyph},
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        (GLuint)face->glyph->advance.x};

    Characters.insert(std::pair<GLchar, ogle::Character>(c, std::move(character)));
  }

  glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // reset value?
}
  
}

namespace ogle {

Font::Font(int size) : size(size) {}

bool Font::loadFromFile(const std::string &filename)
{
  FT_Library ft;
  if (FT_Init_FreeType(&ft)) {
    std::cout << "ERROR::FREETYPE: Could not init FreeType" << std::endl;
    return false;
  }

  FT_Face face;
  if (FT_New_Face(ft, filename.c_str(), 0, &face)) {
    std::cout << "ERROR::FREETYPE: Failed to load font file ("
	      << filename << ")" << std::endl;
    FT_Done_FreeType(ft);
    return false;
  }

  FT_Set_Pixel_Sizes(face, 0, size);

  load_font(face, characters);
  
  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  return true;
}

const std::map<GLchar, Character>& Font::get_characters() const {
  return characters;
}
  
}
