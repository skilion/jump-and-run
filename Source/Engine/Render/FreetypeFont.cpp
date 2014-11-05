#include "Log.h"
#include "Image.h"
#include "Memory.h"
#include "Render.h"
#include "Texture.h"
#include "Platform.h"
#include "FreetypeFont.h"

#include FT_LCD_FILTER_H

#include "memdbg.h"

using namespace myengine;


FT_Library FreetypeFont::library = 0;


//=============================================================================
// Creates a Font class
//=============================================================================
Font *Font::create(const char *filename, int size, bool antialiasing)
{
	return new FreetypeFont(filename, size, antialiasing);
}

//=============================================================================
// Constructor
//=============================================================================
FreetypeFont::FreetypeFont(const char *filename, int size, bool antialiasing)
{
	if(!library) {
		if(FT_Init_FreeType(&library)) {
			log->print("Can't initialize Freetype library...");
			throw;
		}
		FT_Library_SetLcdFilter(library, FT_LCD_FILTER_LIGHT);
	}

	face = 0;

	load(filename, size, antialiasing);
}

//=============================================================================
// Destructor
//=============================================================================
FreetypeFont::~FreetypeFont()
{
	FT_Done_Face(face);
	for(int i = 0; i <= glyphs[GLYPHS_COUNT - 1].textureId; i++) { //HACK
		delete textures[i];
	}
}

