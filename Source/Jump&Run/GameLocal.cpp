#include <Log.h>
#include <Random.h>

#include <EventSystem/Sender.h>
#include <File/File.h>
#include <Math/Math.h>
#include <Render/Color.h>
#include <Render/Image.h>
#include <Render/Render.h>

#include "GameLocal.h"
#include "ParticleSystem.h"

using namespace myengine;


unsigned sounds[SOUND_LAST];
Texture	*textures[TEX_LAST];

GameLocal *gameLocal = 0;

const int	PLAYER_LIFE			= 10;
const int	NEXT_LEVEL_TIME		= 60 * 5;
const int	RELOAD_LEVEL_TIME	= 60 * 3;
const int	LAST_LEVEL			= 5;
const char	*GAME_MESSAGE		= "SPACE = jump - LEFT\\RIGHT ARROWS = move - Press ENTER to start";


//=============================================================================
// Creates a GameLocal class
//=============================================================================
Game *Game::create()
{
	gameLocal = new GameLocal;
	return gameLocal;
}


//=============================================================================
// Constructor
//=============================================================================
GameLocal::GameLocal()
{
	levelMusic = 0;

	smallFont = medFont = bigFont = nullptr;
	for(unsigned i = 0; i < TEX_LAST; i++) textures[i] = nullptr;

	menuBgColor = 0;
	titleX = SCREEN_X_PIXEL;
}

//=============================================================================
// Destructor
//=============================================================================
//GameLocal::~GameLocal() {}

//=============================================================================
// Initialize
//=============================================================================
#include <System.h>
bool GameLocal::initialize()
{
	log->printf("Initializing game...");
	keyEventSender.connect(this);
	mouseEventSender.connect(this);
	
	system->setCursor(System::CURSOR_NONE);

	//Load fonts
	smallFont = Font::create("fonts/pixelperfect.ttf", 16, false);
	if(!smallFont) return false;
	medFont = Font::create("fonts/pixelperfect.ttf", 32, true);
	if(!medFont) return false;
	bigFont = Font::create("fonts/gwibble.ttf", 40, true);
	if(!bigFont) return false;

	//Load textures
	textures[TEX_WORLD_TILES] = Texture::create(Image("images/base.png"));
	textures[TEX_POSTER] = Texture::create(Image("images/poster.png"));
	loadAnimation(TEX_PLAYER_START, TEX_PLAYER_END, "images/player");
	loadAnimation(TEX_GREEN_BALL_START, TEX_GREEN_BALL_END, "images/green_ball");
	loadAnimation(TEX_EVIL_MAN_START, TEX_EVIL_MAN_END, "images/evil_man");
	loadAnimation(TEX_SUPER_BALL_START, TEX_SUPER_BALL_END, "images/super_ball");
	loadAnimation(TEX_FLY_BALL_START, TEX_FLY_BALL_END, "images/fly_ball");

	//Check for null textures
	for(unsigned i = 0; i < TEX_LAST; i++) {
		if(!textures[i]) return false;
	}
	
	//Load sounds
	sounds[SOUND_CREDITS_MUSIC]		= audio->loadSound("sounds/music1.ogg");
	sounds[SOUND_MUSIC_START]		= audio->loadSound("sounds/music2.ogg");
	sounds[SOUND_MUSIC_START + 1]	= audio->loadSound("sounds/music3.ogg");
	sounds[SOUND_MUSIC_START + 2]	= audio->loadSound("sounds/music4.ogg");
	sounds[SOUND_MUSIC_START + 3]	= audio->loadSound("sounds/music5.ogg");
	sounds[SOUND_MUSIC_START + 4]	= audio->loadSound("sounds/music6.ogg");
	sounds[SOUND_COIN]				= audio->loadSound("sounds/coin.ogg");
	sounds[SOUND_SQUISH_START]		= audio->loadSound("sounds/squish1.ogg");
	sounds[SOUND_SQUISH_START + 1]	= audio->loadSound("sounds/squish2.ogg");
	sounds[SOUND_SQUISH_START + 2]	= audio->loadSound("sounds/squish3.ogg");
	sounds[SOUND_SQUISH_START + 3]	= audio->loadSound("sounds/squish4.ogg");
	sounds[SOUND_JUMP]				= audio->loadSound("sounds/jump.ogg");
	sounds[SOUND_DIAMOND]			= audio->loadSound("sounds/diamond.ogg");
	sounds[SOUND_END_LEVEL]			= audio->loadSound("sounds/endlevel.ogg");
	sounds[SOUND_END_GAME]			= audio->loadSound("sounds/endgame.ogg");
	
	world.setWorldTexture(textures[TEX_WORLD_TILES]);
	player.setStartPosition(VEC2_TILE);

	mode = MODE_START;
	titleWidth = smallFont->getStringWidth(GAME_MESSAGE);
	
	return true;
}

