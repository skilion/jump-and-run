#pragma once

#include <File/File.h>
#include <Platform.h>

#include "Memory.h"


namespace myengine {

	class Color
	{
	public:
			Color();
			Color(const float rgba[4]);
			Color(float red, float green, float blue, float alpha = 1.0f);
			//~Color();
			
		operator float*();
		operator const float*() const;

		bool			operator==(const Color &color) const;
		bool			operator!=(const Color &color) const;

		const float		*get() const;
		//void			get(float &red, float &green, float &blue, float &alpha) const;
		float			getRed() const;
		float			getGreen() const;
		float			getBlue() const;
		float			getAlpha() const;

		void			set(const float rgba[4]);
		void			set(float red, float green, float blue, float alpha = 1.0f);
		void			setRed(float red);
		void			setGreen(float green);
		void			setBlue(float blue);
		void			setAlpha(float alpha);

		unsigned		toR8G8B8A8();

		void			readFromFile(File *file);
		void			writeToFile(File *file) const;


	private:
		float	rgba[4]; //R32G32B32A32_FLOAT
	};

}
