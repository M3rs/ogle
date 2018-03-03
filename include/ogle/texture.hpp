#ifndef OGLE_TEXTURE_HPP
#define OGLE_TEXTURE_HPP

#include "glad/glad.h"
#include <string>
#include <vector>

// fwd
typedef struct FT_GlyphSlotRec_* FT_GlyphSlot;


namespace ogle {

class Texture {
public:
  Texture(const std::string &filename);
  // for Cubemaps!
  Texture(const std::vector<std::string> faces);
  Texture(FT_GlyphSlot glyph);
  ~Texture();

  /// This does not support multiple textures yet!
  void bind() const;

  GLuint get_id();

  void set_type(const std::string &type);
  std::string get_type();

  // move only
  Texture();
  Texture(Texture&& other);
  Texture& operator=(Texture&& other);
  
private:
  GLuint ID;
  std::string Type;

  // no copy
  Texture(const Texture& other) = delete;
  Texture& operator=(const Texture& other) = delete;
};
}

#endif // OGLE_TEXTURE_HPP