//=============================================================================
// Shutdown
//=============================================================================
void GameLocal::shutdown()
{
	delete smallFont;
	delete medFont;
	delete bigFont;
	for(unsigned i = 0; i < TEX_LAST; i++) delete textures[i];
	for(unsigned i = 0; i < SOUND_LAST; i++) audio->unloadSound(sounds[i]);

	keyEventSender.disconnect(this);
	mouseEventSender.disconnect(this);
}

//=============================================================================
// Draws a game frame (60 fps)
//=============================================================================
void GameLocal::frame(unsigned elapsedTime)
{
	render->frameBegin();
	render->setup2D(SCREEN_MAX);

	switch(mode)
	{
	case MODE_START:	startLoop();	break;
#ifdef _DEBUG
	case MODE_EDITOR:	editorLoop();	break;
#endif //_DEBUG
	case MODE_GAME:		gameLoop();		break;
	case MODE_BRIEF:	briefLoop();	break;
	case MODE_CREDITS:	creditsLoop();	break;
	}

	render->frameEnd();
}

//=============================================================================
// Manages KeyEvents
//=============================================================================
void GameLocal::onEvent(KeyEvent &keyEvent)
{
	switch(keyEvent.getKeyCode())
	{
	#ifdef _DEBUG

	case KeyEvent::K_E: //Editor mode
		if(keyEvent.getKeyState() == KeyEvent::KEY_PRESS) {
			if(MODE_GAME == mode) {
				mode = MODE_EDITOR;
				log->print("Editor mode ON");
				for (unsigned i = 0; i < enemyCount; i++) enemies[i].respawn();
				system->setCursor(System::CURSOR_ARROW);
			} else if(MODE_EDITOR == mode) {
				mode = MODE_GAME;
				log->print("Editor mode OFF");
				player.setPosition(Vec2i(pixelShift, 0) + VEC2_TILE);
				player.spawn();
				system->setCursor(System::CURSOR_NONE);
			}
		}
		break;

	case KeyEvent::K_R: //Choose a tile
		if((MODE_EDITOR == mode) && (keyEvent.getKeyState() == KeyEvent::KEY_PRESS) && !addingEnemies) {
			choosingTile = !choosingTile;
		}
		break;

	case KeyEvent::K_S: //Change drawing mode
		if((MODE_EDITOR == mode) && (keyEvent.getKeyState() == KeyEvent::KEY_PRESS)) {
			switch(editorTile.type)
			{
			case TILE_NORMAL:		editorTile.type = TILE_SOLID;		break;
			case TILE_SOLID:		editorTile.type = TILE_LETHAL;		break;
			case TILE_LETHAL:		editorTile.type = TILE_COIN;		break;
			case TILE_COIN:			editorTile.type = TILE_ENEMY_STOP;	break;
			case TILE_ENEMY_STOP:	editorTile.type = TILE_DIAMOND;		break;
			case TILE_DIAMOND:		editorTile.type = TILE_END_LEVEL;	break;
			case TILE_END_LEVEL:	editorTile.type = TILE_NORMAL;		break;
			}
		}
		break;

	case KeyEvent::K_D: //Enter in add enemy mode
		if((MODE_EDITOR == mode) && (keyEvent.getKeyState() == KeyEvent::KEY_PRESS) && !choosingTile) {
			addingEnemies = !addingEnemies;
		}
		break;

	case KeyEvent::K_F:
		if((MODE_EDITOR == mode) && (keyEvent.getKeyState() == KeyEvent::KEY_PRESS) && !choosingTile) {
			if(addingEnemies) {
				enemyNo++;
				if(enemyNo >= MAX_ENEMIES) enemyNo = 0;
			}
		}
		break;

	case KeyEvent::K_G:
		if((MODE_EDITOR == mode) && (keyEvent.getKeyState() == KeyEvent::KEY_PRESS) && !choosingTile) {
			if(addingEnemies) {
				enemyNo--;
				if(enemyNo < 0) enemyNo = MAX_ENEMIES - 1;
			}
		}
		break;

	case KeyEvent::K_H: //Change enemy type
		if((MODE_EDITOR == mode)  && !choosingTile && (keyEvent.getKeyState() == KeyEvent::KEY_PRESS)) {
			if(addingEnemies) {
				switch(enemyType)
				{
				case Enemy::ENEMY_GREEN_BALL:	enemyType = Enemy::ENEMY_EVIL_MAN;		break;
				case Enemy::ENEMY_EVIL_MAN:		enemyType = Enemy::ENEMY_SUPER_BALL;	break;
				case Enemy::ENEMY_SUPER_BALL:	enemyType = Enemy::ENEMY_FLY_BALL;		break;
				case Enemy::ENEMY_FLY_BALL:		enemyType = Enemy::ENEMY_GREEN_BALL;	break;
				}
			}
		}
		break;

	case KeyEvent::K_L: //Shrink the level at the current position
		if((MODE_EDITOR == mode) && (keyEvent.getKeyState() == KeyEvent::KEY_PRESS)) {
			int newLenght = pixelShift / TILE_SIZE_PIXEL;
			world.shrink(newLenght);
			log->printf("Level shrinked at %i", newLenght);
		}
		break;
	
	case KeyEvent::K_BACKSPACE: //Reload the current level
		if((MODE_EDITOR == mode) && (keyEvent.getKeyState() == KeyEvent::KEY_PRESS)) {
			loadLevel(false);
		}
		break;
	
	case KeyEvent::K_PRIOR:
		if((MODE_EDITOR == mode) && (keyEvent.getKeyState() == KeyEvent::KEY_PRESS)) level++;
		break;

	case KeyEvent::K_NEXT:
		if((MODE_EDITOR == mode) && (keyEvent.getKeyState() == KeyEvent::KEY_PRESS) && (level > 1)) level--;
		break;
	
	case KeyEvent::MK_LEFT:
		if((MODE_EDITOR == mode)) {
			if(choosingTile) {	//Chosing a tile
				if(keyEvent.getKeyState() == KeyEvent::KEY_RELEASE) {
					if(screenPos.getX() < TEX_SIZE_PIXEL && screenPos.getY() < TEX_SIZE_PIXEL) {
						editorTile.tile1 = screenPos.getX() / TILE_SIZE_PIXEL + (screenPos.getY() / TILE_SIZE_PIXEL) * TEX_SIZE_TILES;
						choosingTile = false;
						editorTile.type = TILE_NORMAL; //Reset the type of the tile to normal
					}
				}
			} else {			//Drawing to the map
				if(keyEvent.getKeyState() == KeyEvent::KEY_RELEASE) {
					drawing1 = false;
				} else {
					drawing1 = true;
				}
			}
		}
		break;

	case KeyEvent::MK_RIGHT:
		if((MODE_EDITOR == mode) && !choosingTile) {
			if(keyEvent.getKeyState() == KeyEvent::KEY_RELEASE) {
				drawing2 = false;
			} else {
				drawing2 = true;
			}
		}
		break;

	#endif //_DEBUG

	case KeyEvent::K_ENTER: 
		#ifdef _DEBUG
		//Save the current level
		if((MODE_EDITOR == mode) && (keyEvent.getKeyState() == KeyEvent::KEY_PRESS)) {
			saveLevel();
			log->print("Level saved");
		}
		#endif //_DEBUG

		if(keyEvent.getKeyState() == KeyEvent::KEY_RELEASE) {
			if(MODE_START == mode) {
				mode = MODE_GAME;
				life = PLAYER_LIFE;
				playerScore = 0;
				level = 1;
				loadLevel(true);
			} else if(MODE_BRIEF == mode) {
				mode = MODE_CREDITS;
				creditsY = SCREEN_Y_PIXEL;
				creditsWait = 0;
				audio->stopSound(levelMusic);
				audio->playSound(sounds[SOUND_CREDITS_MUSIC]);
			} else if(MODE_CREDITS == mode) {
				mode = MODE_START;
				audio->stopSound(sounds[SOUND_CREDITS_MUSIC]);
			}
		}
		break;

	case KeyEvent::K_SPACE:
		if((MODE_GAME == mode) && (keyEvent.getKeyState() == KeyEvent::KEY_PRESS)) player.jump();
		break;

	case KeyEvent::K_LEFT:
		movingLeft = keyEvent.getKeyState() != KeyEvent::KEY_RELEASE;
		break;

	case KeyEvent::K_RIGHT:
		movingRight = keyEvent.getKeyState() != KeyEvent::KEY_RELEASE;
		break;
	}
}

