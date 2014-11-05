#include <File/File.h>
#include <Math/Math.h>
#include <Platform.h>

#include "Color.h"

using namespace myengine;


//=============================================================================
// Constructor
//=============================================================================
Color::Color()
{
	for(unsigned i = 0; i < 4; i++) rgba[i] = 0;
}

Color::Color(const float rgba[4])
{
	for(unsigned i = 0; i < 4; i++) this->rgba[i] = rgba[i];
}

Color::Color(float red, float green, float blue, float alpha)
{
	rgba[0] = red;
	rgba[1] = green;
	rgba[2] = blue;
	rgba[3] = alpha;
}

//=============================================================================
// Destructor
//=============================================================================
//Color::~Color() {}

//=============================================================================
// Overload of conversion operator float*
//=============================================================================
Color::operator float*()
{
	return rgba;
}

Color::operator const float*() const
{
	return rgba;
}

//=============================================================================
// Overload of operator ==
//=============================================================================
bool Color::operator==(const Color &color) const
{
	return (
		fequal(rgba[0], rgba[0]) &&
		fequal(rgba[1], rgba[1]) &&
		fequal(rgba[2], rgba[2]) &&
		fequal(rgba[3], rgba[3])
	);
}

//=============================================================================
// Overload of operator !=
//=============================================================================
bool Color::operator!=(const Color &color) const
{
	return !operator==(color);
}

//=============================================================================
// Returns a pointer to the color array
//=============================================================================
const float *Color::get() const
{
	return rgba;
}

//=============================================================================
// Returns the color components
//=============================================================================
/*void Color::get(float &red, float &green, float &blue, float &alpha) const
{
	red = getRed();
	green = getGreen();
	blue = getBlue();
	alpha = getAlpha();
}*/

//=============================================================================
// Returns the red component
//=============================================================================
float Color::getRed() const
{
	return rgba[0];
}

//=============================================================================
// Returns the green component
//=============================================================================
float Color::getGreen() const
{
	return rgba[1];
}

//=============================================================================
// Returns the blue component
//=============================================================================
float Color::getBlue() const
{
	return rgba[2];
}

//=============================================================================
// Returns the alpha component
//=============================================================================
float Color::getAlpha() const
{
	return rgba[3];
}

//=============================================================================
// Sets the color
//=============================================================================
void Color::set(const float *rgba)
{
	for(unsigned i = 0; i < 4; i++) this->rgba[i] = rgba[i];
}

void Color::set(float red, float green, float blue, float alpha)
{
	rgba[0] = red;
	rgba[1] = green;
	rgba[2] = blue;
	rgba[3] = alpha;
}

//=============================================================================
// Sets the red component
//=============================================================================
void Color::setRed(float red)
{
	rgba[0] = red;
}

//=============================================================================
// Sets the green component
//=============================================================================
void Color::setGreen(float green)
{
	rgba[1] = green;
}

//=============================================================================
// Sets the blue component
//=============================================================================
void Color::setBlue(float blue)
{
	rgba[2] = blue;
}

//=============================================================================
// Sets the alpha component
//=============================================================================
void Color::setAlpha(float alpha)
{
	rgba[3] = alpha;
}

//=============================================================================
// Returns a R8G8B8A8 color
//=============================================================================
unsigned Color::toR8G8B8A8()
{
	unsigned color = static_cast<byte>(rgba[3] * 255);
	color |= static_cast<byte>(rgba[2] * 255) << 8;
	color |= static_cast<byte>(rgba[1] * 255) << 16;
	color |= static_cast<byte>(rgba[0] * 255) << 24;
	return color;
}

//=============================================================================
// Read the color from a file
//=============================================================================
void Color::readFromFile(File *file)
{
	file->read(reinterpret_cast<byte*>(rgba), sizeof(rgba));
}

//=============================================================================
// Saves the color to a file
//=============================================================================
void Color::writeToFile(File *file) const
{
	file->write(reinterpret_cast<const byte*>(rgba), sizeof(rgba));
}
