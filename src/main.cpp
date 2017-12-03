#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include "ogle/debug.hpp"

// LUA
#include "sol/sol.hpp"
#include "sol/helpers.hpp"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// OGLE
#include "ogle/ogle.hpp"
#include "ogle/skybox.hpp"
#include "ogle/light.hpp"

void init_sdl_gl();
void init_glad(SDL_Window *win);

sol::state init_lua(const std::string& startup)
{
  sol::state lua;
  lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math);
  lua["glmvec3"] = [](float x, float y, float z) { return glm::vec3(x,y,z);};
  
  lua.do_file(startup);

  return std::move(lua);
}

int main() {

  // TODO: engine class
  DEBUG("ENABLED");
    
  std::string startup = "res/scripts/startup.lua";
  sol::state lua = init_lua(startup);

  glm::vec3 lamb = lua["light"]["specular"];
  std::cout << "lua vec3: " << lamb.x << " " << lamb.y << std::endl;
  // TODO: texture cache
  // TODO: method to clean up VAO / VBO that aren't being for whatever reason (maybe mesh / models?)
  
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cout << "Failed to init SDL\n";
    return 1;
  }
  std::cout << "SDL Init" << std::endl;
  init_sdl_gl();

  SDL_Window *win =
      SDL_CreateWindow("ogle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       512, 512, SDL_WINDOW_OPENGL);
  if (win == NULL) {
    std::cout << "failed to create window" << std::endl;
    SDL_Quit();
    return 2;
  }

  SDL_GLContext ctx = SDL_GL_CreateContext(win);
  if (ctx == NULL) {
    std::cout << "failed to create context" << std::endl;
    std::cout << SDL_GetError() << std::endl;
    SDL_Quit();
    return 3;
  }

  SDL_ShowCursor(SDL_DISABLE);
  SDL_SetWindowGrab(win, SDL_TRUE);

  init_glad(win);
  // END INIT

  // load shader program
  ogle::Shader lightShader{"res/light.vert", "res/light.frag"};

  int w, h;
  SDL_GetWindowSize(win, &w, &h);
  float aspect = (float)w / (float)h;
  glm::mat4 projection;
  projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

  ogle::Camera camera{w, h};

  // add lighting!!
  glm::vec3 lightPos(1.2f, 1.75f, -2.0f); // making this higher

  std::vector<glm::vec3> positions = {
      glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(0.0f, 0.0f, -1.0f),
      glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f, 0.0f, -3.0f),

      glm::vec3(1.0f, 0.0f, -4.0f), glm::vec3(3.0f, 0.0f, -4.0f),
  };

  // load model
  // ogle::Model ourModel("res/objects/nano/nanosuit.obj");
  ogle::Model robot("res/objects/robot_inprogress.obj");
  ogle::Model forklift("res/objects/forklift.obj");

  // floor:
  ogle::Mesh floor_mesh("res/floor.omf");
  glm::mat4 floor_m;
  floor_m = glm::translate(floor_m, glm::vec3(1.0f, -0.5f, 0.0f));
  glm::vec3 floor_scale = glm::vec3(30.0f, 0.0f, 30.0f);
  floor_m = glm::scale(floor_m, floor_scale);

  ogle::Mesh ceiling_mesh("res/ceiling.omf");
  glm::mat4 ceiling_m;
  ceiling_m = glm::translate(ceiling_m, glm::vec3(1.0f, 2.0f, 0.0f));
  ceiling_m = glm::scale(ceiling_m, floor_scale);

  // end floor

  ogle::Mesh crate_mesh("res/crate.omf");
  glm::mat4 crate_mat;
  crate_mat = glm::translate(crate_mat, glm::vec3(4.0f, 0.0f, -1.0f));

  // wall(s) ?
  ogle::Mesh wall_mesh("res/wall.omf");
  glm::mat4 wall_mat;

  glm::vec3 wall_scale = glm::vec3(4.0f, 3.0f, 0.5f);

  std::vector<ogle::Transform> wall_ts = {
      ogle::Transform(glm::vec3(0.0f, 0.5f, -8.0f), wall_scale),
      ogle::Transform(glm::vec3(4.0f, 0.5f, -8.0f), wall_scale),
      ogle::Transform(glm::vec3(6.25f, 0.5f, -6.25f),
                      ogle::Rotation{90.0f, ogle::RotAxis::Y}, wall_scale),
  };

  // TODO "Drawable"
  /*
    Draw(shader), should contain model (aka transform)

    Give it:

    ogle::Transform xform    
    ogle::Shader* sptr
    void Draw(ogle::Shader shader)

    Draw()
    {
    sptr->set_mat4("model", xform.model);
    Draw(*sptr);
    }
   */

  // way to set light shader properties

  // skybox
  ogle::Skybox skybox { lua.get<sol::table>("skybox") };

  ogle::Light light_s { lua.get<sol::table>("light") };

  // "loop"
  unsigned int lastTime = 0, currentTime = SDL_GetTicks();

  bool run = true;
  while (run) {
    // events
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_QUIT:
        run = false;
        break;
      case SDL_KEYUP:
        if (e.key.keysym.sym == SDLK_ESCAPE) {
          run = false;
        }
	if (e.key.keysym.sym == SDLK_r) {
	  std::cout << "---\nreloading lua\n";
	  lua.do_file(startup);
	}
        break;
      }
    }
    // update
    // float dt = (float)SDL_GetTicks() / 1000.0f; // in s (seconds)
    lastTime = currentTime;
    currentTime = SDL_GetTicks();
    float dt = (float)(currentTime - lastTime) / 1000.0f; // in s (seconds)

    camera.update(dt);

    SDL_WarpMouseInWindow(win, w / 2, h / 2);

    glm::mat4 view = camera.get_view();

    // rotate light
    // lightPos.x = 1.0f + sin((currentTime / 1000.0f) * 2.0f);
    // lightPos.z = sin((currentTime / 1000.0f) / 2.0f) * 1.0f;
    // ct seconds
    /*
    float dx = currentTime / 1000.0f;
    float r = 1.0f;
    lightPos.x = 1.0f + (sin(dx / 3.0f) * r);// * 2.0f);
    lightPos.y = 4.0f - (std::abs(cos(dx / 3.0f)) * r); // / 2, 1.0f;
    // */
    // lightPos.z = 2.0f + cos(currentTime / 1000.0f);

    // render
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // bind texture before shader/vao/draw?

    // render
    lightShader.use();

    // must set very frame
    lightShader.set_mat4("view", view);
    lightShader.set_mat4("projection", projection);
    lightShader.set_vec3("viewPos", camera.get_pos());

    light_s.position = camera.get_pos();
    light_s.direction = camera.get_dir();

    lightShader.set_light(light_s);

    lightShader.set_float("material.shininess", 64.0f);
    /*
    glm::vec3 ldiffuse = glm::vec3(0.9f, 0.9f, 0.9f);
    // glm::vec3 ldiffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    // lightShader.set_vec3("light.position", lightPos);
    // "flashlight"
    lightShader.set_vec3("light.position", camera.get_pos());
    lightShader.set_vec3("light.direction", camera.get_dir());
    lightShader.set_float("light.cutOff", glm::cos(glm::radians(12.5f)));
    lightShader.set_float("light.outerCutOff", glm::cos(glm::radians(17.5f)));

    float amb = 0.6f;
    //lightShader.set_vec3("light.ambient", glm::vec3(0.3f, 0.3f, 0.3f));
    lightShader.set_vec3("light.ambient", glm::vec3(amb, amb, amb));
    lightShader.set_vec3("light.diffuse", ldiffuse);
    lightShader.set_vec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    lightShader.set_float("material.shininess", 64.0f);

    // for flashlight
    lightShader.set_float("light.constant", 1.0f);
    lightShader.set_float("light.linear", 0.09f);
    lightShader.set_float("light.quadratic", 0.032f);
    */

    float robotscale = 0.5f;
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(robotscale, robotscale, robotscale));
    lightShader.set_mat4("model", model);
    robot.Draw(lightShader);

    float flscale = 0.8f;
    glm::mat4 flmodel;
    flmodel = glm::translate(flmodel, glm::vec3(-2.0f, 0.5f, -3.0f));
    flmodel = glm::scale(flmodel, glm::vec3(flscale, flscale, flscale));
    lightShader.set_mat4("model", flmodel);
    forklift.Draw(lightShader);

    lightShader.use();
    // draw multiple crates
    for (auto &p : positions) {
      glm::mat4 m;
      m = glm::translate(m, p);
      lightShader.set_mat4("model", m);
      // TODO: make it not "re-bind"
      crate_mesh.Draw(lightShader);
    }

    // draw wall(s)
    for (auto &wt : wall_ts) {
      lightShader.set_mat4("model", wt.Model);
      wall_mesh.Draw(lightShader);
    }

    // draw floor / ceiling
    lightShader.set_mat4("model", floor_m);
    floor_mesh.Draw(lightShader);
    //lightShader.set_mat4("model", ceiling_m);
    //ceiling_mesh.Draw(lightShader);

    // draw skybox
    skybox.Draw(view, projection);
   

    // end render

    SDL_GL_SwapWindow(win);
  }

  // de-allocation
  // TODO: Fix this!! Mesh dtor should clean up
  // glDeleteVertexArrays(1, &VAO);
  // glDeleteBuffers(1, &VBO);
  // glDeleteBuffers(1, &EBO); // unused atm

  SDL_GL_DeleteContext(ctx);
  SDL_DestroyWindow(win);
  SDL_Quit();
}

void init_sdl_gl() {
  SDL_GL_LoadLibrary(NULL);

  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  // NB: this attribute is required to get a higher OpenGL contextw
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
}

void init_glad(SDL_Window *win) {
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

  int w, h;
  SDL_GetWindowSize(win, &w, &h);
  glViewport(0, 0, w, h);
}