//=============================================================================
// Manages MouseEvents
//=============================================================================
void GameLocal::onEvent(MouseEvent &mouseEvent)
{
	#ifdef _DEBUG
	mouseEvent.get(screenPos);

	//Adjust mouse position according to the resolution
	/*Vec2i resolution;
	render->getResolution(resolution);
	if((resolution.getX() != SCREEN_X_PIXEL) || (resolution.getY() != SCREEN_Y_PIXEL)) {
		float ratioX = SCREEN_X_PIXEL / static_cast<float>(resolution.getX());
		float ratioY = SCREEN_Y_PIXEL / static_cast<float>(resolution.getY());
		screenPos.setX(static_cast<int>(screenPos.getX() * ratioX));
		screenPos.setY(static_cast<int>(screenPos.getY() * ratioY));
	}*/
	#endif //_DEBUG
}

//=============================================================================
// Adds score to the player
//=============================================================================
void GameLocal::addScore(int score)
{
	playerScore += score;
}

//=============================================================================
// Loads the next level
//=============================================================================
void GameLocal::nextLevel()
{
	nextLevelTimeout = NEXT_LEVEL_TIME;
	audio->playSound(sounds[SOUND_END_LEVEL]);
}

//=============================================================================
// Loads multiple textures
//=============================================================================
void GameLocal::loadAnimation(unsigned start, unsigned end, const char *prefix)
{
	String name;
	unsigned len = end - start;
	for(unsigned i = 0; i <= len; i++) {
		name = prefix;
		name += static_cast<int>(i);
		name += ".png";
		textures[start + i] = Texture::create(Image(name));
	}
}

