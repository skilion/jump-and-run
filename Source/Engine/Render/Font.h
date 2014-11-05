#pragma once
#ifndef __FONT_H__
#define __FONT_H__


#include "Texture.h"

#include <Containers/Array.h>


namespace myengine {

	const char GLYPHS_START	= ' ';
	const char GLYPHS_END	= '~';
	const char GLYPHS_COUNT	= GLYPHS_END - GLYPHS_START;
		
	class Font
	{
	public:
			//Font();
			virtual ~Font();

		static Font		*create(const char *filename, int size, bool antialiasing);

		void			draw(const Vec2i &position, const char *string) const;

		int				getHeight() const;
		int				getStringWidth(const char *string) const;

		int				getCharIndex(const char *string, int position) const;
		int				getCharPosition(const char *string, int index) const;
		

	protected:
		bool			isPrintable(char c) const;
		virtual int		getKerning(char left, char right) const = 0;

		struct Glyph {
			int		textureId;
			float	s1, t1, s2, t2;		//Font position in the texture (in pixel)
			int		width, height;		//Glyph size (in pixel)
			int		adjustX, adjustY;	//Center the bitmap
			int		advance;
		} glyphs[GLYPHS_COUNT];

		int		height;  //Font height in pixel

		Array<Texture*>	textures;
	};

}


#endif //!__FONT_H__