#pragma once
#include <vector>

namespace UI {
class Element {
 public:
  Element* parent = nullptr;
  std::vector<Element*> children;

  int x, y, width, height;
  bool dirty = true;

  virtual ~Element() = default;

  virtual void draw() = 0;

  void markDirty() {
    dirty = true;
    if (parent) parent->markDirty();  // Bubble up
  }

  void addChild(Element* child) {
    child->parent = this;
    children.push_back(child);
    markDirty();
  }

  virtual void update() {
    if (dirty) {
      draw();
      dirty = false;
    }
    for (auto* child : children) child->update();
  }

  // Utility: check if this component overlaps a dirty region (optional)
};
}  // namespace UI