#ifndef REI_WIN_APP_H
#define REI_WIN_APP_H

#include <memory>
#include <string>
#include <chrono>

#include <windows.h>

#include "../color.h"
#include "../scene.h"
#include "../input.h"
#include "viewer.h"
#include "win_viewer.h"
#include "../direct3d/d3d_renderer.h"

namespace rei {

class WinApp {
public:
  struct Config {
    std::wstring title = L"Unmaed App";
    int height = 720;
    int width = 480;
    bool show_fps_in_title = true;
    bool enable_vsync = false;
    Color bg_color = Colors::ayanami_blue;
    bool default_camera_control_enabled = true;
    bool enable_grid_line = true;
  };

  WinApp(Config config) : WinApp(config, nullptr) {}
  virtual ~WinApp();

  [[deprecated]]
  void setup(Scene&& scene, Camera&& camera);

  void run();

  // init helper
  static inline HINSTANCE get_hinstance() { return GetModuleHandle(nullptr); }

protected:
  Config config;

  WinApp(Config config, std::unique_ptr<Renderer>&& renderer);

  Viewer& viewer() const { return *m_viewer; }
  Renderer& renderer() const { return *m_renderer; }
  Scene& scene() const { return *m_scene; }
  Camera& camera() const { return *m_camera; }
  InputBus& input_bus() const { return *m_input_bus; }

  virtual void on_start();
  virtual void on_update();
  virtual void on_render();

  // some useful methods
  void update_camera_control();
  void update_title();

private:
  HINSTANCE hinstance = NULL;

  std::unique_ptr<Viewer> m_viewer;
  std::unique_ptr<Renderer> m_renderer;
  std::unique_ptr<Scene> m_scene;
  std::unique_ptr<Camera> m_camera;
  std::shared_ptr<InputBus> m_input_bus;

  bool is_started = false;

  using clock_t = std::chrono::high_resolution_clock;
  clock_t clock;
  clock_t::time_point clock_last_check;
  clock_t::duration last_frame_time;

  void start();
  void initialize_scene(); // TODO should be more elegant
  void update();
  void render();
};

}

#endif
