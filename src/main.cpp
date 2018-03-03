#include <iostream>
#include <map>
#include <vector>

#include <SDL2/SDL.h>
#include <glad/glad.h>

// LUA
#include "sol/helpers.hpp"
#include "sol/sol.hpp"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// OGLE
#include "ogle/engine.hpp"
#include "ogle/renderer.hpp"
#include "ogle/debug.hpp"
#include "ogle/light.hpp"
#include "ogle/ogle.hpp"
#include "ogle/skybox.hpp"

#include "ogle/drawing/drawable.hpp"
#include "ogle/drawing/basicmesh.hpp"
#include "ogle/drawing/basicmodel.hpp"

sol::state init_lua(const std::string &startup) {
  sol::state lua;
  lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math);
  lua["glmvec3"] = [](float x, float y, float z) { return glm::vec3(x, y, z); };

  lua.do_file(startup);

  return std::move(lua);
}

int main() {

  // TODO: engine class
  DEBUG("ENABLED");

  std::string startup = "res/scripts/startup.lua";
  sol::state lua = init_lua(startup);

  // TODO: texture cache
  // TODO: method to clean up VAO / VBO that aren't being for whatever reason
  // (maybe mesh / models?)

  ogle::Engine engine;
  if (!engine.init()) {
    return 1;
  }

  int ww = 1024; // 512
  int wh = 768;  // 512
  ogle::Renderer renderer {"ogle", ww, wh, false};
  
  // END INIT

  // font related
  ogle::Font font{48};
  if (!font.loadFromFile("res/ASENINE_.ttf")) {
    std::cout << "Error loading font" << std::endl;
    return 0;
  }

  glm::mat4 projection_ui = glm::ortho(0.0f, (float)ww, 0.0f, (float)wh);
  ogle::RenderText text{"testing", font, projection_ui};
  text.set_color(glm::vec3(1.0, 0.0, 0.0));
  ogle::RenderText sample("Sample", font, projection_ui);
  sample.set_color(glm::vec3(1.0, 1.0, 1.0));
  sample.set_position(25, 25);

  ogle::Shader f_shader{"res/text.vert", "res/text.frag"};

  // load shader program
  ogle::Shader lightShader{"res/light.vert", "res/light.frag"};

  // IMPORTANT!: because we are using "full screen" it is umportant that
  // we use SDL_GetWindowSize to get the actual window size
  auto win_size = renderer.window_size();
  int w = win_size.x;
  int h = win_size.y;

  float aspect = (float)w / (float)h;
  glm::mat4 projection;
  projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

  ogle::Camera camera{w, h};

  // add lighting!!
  glm::vec3 lightPos(1.2f, 1.75f, -2.0f); // making this higher

  std::vector<ogle::drawing::Drawable*> draw_list;

  // load model
  // ogle::Model ourModel("res/objects/nano/nanosuit.obj");

  ogle::Model forklift("res/objects/forklift.obj");
  ogle::drawing::BasicModel flmodel { forklift, lightShader,
				      { glm::vec3(-2.0f, 0.5f, -3.0f), 0.8f } };
  draw_list.push_back(&flmodel);
  
  ogle::Model robot("res/objects/robot_inprogress.obj");
  float robotscale = 0.5f;
  ogle::Transform robot_transform {glm::vec3(2.0f, 0.0f, 0.0f), 0.5f };
  ogle::drawing::BasicModel robot_m { robot, lightShader, robot_transform };
  draw_list.push_back(&robot_m);
  
  ogle::Model fighter("res/objects/fighter01.obj");
  ogle::Transform ftransform {};
  ogle::drawing::BasicModel fighter_m { fighter, lightShader, {} };
  draw_list.push_back(&fighter_m);

  // floor:
  ogle::Mesh floor_mesh("res/floor.omf");
  ogle::Transform floor_transform { glm::vec3(1.0f, -0.5f, 0.0f),
				    glm::vec3(30.0f, 0.0f, 30.0f)};

  ogle::drawing::BasicMesh floor_d { floor_mesh, lightShader, floor_transform };
  draw_list.push_back(&floor_d);

  // floor 2?
  glm::mat4 floor_m2;
  auto floor_scale = glm::vec3(30.0f, 0.0f, 30.0f);
  floor_m2 = glm::translate(floor_m2, glm::vec3(1.0f, -0.5f, 45.0f));
  floor_m2 = glm::scale(floor_m2, floor_scale);

  ogle::Mesh ceiling_mesh("res/ceiling.omf");
  glm::mat4 ceiling_m;
  ceiling_m = glm::translate(ceiling_m, glm::vec3(1.0f, 2.0f, 0.0f));
  ceiling_m = glm::scale(ceiling_m, floor_scale);

  // end floor
  std::vector<glm::vec3> positions = {
      glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(0.0f, 0.0f, -1.0f),
      glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f, 0.0f, -3.0f),
      glm::vec3(1.0f, 0.0f, -4.0f), glm::vec3(3.0f, 0.0f, -4.0f),
  };
  ogle::Mesh crate_mesh("res/crate.omf");
  //glm::mat4 crate_mat;
  std::vector<ogle::drawing::BasicMesh> crates;
  for (auto p : positions) {
    crates.push_back({crate_mesh, lightShader, {p}});
  }
  for (auto& c : crates) {
    draw_list.push_back(&c);
  }

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
  std::vector<ogle::drawing::BasicMesh> wall_meshes;
  for (auto t : wall_ts) {
    wall_meshes.push_back({wall_mesh, lightShader, t});
  }
  for (auto& w : wall_meshes) {
    draw_list.push_back(&w);
  }

  // way to set light shader properties

  // skybox
  ogle::Skybox skybox{lua.get<sol::table>("skybox")};

  ogle::Light light_s{lua.get<sol::table>("light")};

  // UI stuff
  /*
   float ui_verts[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
  };  
  GLuint ui_vao, ui_vbo;
  glGenVertexArrays(1, &ui_vao);
  glGenBuffers(1, &ui_vbo);
  glBindVertexArray(ui_vao);
  glBindBuffer(GL_ARRAY_BUFFER, ui_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(ui_verts), ui_verts, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  ogle::Shader ui_shader {"res/ui.vert", "res/ui.frag"};
  */
   // END UI STUFF

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

    // TODO: light "update"
    light_s.position = camera.get_pos();
    light_s.direction = camera.get_dir();

    renderer.grab_mouse();

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
    renderer.clear();

    // bind texture before shader/vao/draw?

    // render
    lightShader.use();

    // must set very frame: light properties
    lightShader.set_mat4("view", view);
    lightShader.set_mat4("projection", projection);
    lightShader.set_vec3("viewPos", camera.get_pos());
    lightShader.set_float("material.shininess", 64.0f);
    lightShader.set_light(light_s);

    // draw everything
    for (auto d : draw_list) {
      d->Draw();
    }

    lightShader.set_mat4("model", floor_m2);
    floor_mesh.Draw(lightShader);
    
    lightShader.set_mat4("model", ceiling_m);
    ceiling_mesh.Draw(lightShader);

    // draw skybox
    skybox.Draw(view, projection);

    // END DRAW STUFF

    // RENDER UI
    f_shader.use();
    text.Draw(f_shader);
    sample.Draw(f_shader);
    
    /*
    ui_shader.use();
    glBindVertexArray(ui_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    */
    // end render

    renderer.swap();
  }

  // de-allocation
  // TODO: Fix this!! Mesh dtor should clean up
  // glDeleteVertexArrays(1, &VAO);
  // glDeleteBuffers(1, &VBO);
  // glDeleteBuffers(1, &EBO); // unused atm

}
