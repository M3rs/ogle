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
#include <glm/gtx/string_cast.hpp>

// OGLE
#include "ogle/drawing/drawing.hpp"
#include "ogle/ogle.hpp"
#include "ogle/sf/sf.hpp"

// IMGUI
#include "imgui.h"
#include "imgui_impl_sdl_gl3.h"

sol::state init_lua(const std::string &startup) {
  sol::state lua;
  lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math);
  lua["glmvec3"] = [](float x, float y, float z) { return glm::vec3(x, y, z); };

  lua.do_file(startup);

  return lua;
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

  int ww = 1024;                                  // 512
  int wh = 512;                                   // 768;  // 512
  ogle::Renderer renderer{"ogle", ww, wh, false}; // true};

  // END INIT

  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImGui_ImplSdlGL3_Init(renderer.get_window());
  ImGui::StyleColorsDark();

  // font related
  ogle::Font font{48};
  if (!font.loadFromFile("res/ASENINE_.ttf")) {
    std::cout << "Error loading font" << std::endl;
    return 0;
  }

  auto win_size = renderer.window_size();
  int w = win_size.x;
  int h = win_size.y;

  // glm::mat4 projection_ui = glm::ortho(0.0f, (float)ww, 0.0f, (float)wh);
  glm::mat4 projection_ui = glm::ortho(0.0f, (float)w, 0.0f, (float)h);
  ogle::RenderText text{"testing", font, projection_ui};
  text.set_color(glm::vec3(1.0, 0.0, 0.0));
  ogle::RenderText sample("Sample", font, projection_ui);
  sample.set_color(glm::vec3(1.0, 1.0, 1.0));
  sample.set_position(25, h - 35);

  ogle::Shader f_shader{"res/text.vert", "res/text.frag"};

  // load shader program
  ogle::Shader lightShader{"res/light.vert", "res/light.frag"};

  // IMPORTANT!: because we are using "full screen" it is umportant that
  // we use SDL_GetWindowSize to get the actual window size

  float aspect = (float)w / (float)h;
  glm::mat4 projection;
  projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

  ogle::Camera camera{w, h};

  // add lighting!!
  // glm::vec3 lightPos(1.2f, 1.75f, -2.0f); // making this higher

  std::vector<ogle::drawing::Drawable *> draw_list;

  // load model
  // ogle::Model ourModel("res/objects/nano/nanosuit.obj");

  ogle::Model forklift("res/objects/forklift.obj");
  ogle::drawing::BasicModel flmodel{
      forklift, lightShader, {glm::vec3(-2.0f, 0.5f, -3.0f), 0.8f}};
  draw_list.push_back(&flmodel);

  ogle::Model robot("res/objects/robot_inprogress.obj");
  // float robotscale = 0.5f;
  ogle::Transform robot_transform{glm::vec3(2.0f, 0.0f, 0.0f), 0.5f};
  ogle::drawing::BasicModel robot_m{robot, lightShader, robot_transform};
  draw_list.push_back(&robot_m);

  // auto rmr = projection * robot_transform.Model;
  // std::cout << "Robot: " << glm::to_string(rmr) << std::endl;

  ogle::Model fighter("res/objects/fighter01.obj");
  ogle::drawing::BasicModel fighter_m{
      fighter, lightShader, {glm::vec3(0.0f, 1.0f, 1.5f), 0.5f}};
  draw_list.push_back(&fighter_m);

  glm::vec3 fire_pos(2.0f, 0.0f, 2.0f);
  ogle::Model fire_obj("res/objects/fire/PUSHILIN_campfire.obj");
  ogle::drawing::BasicModel fire_model{fire_obj, lightShader, {fire_pos}};
  draw_list.push_back(&fire_model);

  // floor:
  ogle::Mesh floor_mesh("res/floor.omf");
  auto floor_scale = glm::vec3(30.0f, 0.0f, 30.0f);
  ogle::Transform floor_transform{glm::vec3(1.0f, -0.5f, 0.0f), floor_scale};

  ogle::drawing::BasicMesh floor_d{floor_mesh, lightShader, floor_transform};
  draw_list.push_back(&floor_d);

  // floor 2?
  ogle::drawing::BasicMesh floor_d2{
      floor_mesh, lightShader, {glm::vec3(1.0f, -0.5f, 45.0f), floor_scale}};
  draw_list.push_back(&floor_d2);

  ogle::Mesh ceiling_mesh("res/ceiling.omf");
  ogle::drawing::BasicMesh ceiling_d{
      ceiling_mesh, lightShader, {glm::vec3(1.0f, 2.0f, 0.0f), floor_scale}};
  draw_list.push_back(&ceiling_d);

  // end floor
  std::vector<glm::vec3> positions = {
      glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(0.0f, 0.0f, -1.0f),
      glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f, 0.0f, -3.0f),
      glm::vec3(1.0f, 0.0f, -4.0f), glm::vec3(3.0f, 0.0f, -4.0f),
  };
  ogle::Mesh crate_mesh("res/crate.omf");

  std::vector<ogle::drawing::BasicMesh> crates;
  for (auto p : positions) {
    crates.push_back({crate_mesh, lightShader, {p}});
  }
  for (auto &c : crates) {
    draw_list.push_back(&c);
  }

  // wall(s) ?
  ogle::Mesh wall_mesh("res/wall.omf");
  glm::vec3 wall_scale = glm::vec3(4.0f, 3.0f, 0.5f);
  std::vector<ogle::Transform> wall_ts = {
      {glm::vec3(0.0f, 0.5f, -8.0f), wall_scale},
      {glm::vec3(4.0f, 0.5f, -8.0f), wall_scale},
      {glm::vec3(6.25f, 0.5f, -6.25f), ogle::Rotation{90.0f, ogle::RotAxis::Y},
       wall_scale},
  };
  std::vector<ogle::drawing::BasicMesh> wall_meshes;
  for (auto t : wall_ts) {
    wall_meshes.push_back({wall_mesh, lightShader, t});
  }
  for (auto &w : wall_meshes) {
    draw_list.push_back(&w);
  }

  // way to set light shader properties

  // skybox
  ogle::Skybox skybox{lua.get<sol::table>("skybox")};

  ogle::Light light_s{lua.get<sol::table>("light")};

  // UI stuff

  //  float ui_verts[] = {
  //   -0.5f, -0.5f, 0.0f,
  //    0.5f, -0.5f, 0.0f,
  //   -0.5f,  0.5f, 0.0f,
  //   0.5f, 0.5f, 0.0f,
  //   0.5f, -0.5f, 0.0f,
  //   -0.5f, 0.5f, 0.0f,
  // };
  // GLuint ui_vao, ui_vbo;
  // glGenVertexArrays(1, &ui_vao);
  // glGenBuffers(1, &ui_vbo);
  // glBindVertexArray(ui_vao);
  // glBindBuffer(GL_ARRAY_BUFFER, ui_vbo);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(ui_verts), ui_verts, GL_STATIC_DRAW);
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
  // (void*)0);
  // glEnableVertexAttribArray(0);
  // glBindBuffer(GL_ARRAY_BUFFER, 0);
  // glBindVertexArray(0);

  // ogle::Shader ui_shader {"res/ui.vert", "res/ui.frag"};
  // END UI STUFF

  // sound stuff
  ogle::sf::Audio audio;
  if (!audio.init()) {
    std::cout << "Failed to init audio" << std::endl;
  }
  ogle::sf::WavFx fire_buf("res/sfx/fire.wav");
  ogle::sf::Source fire_source;
  fire_source.set_buffer(fire_buf);
  fire_source.set_position(fire_pos.x, fire_pos.y, fire_pos.z);
  fire_source.set_repeat(true);
  fire_source.set_gain(0.6f);
  fire_source.play();

  ogle::sf::OggFx bgm_ogg("res/sfx/Holst-_venus.ogg");
  ogle::sf::Source bgm_source;
  bgm_source.set_buffer(bgm_ogg);
  bgm_source.set_relative(true);
  bgm_source.set_repeat(true);
  bgm_source.set_gain(0.5f);
  bgm_source.play();

  ogle::sf::WavFx foot_buf("res/sfx/footsteps.wav");
  ogle::sf::Source footsteps;
  footsteps.set_buffer(foot_buf);
  footsteps.set_repeat(true);
  footsteps.set_gain(0.3f);
  //footsteps.set_relative(true);
  bool walking = false;

  const Uint8* keys = SDL_GetKeyboardState(NULL);
  // "loop"
  unsigned int lastTime = 0, currentTime = SDL_GetTicks();

  bool debug = false;
  bool run = true;
  while (run) {
    // events
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      ImGui_ImplSdlGL3_ProcessEvent(&e);

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
        if (e.key.keysym.sym == SDLK_f) {
          debug = !debug;
          camera.set_debug(debug);
          renderer.set_debug(debug);
        }
        if (e.key.keysym.sym == SDLK_g) {
          auto cp = camera.get_pos();
          std::cout << "Camera: " << glm::to_string(cp) << std::endl;
        }
        break;
      }
    }
    // update
    ImGui_ImplSdlGL3_NewFrame(renderer.get_window());
    // float dt = (float)SDL_GetTicks() / 1000.0f; // in s (seconds)
    lastTime = currentTime;
    currentTime = SDL_GetTicks();
    float dt = (float)(currentTime - lastTime) / 1000.0f; // in s (seconds)

    camera.update(dt);

    // TODO: light "update"
    light_s.position = camera.get_pos();
    light_s.direction = camera.get_dir();

    // update audio
    auto cpos = camera.get_pos();
    audio.set_position(cpos.x, cpos.y, cpos.z);
    auto cfront = camera.get_front();
    auto cup = camera.get_up();
    float cam_orient[6] = {cfront.x, cfront.y, cfront.z, cup.x, cup.y, cup.z};
    audio.set_orientation(cam_orient);



    if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_A]
	|| keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_D]) {
      footsteps.set_position(cpos.x, cpos.y - 1, cpos.z);
      if (!walking) {
	// calling play resets the position if it is already playing
	// important to only do this once
	footsteps.play();
	walking = true;
      }
      //std::cout << "playing\n";
    } else {
      footsteps.pause();
      walking = false;
    }

    renderer.grab_mouse();

    ImGui::Text("Testing");
    // static float fx = 0.0f;
    // ImGui::InputFloat("x pos", &fx);
    static glm::vec3 fpos;
    ImGui::InputFloat3("pos", glm::value_ptr(fpos));
    static glm::mat4 fmat;
    ImGui::InputFloat4("model", glm::value_ptr(fmat));
    if (ImGui::Button("+")) {
      // std::cout << "move" << fx << std::endl;
    }

    glm::mat4 view = camera.get_view();

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

    // draw skybox
    skybox.Draw(view, projection);

    // END DRAW STUFF

    // RENDER UI
    f_shader.use();
    text.Draw(f_shader);
    sample.Draw(f_shader);

    // ui_shader.use();
    // glBindVertexArray(ui_vao);
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    if (debug) {
      ImGui::Render();
      ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
    } else {
      ImGui::EndFrame();
    }

    renderer.swap();
  }

  // don't forget de-allocations!
  ImGui_ImplSdlGL3_Shutdown();
  ImGui::DestroyContext();

  std::cout << "Exiting main" << std::endl;
}