//=============================================================================
// Returns the name of the level file
//=============================================================================
String GameLocal::getLevelFileName()
{
	String levelName = "levels/";
	levelName.append(level);
	levelName.append(".bin");
	return levelName;
}

//=============================================================================
// Draws the menu background
//=============================================================================
void GameLocal::drawMenuBack()
{
	float m = 0;
	Color tmpColor;
	for(int y = 0; y <= SCREEN_Y_PIXEL; y += 10) {
		tmpColor.set(abs<float>(sin(menuBgColor + sin(m))),
			abs<float>(cos(menuBgColor - sin(m))),
			abs<float>(cos(menuBgColor + cos(m))), 1);
		render->setDrawColor(tmpColor);
		render->drawFilledRect(Vec2i(0, y), Vec2i(SCREEN_X_PIXEL, y + 10));
		m += 0.05f;
	}
	menuBgColor += 0.01f;
}

//=============================================================================
// Resets the game & the editor
//=============================================================================
void GameLocal::reset()
{
	#ifdef _DEBUG
	choosingTile = addingEnemies = false;
	drawing1 = drawing2 = false;
	enemyNo = 0;
	enemyType = Enemy::ENEMY_GREEN_BALL;
	Memory::memset(&editorTile, 0, sizeof(Tile));
	#endif //_DEBUG

	movingLeft = movingRight = false;
	reloadTimeout = nextLevelTimeout = -1;
	
	player.spawn();

	particleSystem.reset();
	pixelShift = 0;
}

