// Test loading and drwing mesh

#ifdef USE_MSVC
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

#include <iostream>

#include <asset_loader.h>
#include <camera.h>
#include <console.h>
#include <model.h>
#include <renderer.h>
#include <scene.h>
#include <viewer.h>

using namespace std;
using namespace rei;

int main(int argc, char** argv) {
  // Read the .dae model
  string fn;
  if (argc > 1) {
    fn = string(argv[1]);
  } else {
    fn = string("color_cube.dae");
    console << "using default input file: " << fn << endl;
  }

  AssetLoader loader;
  auto meshes = loader.load_meshes(fn);
  console << "Model read. Get " << meshes.size() << " meshes. " << endl;

  // Check the model
  const auto& vertices = meshes[0]->get_vertices();
  for (const auto& t : meshes[0]->get_triangles()) {
    const Mesh::Vertex& a = vertices[t.a];
    const Mesh::Vertex& b = vertices[t.b];
    const Mesh::Vertex& c = vertices[t.c];
    console << "triangle: " << endl;
    console << "  " << a.coord << ", " << a.color << endl;
    console << "  " << b.coord << ", " << b.color << endl;
    console << "  " << c.coord << ", " << c.color << endl;
  }
  // Set up the scene
  auto s = make_shared<StaticScene>();
  for (auto& mesh : meshes)
    s->add_model(mesh, Mat4::I()); // NOTE: they are load in world-space
  console << "Scene set up. " << endl;

  // window size
  const int width = 720;
  const int height = 480;

  // Set up the camera
  auto c = make_shared<Camera>(Vec3 {0, 0.4, 10}, Vec3 {0, 0, -1});
  c->set_aspect((float)width / height);
  console << "Camera set up." << endl;

  // Make a Renderer
  auto r = makeRenderer();

  // Set up the Viewer and Renderer
  auto v = makeViewer(width, height, "Three Triangle (testing color and z-buffer)");
  v->set_camera(c);
  v->set_scene(s);
  v->set_renderer(r);
  console << "Viewer and Renderer set up." << endl;

  // run
  v->run();

  console << "Program closing." << endl;

  return 0;
}
