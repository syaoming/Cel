#ifndef CEL_MODEL_H
#define CEL_MODEL_H

#include <vector>
#include <memory>

#include "algebra.h"

/*
 * model.h
 * This module considers how a model is composed.
 *
 * NOTE: A Model object shoule be unique, but possible to have multiple
 * "instances" by relating with different transforms. (see scene.h)
 *
 * TODO: add aggregated model (and animated model)
 * TODO: support move constructors
 * TODO: Support cube and sphere
 */

namespace CEL {

// Simple Vertex class
struct Vertex {
  Vec4 coord; // Vertex position
  Vertex(Vec3 pos3) : coord(pos3, 1.0) {};
};

// Triangle template class, details depend on implementation of mesh
template<typename VertexIt>
struct Triangle {
  VertexIt a;  // Iterator to vertices
  VertexIt b;
  VertexIt c;
  Triangle(VertexIt a, VertexIt b, VertexIt c) : a(a), b(b), c(c) {};
};

typedef std::vector<Vertex>::iterator VertexIt;
typedef Triangle<VertexIt> TriangleType;


// Model classes //////////////////////////////////////////////////////////////

// Base class
// supports polymophism only through RTTI
class Model {
public:

  virtual ~Model() = default;

};


// Trianglular Mesh
class Mesh : public Model {
public:

  // Dont allow empty mesh
  Mesh() = delete;

  // Constructor with both vertices and triangles of vertex indices
  Mesh(std::vector<Vertex> va, std::vector<Triangle<int>> ta);

  // Destructor; we have only standard containers
  ~Mesh() override = default;

  // Primitives queries
  const std::vector<TriangleType>& get_triangles() const { return triangles; }

private:
  std::vector<Vertex> vertices;
  std::vector<TriangleType> triangles;

};


} // namespace CEL

#endif