//=============================================================================
// Moves the world
//=============================================================================
void GameLocal::moveWorld(int move)
{
	pixelShift += move;
	if(pixelShift < 0) pixelShift = 0;
}

//=============================================================================
// Start loop
//=============================================================================
void GameLocal::startLoop()
{
	drawMenuBack();
	render->setDrawColor(Color(1, 1, 1, 0.4f));
	render->drawFilledRect(SCREEN_MIN, SCREEN_MAX);
	render->drawFilledRect(Vec2i(0, SCREEN_Y_PIXEL - 50), SCREEN_MAX);

	render->setDrawColor(Color(0, 0, 0, 1));
	smallFont->draw(Vec2i(titleX, SCREEN_Y_PIXEL - 30), GAME_MESSAGE);

	render->setDrawColor(Color(1, 1, 1, 1));
	textures[TEX_POSTER]->drawTile(Vec2i(64, 0), Vec2i(512, 512));

	titleX--;
	if(titleX < (-titleWidth)) titleX = SCREEN_X_PIXEL;
}

//=============================================================================
// Editor loop
//=============================================================================
#ifdef _DEBUG
void GameLocal::editorLoop()
{
	if(choosingTile) {													//Choosing a tile
		render->setDrawColor(Color(0.8f, 0.8f, 1, 1));
		render->drawFilledRect(SCREEN_MIN, SCREEN_MAX);
		render->setDrawColor(Color(1, 1, 1, 1));
		textures[TEX_WORLD_TILES]->drawTile(
			SCREEN_MIN,
			Vec2i(TEX_SIZE_PIXEL, TEX_SIZE_PIXEL)
		);
	} else {
		if(drawing1 || drawing2) {
			Vec2i pixelPos = Vec2i(pixelShift, 0) + screenPos;
			Vec2i mapPos;
			world.pixelToMap(pixelPos, mapPos);
			Tile tile = world.getTile(mapPos);
			if(addingEnemies && drawing1) {							//Add enemies
				if(enemyNo <= enemyCount) {
					if(enemyNo == enemyCount) enemyCount = enemyNo + 1;
					enemies[enemyNo].setPosition(pixelPos);
					enemies[enemyNo].setStartPosition(pixelPos);
					enemies[enemyNo].spawn(enemyType);
				}
			} else {
				if(drawing1) {
					if(editorTile.type != TILE_NORMAL) {
						tile.type = editorTile.type;				//Foreground tile
					} else {
						tile.tile1 = editorTile.tile1;
						tile.type = TILE_NORMAL;					//Reset tile type
					}
				}
				else if(drawing2) tile.tile2 = editorTile.tile1;	//Background tile
		}

			world.setTile(mapPos, tile);
		}

		if(movingLeft && (pixelShift > 4)) {
			moveWorld(-8);
		}
		else if(movingRight) moveWorld(8);

		world.draw(pixelShift, true);

		render->setDrawColor(Color(1, 1, 1, 1));
		for(unsigned i = 0; i < enemyCount; i++) {
			if(enemies[i].isDead()) continue;
			enemies[i].draw(pixelShift);
			const Vec2i &pos = enemies[i].getPosition() - Vec2i(pixelShift, 0);
			smallFont->draw(pos, String(static_cast<int>(i)));
		}

		render->setDrawColor(Color(1, 1, 1, 1));
		String editorStr("EDITOR MODE level ");
		editorStr += level;
		smallFont->draw(Vec2i(5, 10), editorStr);

		if(addingEnemies) {
			String enemyStr("Adding enemy No. ");
			enemyStr += static_cast<int>(enemyNo);
			enemyStr += " type ";
			enemyStr += Enemy::getEnemyTypeName(enemyType);
			smallFont->draw(Vec2i(5, 26), enemyStr);
		} else {
			String drawMode("Draw mode = ");
			drawMode +=	TILE_TYPE_NAMES[editorTile.type];
			smallFont->draw(Vec2i(5, 26), drawMode);
		}

		String mapShiftStr("Map tile shift = ");
		mapShiftStr += pixelShift / TILE_SIZE_PIXEL;
		smallFont->draw(Vec2i(5, 42), mapShiftStr);
	}

}
#endif //_DEBUG

