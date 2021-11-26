#include "Hand.h"

#include "World/Block/BlockDatabase.h"
#include "Renderer/RenderMaster.h"
#include "HandAnimationData.h"
#include "Audio/SoundMaster.h"
#include "Audio/SoundFunctions.h"

#include <iostream>

using TexCoords = std::array<GLfloat, 8>;

Hand::Hand()
	:m_animationType{ AnimationType::Idle },
	m_leftMouseHolded{ false },
	m_lastItemInHand{ BlockId::NUM_TYPES },
	m_lastAnimationType{ AnimationType::Idle },
	m_soundMade{ false }
{
	m_pTexturAtlas = &BlockDatabase::get().textureAtlas;
	m_pTexturAtlas->createAtlasPixelsVector();
}

void Hand::blockHit()
{
	if (m_animationType != AnimationType::Breaking) {
		m_animationType = AnimationType::Breaking;
		m_handMesh.setAnimationType(m_animationType);
		m_animationTimer.restart();
	}
}

void Hand::swing()
{
	if (m_animationType != AnimationType::Breaking && m_leftMouseHolded == false) {
		m_animationType = AnimationType::Breaking;
		m_handMesh.setAnimationType(m_animationType);
		m_animationTimer.restart();
	}
}

void Hand::eat()
{
	if (m_animationType != AnimationType::Eating) {
		m_animationType = AnimationType::Eating;
		m_handMesh.setAnimationType(m_animationType);
		m_animationTimer.restart();
	}
}

void Hand::stopEating()
{
	if (m_animationType == AnimationType::Eating) {
		m_animationType = AnimationType::Idle;
		m_handMesh.setAnimationType(m_animationType);
		m_animationTimer.restart();
	}
}

void Hand::leftMouseHold()
{
	m_leftMouseHolded = true;
}

void Hand::leftMouseUnhold()
{
	m_leftMouseHolded = false;
}

void Hand::rightMouseHold()
{
	m_rightMouseHolded = true;
}

void Hand::rightMouseUnhold()
{
	m_rightMouseHolded = false;
}

void Hand::drawHand(RenderMaster & renderer)
{
	renderer.drawHand(m_handMesh);
	m_handMesh.bufferMesh();
}

bool Hand::update(ChunkBlock block)
{
	bool createMesh = false;

	// item changed
	if (m_lastItemInHand != block.getData().id) {
		m_lastItemInHand = block.getData().id;
		
		if (m_animationType != AnimationType::Breaking) {

			m_animationType = AnimationType::Idle;
			m_handMesh.setAnimationType(m_animationType);
			m_lastAnimationType = m_animationType;
			m_animationStage = 0.0f;
			m_animationTimer.restart();
		}

		createMesh = true;
		m_soundMade = false;
	}

	// animation changed
	if (m_lastAnimationType != m_animationType) {
		m_lastAnimationType = m_animationType;

		createMesh = true;
		m_soundMade = false;
	}

	bool makeHitSound = false;
	// animation update
	if (m_animationType != AnimationType::Idle) {
		if (m_animationStage == 0.0f)
			createMesh = true;
		if (!m_soundMade) {
			if (m_leftMouseHolded) {
				if (m_animationType == AnimationType::Breaking) {
					makeHitSound = true;
					m_soundMade = true;
				}
			}
			if (m_rightMouseHolded) {
				if (m_animationType == AnimationType::Eating) {
					g_SoundMaster.playRandom(SoundSet::EatSet);
					m_soundMade = true;
				}
			}
		}

		float animationTimePassed = m_animationTimer.getElapsedTime().asSeconds();
		if (animationTimePassed < ANIMATION_TIME) {
			m_animationStage = animationTimePassed / ANIMATION_TIME;
		}
		// animation finished
		else {
			m_animationType = AnimationType::Idle;
			m_handMesh.setAnimationType(m_animationType);
			m_lastAnimationType = m_animationType;
			m_animationStage = 0.0f;
			m_animationTimer.restart();

			createMesh = true;
			m_soundMade = false;
		}
	}

	m_handMesh.setAnimationStage(m_animationStage);

	if (createMesh) {
		//static int iter = 0;
		//std::cout << iter++ << "\n";
		m_handMesh.deleteData();

		// empty hand
		if (block.getData().id == BlockId::Air) {
			m_handMesh.setHandType(HandType::Empty);
			makeEmptyHandMesh();
		}
		else {
			if (block.getData().meshType == BlockMeshType::Cube) {
				m_handMesh.setHandType(HandType::Cube);
				makeCubeMesh(block);
			}
			else if (block.getData().meshType == BlockMeshType::Cactus) {
				m_handMesh.setHandType(HandType::Cactus);
				makeCactusMesh(block);
			}
			else { // X or Default
				m_handMesh.setHandType(HandType::Default);
				makeDefaultItemMesh(block);
			}
		}
	}

	return makeHitSound;
}

