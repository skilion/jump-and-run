#include <Math/Vector.h>

#include "Font.h"

#include "memdbg.h"

using namespace myengine;


//=============================================================================
// Constructor
//=============================================================================
//Font::Font() {}

//=============================================================================
// Destructor
//=============================================================================
Font::~Font() {}

//=============================================================================
// Draws a string using a specified font
//=============================================================================
void Font::draw(const Vec2i &position, const char *string) const
{
	//textures[0]->drawTile(0, 0, 256, 256);
	//return;
	int x, y;
	position.get(x, y);

	char previous = 0;
	while(*string) {
		if(isPrintable(*string)) {
			const Glyph &glyph = glyphs[*string - GLYPHS_START];

			textures[glyph.textureId]->drawTile(x + glyph.adjustX, y + glyph.adjustY, glyph.width, glyph.height, glyph.s1, glyph.t1, glyph.s2, glyph.t2);
			
			x += glyph.advance + getKerning(previous, *string);
			previous = *string;
			
		}
		string++;
	}
}

//=============================================================================
// Returns the font height in pixel
//=============================================================================
int Font::getHeight() const
{
	return height;
}

//=============================================================================
// Returns the width of the specified string in pixel
//=============================================================================
int Font::getStringWidth(const char *string) const
{
	int width = 0;

	char previous = 0;
	while(*string) {
		if(isPrintable(*string)) {
			width += glyphs[*string - GLYPHS_START].advance;
			width += getKerning(previous, *string);
			previous = *string;
		}
		string++;
	}

	return width;
}

//=============================================================================
// Get the index of the character at the specified position in pixel
//=============================================================================
int Font::getCharIndex(const char *string, int position) const
{
	/*int index = -1;
	int currentPos = 0;

	char previous = 0;
	const char *cp = string;

	while(*cp) {
		if(isPrintable(*cp)) {
			if(position <= currentPos) break;

			currentPos += width[*cp - PRINTABLE_ASCII_START];
			currentPos += getKerning(previous, *cp);
			previous = *cp;
		}

		cp++;
		index++;
	}

	return index;*/
	return 0;
}

//=============================================================================
// Get the position of the character at the specified index
//=============================================================================
int Font::getCharPosition(const char *string, int index) const
{
	/*int position = 0;

	char previous = 0;
	while(*string && index) {
		if(isPrintable(*string)) {
			position += width[*string - PRINTABLE_ASCII_START];
			position += getKerning(previous, *string);
			previous = *string;
			index--;
		}
		string++;
	}

	return position;*/
	return 0;
}

//=============================================================================
// Returns true if the character is printable
//=============================================================================
bool Font::isPrintable(char c) const
{
	if(c < GLYPHS_START)	return false;
	if(c > GLYPHS_END)		return false;
	return true;
}