#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "../Shaders/WaterShader.h"

struct RenderInfo;
class ChunkMesh;
class Camera;

class WaterRenderer {
public:
    void add(const ChunkMesh &mesh);
    void render(const Camera &camera);

private:
    std::vector<const RenderInfo *> m_chunks;

    WaterShader m_shader;
};