//=============================================================================
// Loads the font from a file
//=============================================================================
bool FreetypeFont::load(const char *filename, int size, bool antialiasing)
{
	FT_Error error = 0;

	//Create a font face
	error = FT_New_Face(library, filename, 0, &face);
	if(error) {
		log->printf("FreeType can't load this font file: %s", filename);
		return false;
	}

	if(!face->charmap) {
		log->printf("This font file %s does not contain a unicode charmap", filename);
		return false;
	}

	//Select the font size
	if(size < 8) size = 8;
	if(size > 250) size = 150;
	FT_Set_Pixel_Sizes(face, size, size);

	height = size;
	
	//Create a RGB bitmap for the font
	const int fontmapSize = 256;
	float aspect = 1.0f / fontmapSize;
	byte *fontmap = new byte[fontmapSize * fontmapSize];
	Memory::memset(fontmap, 0, fontmapSize * fontmapSize);

	//Set the Freetype load flags
	int ftLoadFlag = FT_LOAD_RENDER;
	ftLoadFlag |= antialiasing ? FT_LOAD_TARGET_LCD : FT_LOAD_TARGET_MONO;

	//Fill the fontmaps
	unsigned textureId = 0;
	int fontmapX = 0;
	int fontmapY = 0;
	int fontmapLineHeight = 0;
	for(unsigned i = 0; i < GLYPHS_COUNT; i++)
	{
		FT_Load_Char(face, i + GLYPHS_START, ftLoadFlag);

		FT_GlyphSlot slot = face->glyph;
		FT_Bitmap *bitmap = &slot->bitmap;
		
		//Check if the character fits in the fontmap
		if(bitmap->width > (fontmapSize - fontmapX)) {
			fontmapX = 0;
			fontmapY += fontmapLineHeight + 1;
		}
		
		//Keep the maximum character height for this line in the fontmap
		if(fontmapLineHeight < bitmap->rows) {
			fontmapLineHeight = bitmap->rows;
		}

		//Check if the character fits in the fontmap
		if(fontmapY > (fontmapSize - fontmapLineHeight)) {
			fontmapY = 0;
			fontmapLineHeight = bitmap->rows;
				
			Image image(fontmap, Image::IMAGE_ALPHA, fontmapSize, fontmapSize);
					
			//Save the texture
			textures.ensureSize(textureId + 1);
			textures[textureId] = Texture::create(image);
			textureId++;

			Memory::memset(fontmap, 0, fontmapSize * fontmapSize);
		}

		//Copy the character in the fontmap
		unsigned char *pBitmap = bitmap->buffer, *pFontmap = fontmap;
		pFontmap += fontmapX + fontmapY * fontmapSize;
		if(antialiasing)	//Grey bitmap
		{
			for(int j = 0; j < bitmap->rows; j++) {
				Memory::memcpy(pFontmap, pBitmap, bitmap->width);
				pBitmap += bitmap->pitch;
				pFontmap += fontmapSize;
			}
		}
		else				//Monochrome bitmap
		{
			for(int j = 0; j < bitmap->rows; j++) {
				for(int k = 0; k < bitmap->width; k++) {
					bool pixel = ((pBitmap[k / 8] << (k % 8)) & 0x80) != 0;
					pFontmap[k] = pixel ? 0xFF : 0;
				}
				pBitmap += bitmap->pitch;
				pFontmap += fontmapSize;
			}
		}

		Glyph &glyph = glyphs[i];

		//Save the texture id
		glyph.textureId = textureId;

		//Save the character position
		glyph.s1 = aspect * fontmapX;
		glyph.t1 = aspect * fontmapY;
		glyph.s2 = aspect * (fontmapX + bitmap->width);
		glyph.t2 = aspect * (fontmapY + bitmap->rows);

		//Save the character size
		if(antialiasing) { //Antialiased font are 3 times wider
			glyph.width = bitmap->width / 3;
		} else {
			glyph.width = bitmap->width;
		}
		glyph.height = bitmap->rows;

		//if(bitmap->rows != slot->bitmap_top) throw;
		//if(bitmap->width != slot->bitmap_left) throw;

		glyph.adjustX = slot->bitmap_left;
		glyph.adjustY = size - slot->bitmap_top;
		glyph.advance = slot->advance.x >> 6;

		/*glNewList(listbase + i, GL_COMPILE);
			glBindTexture(GL_TEXTURE_2D, texture);
			glPushMatrix();

			//Horizontal center the character and align the character at the bottom
			glTranslatef(static_cast<float>(slot->bitmap_left), static_cast<float>(size - slot->bitmap_top), 0);

			glBegin(GL_QUADS);
			float x1 = static_cast<float>(fontmapX) / static_cast<float>(fontmapWidth);
			float y1 = static_cast<float>(fontmapY) / static_cast<float>(fontmapHeight);
			float x2 = static_cast<float>(fontmapX + bitmap->width) / static_cast<float>(fontmapWidth);
			float y2 = static_cast<float>(fontmapY + bitmap->rows) / static_cast<float>(fontmapHeight);
			glTexCoord2f(x1, y1);	glVertex2i(0, 0);
			glTexCoord2f(x1, y2);	glVertex2i(0, bitmap->rows);
			if(antialiasing) {
				//Antialiased font are 3 times wider
				glTexCoord2f(x2, y2);	glVertex2i(bitmap->width / 3, bitmap->rows);
				glTexCoord2f(x2, y1);	glVertex2i(bitmap->width / 3, 0);
			} else {
				glTexCoord2f(x2, y2);	glVertex2i(bitmap->width, bitmap->rows);
				glTexCoord2f(x2, y1);	glVertex2i(bitmap->width, 0);
			}
			glEnd();

			glPopMatrix();

			//Move to draw the next glyph
			glTranslatef(static_cast<float>(slot->advance.x >> 6), 0, 0);
		glEndList();*/

		//Move the pen for the next character
		fontmapX += bitmap->width;
	}

	//Save the last fontmap
	if(fontmapX != 0 || fontmapY != 0) {
		Image image(fontmap, Image::IMAGE_ALPHA, fontmapSize, fontmapSize);
		textures.ensureSize(textureId + 1);
		textures[textureId] = Texture::create(image);
	}

	delete [] fontmap;

	return true;
}

//=============================================================================
// Returns the kerning on the X axis
//=============================================================================
int FreetypeFont::getKerning(char left, char right) const
{
	//if(kerning = FT_HAS_KERNING(face) != 0;);
	FT_Vector kerning;
	FT_Get_Kerning(face, FT_Get_Char_Index(face, left), FT_Get_Char_Index(face, right), FT_KERNING_DEFAULT, &kerning);
	return kerning.x >> 6;
}
