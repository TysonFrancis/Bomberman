#pragma once //whoa I made a change
#include <vector>
#include <fstream>
#include <optional>
#include <SFML/Graphics.hpp>

#include "Animations.h"
#include "Audio.h"
#include "Bomb.h"
#include "BonusPoint.h"
#include "Constants.h"
#include "Enemy.h"
#include "Explosion.h"
#include "InfoPanel.h"
#include "Player.h"
#include "Pod.h"
#include "Points.h"
#include "PowerUp.h"
#include "SoftWall.h"
#include "Text.h"

/*
	Game class, handles all game logic, displaying, and events.
	Is the base of the game, holding all necessary items for playing.
*/

class Game
{
public:
	enum class GameState { Title, RoundStart, Transition, Death, Playing, GameOver };

	Game();

	void run();
	
	// Getters for static ints
	static int getSeconds();
	static int getScore();

private:								// *** Main methods *** //
	void events();
	void update();
	void render();
	void closeGame();
										// *** Level preparatory methods *** //
	void level();
	void clear();
	void reset();
										// *** Update submethods *** //
	void timingAndStateChanges();
	void checkBonusConditions();
	void updateEntities();
	void updateUI();
	void pause() { paused = !paused; };

	void startRoundLogic();
	void transitionLogic();
	void deathLogic();
	void titleLogic();
	void gameOverLogic();
										// *** Internal helper methods *** //
	void spawnEnemies(Enemy::Type = Enemy::Type::Pontan);
	Enemy::Type getEnemyType() const;
	std::pair<int, int> getFree() const;


	inline static int s_gameSeconds = 0;
	inline static int s_gameScore = 0;

	Animations animations;

	Player bomber;
	std::vector<Enemy> enemies;
	std::vector<Bomb> bombs;
	std::vector<Explosion> explosions;
	std::vector<SoftWall> softWalls;
	std::vector<Points> points;

	std::optional<PowerUp> powerUp;
	std::optional<BonusPoint> bonusPoints;

	std::vector<Text> textObjects;

	Pod pods[Constants::_rows][Constants::_cols];

	Audio audio;

	sf::Sprite background;
	sf::Sprite title;
	sf::Sprite endTitle;

	sf::RectangleShape pauseBlock;
	Text pauses;

	sf::RenderWindow window;
	sf::View world, UI;
	InfoPanel panel;

	GameState gameState;
	int gameTick;
	int stage;

	int invincibilePlayerTicks;
	bool isInvincibleLit;

	int streak;
	int combo;
	int enemyType;
	int point;
	int goddessCount;
	int colaTimer;
	int colaTick;
	int exitBombs;
	int chain;

	bool levelTransition;
	bool levelTimerExpired;

	bool enterPressed;
	bool displayScore;
	std::fstream file;

	bool gameOver;

	bool bonus;
	bool paused;

	bool bonusSpawned=false;
	bool enemiesKilled=false;
	bool softDestroyed = false;
	bool goddessSet = false;
	bool node, target;

	std::pair<int, int> goddessStart;
	std::pair<int, int> goddessNode;
	std::pair<int, int> goddessTarget;



	const char* song;		// To know which song to stop when level change or game over

	// Each number or vector represents the type of powerup
	// or enemy to spawn for each level, for 50 levels
	int powerupPresets[50] =
	{
		1,0,4,2,0,
		0,1,4,5,3,
		0,0,4,5,1,
		3,0,5,0,4,
		5,4,0,4,3,
		7,1,0,4,6,
		3,0,4,7,5,
		6,4,1,3,7,
		4,3,5,4,7,
		3,5,4,6,7
	};

	int enemyPresets[50][8] =
	{
		{6,0,0,0,0,0,0,0}, {3,3,0,0,0,0,0,0}, {2,2,2,0,0,0,0,0}, {1,1,2,2,0,0,0,0}, {0,4,3,0,0,0,0,0},
		{0,2,3,2,0,0,0,0}, {0,2,3,0,2,0,0,0}, {0,1,2,4,0,0,0,0}, {0,1,1,4,0,1,0,0}, {0,1,1,1,1,3,0,0},
		{0,1,2,3,1,1,0,0}, {0,1,1,1,1,4,0,0}, {0,0,3,3,0,2,0,0}, {0,0,0,0,7,0,1,0}, {0,0,1,3,0,3,1,0},
		{0,0,0,3,0,4,1,0}, {0,0,5,0,0,2,1,0}, {3,3,0,0,0,0,2,0}, {1,1,3,0,1,0,2,0}, {0,1,1,1,1,2,2,0},
		{0,0,0,0,3,4,2,0}, {0,0,4,3,0,1,1,0}, {0,0,2,2,2,2,1,0}, {0,0,1,1,2,4,1,0}, {0,2,1,1,2,2,1,0},
		{1,1,1,1,2,2,1,0}, {1,1,0,0,1,5,1,0}, {0,1,3,3,0,1,1,0}, {0,0,0,0,5,2,2,0}, {0,0,3,2,2,1,1,0},
		{0,2,2,2,2,2,0,0}, {0,1,1,3,0,4,0,0}, {0,0,2,2,1,3,1,0}, {0,0,2,3,0,3,1,0}, {0,0,2,1,1,3,2,0},
		{0,0,2,2,0,3,2,0}, {0,0,2,1,1,3,2,0}, {0,0,2,2,0,3,3,0}, {0,0,1,1,2,2,3,0}, {0,0,1,2,0,3,3,0},
		{0,0,1,1,1,3,3,0}, {0,0,0,1,1,3,5,0}, {0,0,0,1,1,2,5,0}, {0,0,0,1,1,2,6,0}, {0,0,0,0,2,2,5,0},
		{0,0,0,0,2,2,5,0}, {0,0,0,0,2,2,5,0}, {0,0,0,0,1,2,6,1}, {0,0,0,0,2,1,6,1}, {0,0,0,0,2,1,5,2}
	};

	int bonusPointPreset [50] =
	{
		1,2,3,4,5,
		0,1,0,1,2,
		3,4,5,0,1,
		0,1,2,3,4,
		5,0,1,0,1,
		2,3,4,5,0,
		1,0,1,2,3,
		4,5,0,1,0,
		1,2,3,4,5,
		0,1,0,1,2
	};
};