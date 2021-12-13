#pragma once

#include "Player/Player.h"
#include "StateBase.h"

#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Util/FPSCounter.h"
#include "World/Chunk/Chunk.h"
#include "World/World/World.h"

#include "Tick/TickManager.h"
#include "Environment/SkyManager.h"
#include "Player/BlockBreaker.h"
#include "Player/PlayerHand/Hand.h"

extern std::shared_ptr<SkyManager> m_sky;

class StatePlaying : public StateBase {
public:
    StatePlaying(Application &app, const Config &config);
	~StatePlaying();

    void handleEvent(sf::Event e) override;
    void handleInput() override;

    void update(float deltaTime) override;

    void render(RenderMaster &renderer) override;

    void onOpen() override;
private:
	void setTextSettings(Application &app);

	Mouse m_mouse;
    Keyboard m_keyboard;
    Player m_player;
    World m_world;

    FPSCounter						m_fpsCounter;
	BlockBreaker					m_blockBreaker;
	Hand							m_hand;
	sf::Clock						m_placeBlockTimer;

	sf::RectangleShape				m_crosshair;
	sf::Texture						m_chTexture;

	sf::Font						m_font;
	sf::Text						m_startText;
	sf::Text						m_pauseText;
	sf::Text						m_deathText;

	std::unique_ptr<TickManager>	m_tickManager;
	std::unique_ptr<std::thread>	m_tickThread;

	std::unique_ptr<std::thread>	m_soundThread;

	bool							m_makeHitSound;
};