void Hand::makeEmptyHandMesh()
{
	TexCoords texCoords;

	BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, HAND_TEXTURE_1_COORDS);
	m_handMesh.addFace(emptyHandData::leftFace, texCoords, { 0, 0, 0 }, LIGHT_FRONT);
	BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, HAND_TEXTURE_2_COORDS);
	m_handMesh.addFace(emptyHandData::rightFace, texCoords, { 0, 0, 0 }, LIGHT_RIGHT);
}

void Hand::makeCubeMesh(ChunkBlock& block)
{
	TexCoords texCoords;

	auto &data = block.getData();

	if (m_animationType == AnimationType::Idle) {
		if (!data.isOpaque) {
			BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, data.texSideCoord);
			m_handMesh.addFace(cubeData::backFace, texCoords, { 0, 0, 0 }, LIGHT_FRONT);
			m_handMesh.addFace(cubeData::leftFace, texCoords, { 0, 0, 0 }, LIGHT_RIGHT);
			BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, data.texBottomCoord);
			m_handMesh.addFace(cubeData::bottomFace, texCoords, { 0, 0, 0 }, LIGHT_BOT);
		}
		BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, data.texSideCoord);
		m_handMesh.addFace(cubeData::rightFace, texCoords, { 0, 0, 0 }, LIGHT_RIGHT);
		m_handMesh.addFace(cubeData::frontFace, texCoords, { 0, 0, 0 }, LIGHT_FRONT);
		BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, data.texTopCoord);
		m_handMesh.addFace(cubeData::topFace, texCoords, { 0, 0, 0 }, LIGHT_TOP);
	}
	else {

		BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, data.texTopCoord);
		m_handMesh.addFace(cubeData::topFace, texCoords, { 0, 0, 0 }, LIGHT_TOP);
		BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, data.texSideCoord);
		m_handMesh.addFace(cubeData::rightFace, texCoords, { 0, 0, 0 }, LIGHT_RIGHT);
		BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, data.texBottomCoord);
		m_handMesh.addFace(cubeData::bottomFace, texCoords, { 0, 0, 0 }, LIGHT_BOT);
		BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, data.texSideCoord);
		m_handMesh.addFace(cubeData::leftFace, texCoords, { 0, 0, 0 }, LIGHT_RIGHT);
		BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, data.texSideCoord);
		m_handMesh.addFace(cubeData::frontFace, texCoords, { 0, 0, 0 }, LIGHT_FRONT);
	}
}

void Hand::makeCactusMesh(ChunkBlock& block)
{
	TexCoords texCoords;

	auto &data = block.getData();

	if (m_animationType == AnimationType::Idle) {
		BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, data.texSideCoord);
		m_handMesh.addFace(cactusData::rightFace, texCoords, { 0, 0, 0 }, LIGHT_RIGHT);
		m_handMesh.addFace(cactusData::frontFace, texCoords, { 0, 0, 0 }, LIGHT_FRONT);
		BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, data.texTopCoord);
		m_handMesh.addFace(cactusData::topFace, texCoords, { 0, 0, 0 }, LIGHT_TOP);
	}
	else {

		BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, data.texTopCoord);
		m_handMesh.addFace(cactusData::topFace, texCoords, { 0, 0, 0 }, LIGHT_TOP);
		BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, data.texSideCoord);
		m_handMesh.addFace(cactusData::rightFace, texCoords, { 0, 0, 0 }, LIGHT_RIGHT);
		BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, data.texBottomCoord);
		m_handMesh.addFace(cactusData::bottomFace, texCoords, { 0, 0, 0 }, LIGHT_BOT);
		BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, data.texSideCoord);
		m_handMesh.addFace(cactusData::leftFace, texCoords, { 0, 0, 0 }, LIGHT_RIGHT);
		BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, data.texSideCoord);
		m_handMesh.addFace(cactusData::frontFace, texCoords, { 0, 0, 0 }, LIGHT_FRONT);
	}
}

