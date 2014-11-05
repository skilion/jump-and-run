#pragma once
#ifndef __FREETYPE_FONT_H__
#define __FREETYPE_FONT_H__


#include <ft2build.h>
#include FT_FREETYPE_H

#include "Font.h"


namespace myengine {

	class FreetypeFont: public Font
	{
	public:
			FreetypeFont(const char *filename, int size, bool antialiasing);
			~FreetypeFont();


	private:
		bool			load(const char *filename, int size, bool antialiasing);

		virtual int		getKerning(char left, char right) const;
		
		FT_Face			face;
		
		static FT_Library	library;	
	};

}


#endif //!__FREETYPE_FONT_H__