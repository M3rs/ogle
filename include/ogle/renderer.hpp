#ifndef OGLE_RENDERER_HPP
#define OGLE_RENDERER_HPP

#include <string>
#include <SDL2/SDL.h>

namespace ogle {

  class Renderer {
  public:
    Renderer(const std::string& title, int width, int height, bool fullscreen);
    ~Renderer();

    SDL_Point window_size() const;

    void grab_mouse();

    void clear();
    void swap();
    
  private:
    SDL_Window* win;
    SDL_GLContext ctx;

    int width;
    int height;

    /// window height is adjusted by fullscreen
    int win_width;
    int win_height;
  };
  
}

#endif // OGLE_RENDERER_HPP
