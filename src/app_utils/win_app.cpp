#if WIN32

#include "win_app.h"

#include <iomanip>
#include <sstream>

#include "../debug.h"
#include "../rmath.h"

using std::make_shared;
using std::make_unique;

namespace rei {

WinApp::WinApp(Config config) : config(config) {
  // NOTE: Handle normally get from WinMain;
  // we dont do that here, because we want to create app with standard main()
  hinstance = GetModuleHandle(nullptr); // handle to the current .exe

  input_bus = make_unique<InputBus>();

  // Default renderer
  renderer = make_unique<d3d::Renderer>(hinstance);

  // Default viewer
  viewer = make_unique<WinViewer>(hinstance, config.width, config.height, config.title);
  viewer->init_viewport(*renderer);
  viewer->set_input_bus(input_bus);
  renderer->set_viewport_clear_value(viewer->get_viewport(), config.bg_color);

  // Create default scene and camera
  scene = make_unique<Scene>();
  camera = make_unique<Camera>(Vec3 {0, 0, 10}, Vec3 {0, 0, -1});
  camera->set_aspect(config.width, config.height);
}

WinApp::~WinApp() {
  log("WinApp terminated.");
}

void WinApp::setup(Scene&& scene, Camera&& camera) {
  if (is_started) {
    REI_WARNING("App setup is called after started");
    return;
  }
  initialize_scene();
}

void WinApp::initialize_scene() {
  for (ModelPtr& m : this->scene->get_models()) {
    REI_ASSERT(m);

    // register geometry
    GeometryPtr geo = m->get_geometry();
    if (geo && geo->get_graphic_handle() == nullptr) {
      GeometryHandle g_handle = renderer->create_geometry(*geo);
      geo->set_graphic_handle(g_handle);
    }

    // register material&shader
    MaterialPtr mat = m->get_material();
    if (mat && mat->get_graphic_handle() == nullptr) { REI_NOT_IMPLEMENTED }

    ModelHandle h_model = renderer->create_model(*m);
    m->set_rendering_handle(h_model);
  }
}

void WinApp::start() {
  on_start();

  initialize_scene();

  is_started = true;
}

void WinApp::on_start() {
  if (config.enable_grid_line) {
    // todo create and draw grid line
  }
}

void WinApp::update() {
  // callback
  on_update();

  // Finalize
  input_bus->reset();
}

void WinApp::on_update() {
  update_camera_control();
  update_title();
}

void WinApp::update_camera_control() {
  if (config.default_camera_control_enabled) {
    const Vec3 focus_center {0, 0, 0};
    const Vec3 up {0, 1, 0};

    Vec3 acc;
    for (auto& input : input_bus->get<CursorDrag>()) {
      auto mov = *(input.get<CursorDrag>());
      acc += (mov.stop - mov.start);
    }
    if (acc.norm2() > 0.0001) {
      const double rot_range = pi;
      camera->rotate_position(focus_center, up, -acc.x / viewer->width() * rot_range);
      camera->rotate_position(
        focus_center, -camera->right(), -acc.y / viewer->height() * rot_range);
      camera->look_at(focus_center, up);
    }

    double zoom_in = 0;
    for (auto& input : input_bus->get<Zoom>()) {
      auto zoom = *(input.get<Zoom>());
      zoom_in += zoom.delta;
    }
    if (std::abs(zoom_in) != 0) {
      const double ideal_dist = 8;
      const double block_dist = 4;
      double curr_dist = (camera->position() - focus_center).norm();
      double scale = std::abs((curr_dist - block_dist) / (ideal_dist - block_dist));
      if (zoom_in < 0) scale = (std::max)(scale, 0.1);
      camera->move(0, 0, zoom_in * scale);
    }
  }
}

void WinApp::update_title() {
  if (config.show_fps_in_title) {
    using millisecs = std::chrono::duration<float, std::milli>;
    float ms = millisecs(last_frame_time).count();
    float fps = 1000.f / ms;
    std::wstringstream str {};
    str << config.title << " -- frame info: " << std::setw(16) << ms << "ms, " << std::setw(16)
        << fps << "fps";
    viewer->update_title(str.str());
  }
}

void WinApp::render() {
  clock_last_check = clock.now();
  on_render();
  last_frame_time = clock.now() - clock_last_check;
}

void WinApp::on_render() {
  renderer->prepare(*scene);
  ViewportHandle viewport = viewer->get_viewport();
  renderer->update_viewport_transform(viewport, *camera);
  auto culling_result = renderer->cull(viewport, *scene);
  renderer->render(viewport, culling_result);
}

void WinApp::run() {
  start();

  // Check event and do update&render
  MSG msg;
  ZeroMemory(&msg, sizeof(MSG));
  while (true) {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // passge message to WndProc
    {
      if (msg.message == WM_QUIT) break;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    } else {
      // simple one tick
      update();
      render();
    }
  } // end while
}

} // namespace rei

#endif
