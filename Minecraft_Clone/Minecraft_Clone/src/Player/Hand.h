#pragma once

#include "SFML/System.hpp"

#include "Model.h"

enum class HandState
{
	IDLE = 0,
	BREAKING = 1,
};

class Hand
{
public:
	Hand();

	void _break();
	void swing();
	void setMeshToDraw();

	void leftMouseHold() { m_leftMouseHolded = true; }
	void leftMouseUnhold() { m_leftMouseHolded = false; }

	Model& getModel() { return m_model; }
private:
	HandState	m_state;
	sf::Clock	m_animationTimer;

	Model		m_model;
	bool		m_leftMouseHolded;
};