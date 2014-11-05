#pragma once
#ifndef __OPENGL_TEXTURE_H__
#define __OPENGL_TEXTURE_H__


#include "Texture.h"


//OpenGL supports non power-of-two texture since version 2.0
//#define OPENGL_TEXTURE_LEGACY


namespace myengine {

	class OpenGLTexture: public Texture
	{
	public:
			OpenGLTexture(const Image &image);
			~OpenGLTexture();

		//Texture interface
		virtual void	drawTile(int x, int y, int width, int height);
		virtual void	drawTile(int x, int y, int width, int height, float s1, float t1, float s2, float t2);
		virtual void	drawTile(const Vec2i &position, const Vec2i &size);
		virtual void	drawTile(const Vec2i &position, const Vec2i &size, const Vec2f &st1, const Vec2f &st2);

	private:
		void		load(const Image &image);
		
		unsigned	texture;
		
#ifdef OPENGL_TEXTURE_LEGACY
		float	w, h;
#endif
	};
	
}


#endif //!__OPENGL_TEXTURE_H__