//=============================================================================
// Game loop
//=============================================================================
void GameLocal::gameLoop()
{
	//Update the particle system
	particleSystem.tick();

	//Move enemies
	for(unsigned i = 0; i < enemyCount; i++) {
		enemies[i].tick();
	}
	
	if(!player.isDead() && (nextLevelTimeout < 0)) {
		//Move player
		if(movingLeft) {
			if(player.getPosition().getX() - pixelShift < 192) moveWorld(-4);
			player.move(Vec2i(-4, 0));
		} else if(movingRight) {
			if(player.getPosition().getX() - pixelShift > 320) moveWorld(4);
			player.move(Vec2i(4, 0));
		}
		player.tick();

		//Player-enemy collision
		for(unsigned i = 0; i < enemyCount; i++) {
			if(enemies[i].isDead()) continue;
			Rect playerRect = player.getRect();
			Rect enemyRect = enemies[i].getRect();
			if(playerRect.intersects(enemyRect)) {
				if(enemies[i].isKillable() && (playerRect.getMax().getY() <= (enemyRect.getMin().getY() + 12)))
				{
					enemies[i].kill();
					playerScore += 1;
				}
				else player.kill();
			}
		}
	} else { //Player dead
		if(reloadTimeout < 0) {
			reloadTimeout = RELOAD_LEVEL_TIME;
		}
	}

	//Draw everything
	world.draw(pixelShift, false);
	render->setDrawColor(Color(1, 1, 1, 1));
	for(unsigned i = 0; i < enemyCount; i++) {
		if(!enemies[i].isDead()) {
			enemies[i].draw(pixelShift);
		}
	}
	if(!player.isDead()) player.draw(pixelShift);
	particleSystem.draw(pixelShift);

	//Player infos
	String lifeStr = ("Life x"), pointStr("Points x");
	lifeStr += static_cast<int>(life);
	pointStr += playerScore;
	render->setDrawColor(Color(0, 0, 0, 1));
	smallFont->draw(Vec2i(10, 10), lifeStr);
	smallFont->draw(Vec2i(10, 26), pointStr);
	render->setDrawColor(Color(1, 1, 1, 1));
	smallFont->draw(Vec2i(11, 11), lifeStr);
	smallFont->draw(Vec2i(11, 27), pointStr);

	//Load next level or the current one
	if(nextLevelTimeout == 0) {
		level++;
		if(LAST_LEVEL == level) mode = MODE_BRIEF; //End game
		else loadLevel(true);
	} else if(reloadTimeout == 0) {
		if(life <= 0) mode = MODE_BRIEF;
		else {
			life--; //Decrease player life
			loadLevel(false);
		}
	}

	//Level finished\failed effect
	if(nextLevelTimeout > 0) {
		nextLevelTimeout--;
		float alpha = (NEXT_LEVEL_TIME - nextLevelTimeout) / static_cast<float>(NEXT_LEVEL_TIME / 2);
		render->setDrawColor(Color(1, 1, 1, alpha));
		render->drawFilledRect(SCREEN_MIN, SCREEN_MAX);
	} else if(reloadTimeout > 0) {
		reloadTimeout--;
		float alpha = (RELOAD_LEVEL_TIME - reloadTimeout) / static_cast<float>(RELOAD_LEVEL_TIME);
		render->setDrawColor(Color(1, 0.1f, 0.1f, alpha));
		render->drawFilledRect(SCREEN_MIN, SCREEN_MAX);
	}
}