void Hand::makeDefaultItemMesh(ChunkBlock& block)
{
	TexCoords texCoords;

	auto &data = block.getData();

	BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, data.texTopCoord);

	auto pixels = BlockDatabase::get().textureAtlas.getIndivTexturePixels(data.texTopCoord);

	int indivTextureSize = BlockDatabase::get().textureAtlas.getIndivTextureSize();
	float pixelSize = (1.0f / 16) / indivTextureSize; // 16 hardcored

	for (int y = 0; y < indivTextureSize; ++y) {
		for (int x = 0; x < indivTextureSize; ++x) {
			int index = y * indivTextureSize + x;
			if (pixels[index]->a == 0.0f)
				continue;

			auto pixelTexCoords = texCoords;
			//\
					xMin 0 6\
					xMax 2 4\
					yMin 5 7\
					yMax 1 3\
					return { xMin, yMin, xMax, yMin, xMax, yMax, xMin, yMax };
			pixelTexCoords[0] = pixelTexCoords[6] = pixelTexCoords[0] + x * pixelSize; // xMin
			pixelTexCoords[2] = pixelTexCoords[4] = pixelTexCoords[0] + pixelSize; // xMax
			pixelTexCoords[5] = pixelTexCoords[7] = pixelTexCoords[5] + y * pixelSize; // yMin
			pixelTexCoords[1] = pixelTexCoords[3] = pixelTexCoords[5] + pixelSize; // yMax

			glm::vec3 pos = { 0, 0, -1.0f - defaultItemData::PIXEL_SIZE };
			pos.x += x * defaultItemData::PIXEL_SIZE;
			pos.y -= (y + 1) * defaultItemData::PIXEL_SIZE;

			switch (m_animationType)
			{
			case AnimationType::Idle:
			case AnimationType::Breaking:
				if (y == 0)
					m_handMesh.addFace(defaultItemData::sideFaceTop, pixelTexCoords, pos, LIGHT_TOP);
				else if (pixels[index - indivTextureSize]->a == 0.0f)
					m_handMesh.addFace(defaultItemData::sideFaceTop, pixelTexCoords, pos, LIGHT_TOP);

				if (y == indivTextureSize - 1)
					m_handMesh.addFace(defaultItemData::sideFaceBottom, pixelTexCoords, pos, LIGHT_BOT);
				else if (pixels[index + indivTextureSize]->a == 0.0f)
					m_handMesh.addFace(defaultItemData::sideFaceBottom, pixelTexCoords, pos, LIGHT_BOT);

				if (x == indivTextureSize - 1)
					m_handMesh.addFace(defaultItemData::sideFaceRight, pixelTexCoords, pos, LIGHT_RIGHT);
				else if (pixels[index + 1]->a == 0.0f)
					m_handMesh.addFace(defaultItemData::sideFaceRight, pixelTexCoords, pos, LIGHT_RIGHT);
				break;
			case AnimationType::Eating:
				// probably remove
				if (x == 00)
					m_handMesh.addFace(defaultItemData::sideFaceLeft, pixelTexCoords, pos, LIGHT_RIGHT);
				else if (pixels[index - 1]->a == 0.0f)
					m_handMesh.addFace(defaultItemData::sideFaceLeft, pixelTexCoords, pos, LIGHT_RIGHT);

				if (x == indivTextureSize - 1)
					m_handMesh.addFace(defaultItemData::sideFaceRight, pixelTexCoords, pos, LIGHT_RIGHT);
				else if (pixels[index + 1]->a == 0.0f)
					m_handMesh.addFace(defaultItemData::sideFaceRight, pixelTexCoords, pos, LIGHT_RIGHT);

				if (y == 0)
					m_handMesh.addFace(defaultItemData::sideFaceTop, pixelTexCoords, pos, LIGHT_TOP);
				else if (pixels[index - indivTextureSize]->a == 0.0f)
					m_handMesh.addFace(defaultItemData::sideFaceTop, pixelTexCoords, pos, LIGHT_TOP);
				break;
			default:
				break;
			}
		}
	}
	m_handMesh.addFace(defaultItemData::mainFace, texCoords, { 0, 0, 0 }, LIGHT_FRONT);
}
