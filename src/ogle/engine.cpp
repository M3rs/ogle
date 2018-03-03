#include "ogle/engine.hpp"

#include <iostream>
#include <SDL2/SDL.h>

namespace ogle {

Engine::Engine() {}

Engine::~Engine() {
  std::cout << "Engine shup down" << std::endl;
  SDL_Quit();
}

bool Engine::init() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cout << "Failed to init SDL: " << SDL_GetError() << std::endl;
    return false;
  }

  std::cout << "SDL Init" << std::endl;

  // TODO: way to config this?
  SDL_GL_LoadLibrary(NULL);

  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  // NB: this attribute is required to get a higher OpenGL contextw
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  
  return true;
}
  
}
