#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "../Shaders/FloraShader.h"

struct RenderInfo;
class ChunkMesh;
class Camera;

class FloraRenderer {
  public:
    void add(const ChunkMesh &mesh);
    void render(const Camera &camera);

  private:
    std::vector<const RenderInfo *> m_chunks;

    FloraShader m_shader;
};