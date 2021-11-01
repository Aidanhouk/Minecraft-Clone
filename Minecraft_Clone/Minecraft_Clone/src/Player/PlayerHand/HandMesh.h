#pragma once

#include "Model.h"
#include "HandData.h"

#include "Maths/glm.h"
#include <array>
#include <vector>

class HandMesh {
public:

	HandMesh() = default;

	void addFace(const std::array<GLfloat, 12> &verticesPosition,
		const std::array<GLfloat, 8> &textureCoords,
		const glm::vec3 &itemPosition,
		GLfloat cardinalLight);

	void bufferMesh();
	void deleteData();

	const Model &getModel() const { return m_model; }

	const HandData& getHandData() const { return m_handData; }
	HandType getHandType() const { return m_handData.handType; }
	AnimationType getAnimationType() const { return m_handData.animationType; }
	float getAnimationStage() const { return m_handData.animationStage; }

	void setHandType(HandType handType) { m_handData.handType = handType; }
	void setAnimationType(AnimationType animationType) { m_handData.animationType = animationType; }
	void setAnimationStage(float stage) { m_handData.animationStage = stage; }
private:
	Mesh m_mesh;
	Model m_model;
	std::vector<GLfloat> m_cardinalLight;
	GLuint m_indexIndex = 0;

	HandData m_handData;
	float m_animationStage;
};