#pragma once
#ifndef __IMAGE_H__
#define __IMAGE_H__


//=============================================================================
//
//		Class for image storing
//
//=============================================================================


#include "Platform.h"


namespace myengine {

	class Image
	{
	public:
		enum ImageFormat {
			IMAGE_EMPTY,
			IMAGE_RGB,		// 1 pixel = 3 bytes
			IMAGE_RGBA,		// 1 pixel = 4 bytes
			IMAGE_ALPHA		// 1 pixel = 1 byte
		};

			Image();
			Image(const char *filename);
			Image(byte *image, ImageFormat format, int width, int height);
			~Image();

		bool			load(const char *filename);
		void			free();

		bool			isEmpty() const;

		const byte		*getImage() const;

		unsigned		getSize() const;
		ImageFormat		getFormat() const;
		int				getHeight() const;
		int				getWidth() const;


	private:
		bool			loadBitmap(byte *rawData, unsigned size);
		bool			loadJpeg(byte *rawData, unsigned size);
		bool			loadPng(byte *rawData, unsigned size);

		byte			*data;
		ImageFormat		format;
		int				width, height;
		
		static const byte	bmpMagic[];
		static const byte	jpgMagic[];
		static const byte	pngMagic[];
	};

}


#endif //!__IMAGE_H__
