#pragma once
#ifndef __TEXTURE_H__
#define __TEXTURE_H__


namespace myengine {

	template<unsigned SIZE, typename T> class Vector;
	typedef Vector<2, int> Vec2i;
	typedef Vector<2, float> Vec2f;
	class Image;
	
	class Texture
	{
	public:
			//Texture();
			virtual ~Texture();

		static Texture	*create(const Image &image);

		virtual void	drawTile(int x, int y, int width, int height) = 0;
		virtual void	drawTile(int x, int y, int width, int height, float s1, float t1, float s2, float t2) = 0;
		virtual void	drawTile(const Vec2i &position, const Vec2i &size) = 0;
		virtual void	drawTile(const Vec2i &position, const Vec2i &size, const Vec2f &st1, const Vec2f &st2) = 0;
	};
	
}


#endif //!__TEXTURE_H__