//=============================================================================
// Brief loop
//=============================================================================
void GameLocal::briefLoop()
{
	int finalScore = playerScore + life * 1000;
	String str("GAME OVER");
	String score("Final score ");
	if(life) {
		score += playerScore;
		score += " + ";
		score += life;
		score += " x 1000 = ";
		score += finalScore;
	} else {
		score += "= ";
		score += playerScore;
	}
	render->setDrawColor(Color(1, 1, 1, 1));
	int x = (SCREEN_X_PIXEL - medFont->getStringWidth(str)) / 2;
	medFont->draw(Vec2i(x, 200), str);
	x = (SCREEN_X_PIXEL - medFont->getStringWidth(score)) / 2;
	medFont->draw(Vec2i(x, 225), score);
	smallFont->draw(Vec2i(10, SCREEN_Y_PIXEL - 20), "Press ENTER to continue");
}

//=============================================================================
// Credits loop
//=============================================================================
void GameLocal::creditsLoop()
{
	int y = creditsY, n = 0;
	const char * const * line = CREDITS;

	while(*line) {
		if(y > -16) {
			if(y < 50) { //Fade effect
				float alpha = (y + 20) / 70.0f;
				if(4 == n) {
					render->setDrawColor(Color(1, alpha, alpha, alpha));
				} else {
					render->setDrawColor(Color(1, 1, 1, alpha));
				}
			} else {
				render->setDrawColor(Color(1, 1, 1, 1));
			}

			int x = (SCREEN_X_PIXEL - medFont->getStringWidth(*line)) / 2;
			medFont->draw(Vec2i(x, y), *line);
		} else if(y > SCREEN_Y_PIXEL) break; //Out of the screen

		y += medFont->getHeight();
		n++;
		line++;
	};

	if(y < - 16) {
		audio->stopSound(sounds[SOUND_CREDITS_MUSIC]);
		mode = MODE_START;
		titleX = SCREEN_X_PIXEL;
	}

	if(0 == creditsWait) {
		creditsY--;
		creditsWait = 4;
	} else creditsWait--;
}

//=============================================================================
// Loads the level
//=============================================================================
bool GameLocal::loadLevel(bool changeMusic)
{
	reset();

	enemyCount = 0;
	
	File *file = File::createFile();
	if(file->open(getLevelFileName(), File::OPENMODE_INPUT)) {
		world.readFromFile(file);
		file->readUnsignedInt(enemyCount);
		for(unsigned i = 0; i < enemyCount; i++) {
			enemies[i].readFromFile(file);
			enemies[i].respawn();
		}
		file->close();
		log->printf("Level %i loaded", level);
	} else {
		#ifdef _DEBUG
		if((MODE_EDITOR == mode)) {
			world.make(1000);
			log->printf("Created new level %i", level);
		} else
		#endif //_DEBUG
		log->printf("Level %i not found", level);
	}
	delete file;

	if(changeMusic) {
		if(levelMusic) {
			audio->stopSound(levelMusic);
			levelMusic++;
			if(levelMusic > sounds[SOUND_MUSIC_END]) {
				levelMusic = sounds[SOUND_MUSIC_START];
			}
		} else {
			levelMusic = sounds[SOUND_MUSIC_START + random.getUnsigned(SOUND_MUSIC_END - SOUND_MUSIC_START)];
		}
		audio->loopSound(levelMusic);
		audio->setSoundGain(levelMusic, 0.2f);
	}

	return false;
}

//=============================================================================
// Saves the level
//=============================================================================
#ifdef _DEBUG
void GameLocal::saveLevel()
{
	File *file = File::createFile();
	if(file->open(getLevelFileName(), File::OPENMODE_OUTPUT)) {
		world.writeToFile(file);
		file->writeUnsignedInt(enemyCount);
		for(unsigned i = 0; i < enemyCount; i++) enemies[i].writeToFile(file);
		file->close();
	}

	delete file;
}
#endif //_DEBUG