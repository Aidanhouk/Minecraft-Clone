#pragma once

//#include "Mesh.h"
#include "Environment/SnowfallMesh.h"

#include "Util/NonCopyable.h"

#include "Renderer/RenderInfo.h"

class SnowfallMesh;

class Model : public NonCopyable {
public:
    Model() = default;
    Model(const Mesh &mesh);
    ~Model();

    Model(Model &&other);
    Model &operator=(Model &&other);

	void addData(const Mesh &mesh);
	void addDataNoTexture(const Mesh &mesh);
	void addSnowfallData(const SnowfallMesh &mesh);

    void deleteData();

    void genVAO();
    void addEBO(const std::vector<GLuint> &indices);
	void addVBO(int dimensions, const std::vector<GLfloat> &data);
    void bindVAO() const;

    int getIndicesCount() const;

    const RenderInfo &getRenderInfo() const;

private:
    RenderInfo m_renderInfo;

    int m_vboCount = 0;
    std::vector<GLuint> m_buffers;
};