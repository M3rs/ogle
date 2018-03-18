#include "ogle/renderer.hpp"

#include <glad/glad.h>
#include <iostream>

namespace ogle {

Renderer::Renderer(const std::string &title, int width, int height,
                   bool fullscreen)
  : width(width), height(height), win_width(0), win_height(0), debug(false) {
  int flags = SDL_WINDOW_OPENGL;
  if (fullscreen) {
    flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
  }

  win = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, width, height, flags);

  if (!win) {
    std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
    throw "Renderer creation failure (window)";
  }

  ctx = SDL_GL_CreateContext(win);
  if (ctx == NULL) {
    std::cout << "Failed to create OpenGL context: " << SDL_GetError()
              << std::endl;
    throw "Renderer creation failure (context)";
  }

  // hide mouse / grab cursor
  SDL_ShowCursor(SDL_DISABLE);
  SDL_SetWindowGrab(win, SDL_TRUE);

  std::cout << "Open GL Loaded!";
  gladLoadGLLoader(SDL_GL_GetProcAddress); // IMPORTANT!! GL functions will
                                           // SEGFAULT without this
  std::cout << " (got proc address!)\n";
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << '\n';
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << '\n';
  std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;

  SDL_GL_SetSwapInterval(1); // vsync

  // disable for now?
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  SDL_GetWindowSize(win, &win_width, &win_height);
  glViewport(0, 0, win_width, win_height);
}

Renderer::~Renderer() {
  if (ctx) {
    SDL_GL_DeleteContext(ctx);
  }
  if (win) {
    SDL_DestroyWindow(win);
  }
}

SDL_Point Renderer::window_size() const { return {win_width, win_height}; }

void Renderer::grab_mouse() {
  if (debug) {
    return;
  }
  
  SDL_WarpMouseInWindow(win, win_width / 2, win_height / 2);
}

void Renderer::clear() {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::swap() { SDL_GL_SwapWindow(win); }

  SDL_Window * Renderer::get_window() { return win; }

  void Renderer::set_debug(bool debug)
  {
    this->debug = debug;
  }
}
