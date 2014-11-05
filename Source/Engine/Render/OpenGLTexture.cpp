#include "OpenGLHeader.h" //

#include "Log.h"
#include "Image.h"
#include "OpenGLRender.h"
#include "OpenGLTexture.h"

#include "Math/Math.h"

#include "memdbg.h"

using namespace myengine;


//=============================================================================
// Creates a Texture class
//=============================================================================
Texture *Texture::create(const Image &image)
{
	return new OpenGLTexture(image);
}

//=============================================================================
// Constructor
//=============================================================================
OpenGLTexture::OpenGLTexture(const Image &image)
{
	texture = 0;
	load(image);
}

//=============================================================================
// Destructor
//=============================================================================
OpenGLTexture::~OpenGLTexture()
{
	glDeleteTextures(1, &texture);
}

//=============================================================================
// Draws a tile
//=============================================================================
void OpenGLTexture::drawTile(int x, int y, int width, int height)
{
	int x2 = x + width;
	int y2 = y + height;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
#ifdef OPENGL_TEXTURE_LEGACY
	glTexCoord2f(0, 0);	glVertex2i( x,  y);
	glTexCoord2f(0, h);	glVertex2i( x, y2);
	glTexCoord2f(w, h);	glVertex2i(x2, y2);
	glTexCoord2f(w, 0);	glVertex2i(x2,  y);
#else
	glTexCoord2i(0, 0);	glVertex2i( x,  y);
	glTexCoord2i(0, 1);	glVertex2i( x, y2);
	glTexCoord2i(1, 1);	glVertex2i(x2, y2);
	glTexCoord2i(1, 0);	glVertex2i(x2,  y);
#endif
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void OpenGLTexture::drawTile(int x, int y, int width, int height, float s1, float t1, float s2, float t2)
{
	int x2 = x + width;
	int y2 = y + height;
	
#ifdef OPENGL_TEXTURE_LEGACY
	s1 *= w;
	s2 *= w;
	t1 *= h;
	t2 *= h;
#endif

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glTexCoord2f(s1, t1);	glVertex2i( x,  y);
	glTexCoord2f(s1, t2);	glVertex2i( x, y2);
	glTexCoord2f(s2, t2);	glVertex2i(x2, y2);
	glTexCoord2f(s2, t1);	glVertex2i(x2,  y);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

 void OpenGLTexture::drawTile(const Vec2i &position, const Vec2i &size)
 {
	 drawTile(position.getX(), position.getY(), size.getX(), size.getY());
 }

void OpenGLTexture::drawTile(const Vec2i &position, const Vec2i &size, const Vec2f &st1, const Vec2f &st2)
{
	drawTile(position.getX(), position.getY(), size.getX(), size.getY(), st1.getX(), st1.getY(), st2.getX(), st2.getY());
}

//=============================================================================
// Loads a texture
// Note: OpenGL expects the images upside-down, but we upload them right up anyway
//=============================================================================
void OpenGLTexture::load(const Image &image)
{
	GLenum format;
	switch(image.getFormat())
	{
	case Image::IMAGE_EMPTY:
		log->print("Tried to load an empty image");
		return;

	case Image::IMAGE_RGB:
		format = GL_RGB;
		break;

	case Image::IMAGE_RGBA:
		format = GL_RGBA;
		break;

	case Image::IMAGE_ALPHA:
		format = GL_ALPHA;
		break;

	default:
		log->print("Tried to load an unsupported image format in OpenGL");
		return;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//Texture filtering
	/*switch(r_textureFilter.getInt())
	{
	default:
		r_textureFilter = 1;
		log->print("r_textureFiltering has an invalid value");

	case 0:*/
		//Point sampling
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//break;

	//case 1:
		//Bilinear sampling
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//break;
	//}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//GL_REPEAT);

	int width = image.getWidth();
	int height = image.getHeight();
	
#ifdef OPENGL_TEXTURE_LEGACY
	if((width != height) || !isPow2(width)) {
		int bytesPerPixel;
		switch(image.getFormat())
		{
		case Image::IMAGE_RGB:
			bytesPerPixel = 3;
			break;

		case Image::IMAGE_RGBA:
			bytesPerPixel = 4;
			break;

		default:
			return;
		}

		unsigned size = nextPow2(max<int>(width, height));

		pixel2lengthX = pixel2lengthY = 1 / static_cast<float>(size);

		//Texture coordinates inside the texture !
		w = static_cast<float>(width) / size;
		h = static_cast<float>(height) / size;

		unsigned sizeBytes = size * bytesPerPixel;
		unsigned widthBytes = width * bytesPerPixel;

		byte *data = new byte[size * sizeBytes], *pData = data;
		const byte *pImage = image.getImage();

		//Copy the image
		for(int i = 0; i < height; i++) {
			Memory::memcpy(pData, image.getImage(), widthBytes);
			pData += sizeBytes;
			pImage += widthBytes;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, format, size, size, 0, format, GL_UNSIGNED_BYTE, data);
		delete [] data;
	} else {
		w = h = 1;
	}
#endif

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image.getImage());
}