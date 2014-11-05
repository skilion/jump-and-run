#pragma once
#ifndef __GAME_LOCAL_H__
#define __GAME_LOCAL_H__


#include <Game.h>
#include <Audio.h>

#include <EventSystem/KeyEvent.h>
#include <EventSystem/MouseEvent.h>
#include <EventSystem/Receiver.h>
#include <Math/Rect.h>
#include <Render/Font.h>
#include <Render/Render.h>

#include "Enemy.h"
#include "World.h"
#include "Player.h"

using namespace myengine;


enum Sounds {
	SOUND_CREDITS_MUSIC,
	SOUND_MUSIC_START,
	SOUND_MUSIC_END = SOUND_MUSIC_START + 4,
	SOUND_COIN,
	SOUND_SQUISH_START,
	SOUND_SQUISH_END = SOUND_SQUISH_START + 3,
	SOUND_JUMP,
	SOUND_DIAMOND,
	SOUND_END_LEVEL,
	SOUND_END_GAME,
	SOUND_LAST
};

enum Textures {
	TEX_WORLD_TILES,
	TEX_PLAYER_START,
	TEX_PLAYER_END = TEX_PLAYER_START + 8,
	TEX_GREEN_BALL_START,
	TEX_GREEN_BALL_END = TEX_GREEN_BALL_START + 6,
	TEX_EVIL_MAN_START,
	TEX_EVIL_MAN_END = TEX_EVIL_MAN_START + 2,
	TEX_SUPER_BALL_START,
	TEX_SUPER_BALL_END = TEX_SUPER_BALL_START + 4,
	TEX_FLY_BALL_START,
	TEX_FLY_BALL_END = TEX_FLY_BALL_START + 4,
	TEX_POSTER,
	TEX_LAST
};

extern unsigned sounds[SOUND_LAST];
extern Texture	*textures[TEX_LAST];


class GameLocal: public Game, public Receiver<KeyEvent>, public Receiver<MouseEvent>
{
public:
		GameLocal();
		//virtual ~GameLocal();
	
	//Game interface
	virtual bool	initialize();
	virtual void	shutdown();
	virtual void	frame(unsigned elapsedTime);

	//Receiver interface
	virtual void	onEvent(KeyEvent &keyEvent);
	virtual void	onEvent(MouseEvent &mouseEvent);

	void			addScore(int score);
	void			nextLevel();


private:
	enum Mode {
		MODE_START,
		MODE_EDITOR,
		MODE_GAME,
		MODE_BRIEF,
		MODE_CREDITS
	};

	void			loadAnimation(unsigned start, unsigned end, const char *prefix);

	String			getLevelFileName();
	void			drawMenuBack();

	void			reset();
	void			moveWorld(int move);

	void			startLoop();
#ifdef _DEBUG
	void			editorLoop();
#endif //_DEBUG
	void			gameLoop();
	void			briefLoop();
	void			creditsLoop();

	bool			loadLevel(bool changeMusic);

#ifdef _DEBUG
	void			saveLevel();
#endif //_DEBUG

	//Start loop variables
	float			menuBgColor;
	int				titleX, titleWidth;
	
	//Editor loop variables
#ifdef _DEBUG
	bool			choosingTile, addingEnemies;
	bool			drawing1, drawing2;
	Tile			editorTile;
	unsigned		enemyNo;
	Enemy::EnemyType	enemyType;
	Vec2i			screenPos;
#endif //_DEBUG

	//Game loop variables
	Player			player;
	bool			movingLeft, movingRight;
	int				level, life, playerScore;
	unsigned		levelMusic;
	int				reloadTimeout;
	int				nextLevelTimeout;
	
	//Credits loop variables
	int				creditsY;
	unsigned		creditsWait;
	
	//Common things
	Mode			mode;
	int				pixelShift;
	Font			*smallFont, *medFont, *bigFont;
	static const unsigned MAX_ENEMIES = 50;
	Enemy			enemies[MAX_ENEMIES];
	unsigned		enemyCount;
};

extern GameLocal *gameLocal;


#endif //!__GAME_LOCAL_H__