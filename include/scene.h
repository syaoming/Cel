#ifndef CEL_SCENE_H
#define CEL_SCENE_H

#include <memory>

#include "algebra.h"
#include "model.h"

/*
 * scene.h
 * Define a Scene class
 *
 * TODO: support renderer queries
 * TODO: add lights
 * TODO: add audio
 */

namespace CEL {

// Instance ///////////////////////////////////////////////////////////////////
////

using ModelPtr = std::shared_ptr<Model>;

// Model
// NOTE: you can add more features to help rendering !
struct ModelInstance {
  ModelPtr pmodel;
  Mat4 transform;

  // More
};

// Scene class ////////////////////////////////////////////////////////////////
////

// Base Scene
class Scene {
public:

  // Destructor
  virtual ~Scene() {};

  // Get elements
  // TODO: replace this by a base container type?
  typedef std::vector<ModelInstance> ModelContainer;
  virtual const ModelContainer& get_models() const = 0;

};


// Static Scene
class StaticScene : public Scene {

public:

  // Default constructor; an empty scene
  StaticScene() {}

  // Destructor
  virtual ~StaticScene() override = default;

  // Add elements
  void add_model(const ModelPtr& mp, const Mat4& trans);

  // Get models
  const ModelContainer& get_models() const override;

private:

  std::vector<ModelInstance> models;

};

} // namespace CEL

#endif