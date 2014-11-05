#pragma once
#ifndef __ENTITY_H__
#define __ENTITY_H__


#include <Math/Rect.h>
#include <Math/Vector.h>
#include <Render/Render.h>

using namespace myengine;


class Entity
{
public:
		Entity();
		//~Entity();

	bool			isDead() const;

	const Vec2i		&getPosition() const;
	const Vec2i		&getSize() const;
	Rect			getRect() const;

	void			setPosition(const Vec2i &position);
	void			setStartPosition(const Vec2i &position);
	
	virtual bool	checkCollision() = 0;
	bool			move(const Vec2i &movement);

	void			draw(int pixelShift);

	void			readFromFile(File *file);
	void			writeToFile(File *file) const;


protected:
	void		spawn();
	void		kill();

	void			setSize(const Vec2i &size);
	void			setTexture(unsigned start, unsigned len);

	bool			applyGravity();


private:
	bool		dead;
	Vec2i		startPosition, position;
	Vec2i		size;
	
	unsigned	start, len;
	unsigned	frame, wait;
	bool		flip;
};


#endif //!__ENTITY_H__