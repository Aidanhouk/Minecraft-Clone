#pragma once

#include <SFML/System.hpp>

class CameraShaking
{
public:
	CameraShaking();

	void defaultPosition();

	void walk(bool &makeStep);
	void sprint(bool &makeStep);
private:
	sf::Clock m_Clock;
	float m_shift = 0.0f;
	int m_coef = -1;
	int m_rotationStage = 0;
};