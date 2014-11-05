#include <png.h>
#include <setjmp.h>
#include <jpeglib.h>

#include "Log.h"
#include "Image.h"
#include "Memory.h"

#include "File/File.h"
#include "Math/Math.h"

#include "memdbg.h"

using namespace myengine;


void	jpeg_output_message_override(j_common_ptr cinfo);
void	jpeg_error_exit_override(j_common_ptr cinfo);
void	png_error_override(png_structp png_ptr, png_const_charp error_message);
void	png_read_data_override(png_structp png_ptr, png_bytep data, png_size_t lenght);
void	png_warning_override(png_structp png_ptr, png_const_charp warning_message);

jmp_buf jpeg_error_jmp;
unsigned png_read_count;

//=============================================================================
// Overrides the default jpeg message function
//=============================================================================
void jpeg_output_message_override(j_common_ptr cinfo)
{
	char buffer[JMSG_LENGTH_MAX];
	(*cinfo->err->format_message) (cinfo, buffer);
	log->printf("(libjpeg) %s", buffer);
}

//=============================================================================
// Overrides the default jpeg error function
//=============================================================================
void jpeg_error_exit_override(j_common_ptr cinfo)
{
	jpeg_output_message_override(cinfo);
	jpeg_destroy(cinfo);
	longjmp(jpeg_error_jmp, 1);
}

//=============================================================================
// Overrides the default png error function
//=============================================================================
void png_error_override(png_structp png_ptr, png_const_charp error_message)
{
	log->printf("(libpng) %s", error_message);
	longjmp(png_jmpbuf(png_ptr), 1);
}

//=============================================================================
// Overrides the default png read function
//=============================================================================
void png_read_data_override(png_structp png_ptr, png_bytep data, png_size_t lenght)
{
	Memory::memcpy(data, static_cast<byte*>(png_get_io_ptr(png_ptr)) + png_read_count, lenght);
	png_read_count += lenght;
}

//=============================================================================
// Overrides the default png warning function
//=============================================================================
void png_warning_override(png_structp png_ptr, png_const_charp warning_message)
{
	log->printf("(libpng) %s", warning_message);
}


const byte Image::bmpMagic[] = {'B', 'M'};
const byte Image::jpgMagic[] = {0xFF, 0xD8};
const byte Image::pngMagic[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};


//=============================================================================
// Constructor
//=============================================================================
Image::Image()
{
	data = nullptr;
	format = IMAGE_EMPTY;
	height = width = 0;
}

Image::Image(const char *filename)
{
	data = nullptr;
	format = IMAGE_EMPTY;
	load(filename);
}

Image::Image(byte *image, ImageFormat format, int width, int height)
{
	this->format = format;

	if(format == IMAGE_EMPTY) {
		data = nullptr;
	} else {
		this->width		= width;
		this->height	= height;
		int size		= getSize();

		data = new byte[size];
		Memory::memcpy(data, image, size);
	}	
}

//=============================================================================
// Destructor
//=============================================================================
Image::~Image()
{
	delete [] data;
}

//=============================================================================
// Loads an image
//=============================================================================
bool Image::load(const char *filename)
{
	if(!isEmpty()) free();

	unsigned fileSize;
	byte *fileData = 0;

	if(!File::readFile(filename, &fileData, fileSize)) {
		log->printf("Image not found: %s", filename);
		return false;
	}

	//Discover what type of image it is
	bool returnCode = false;
	if(0 == Memory::memcmp(fileData, bmpMagic, sizeof(bmpMagic)))
		returnCode = loadBitmap(fileData, fileSize);
	else if(0 == Memory::memcmp(fileData, jpgMagic, sizeof(jpgMagic)))
		returnCode = loadJpeg(fileData, fileSize);
	else if(0 == Memory::memcmp(fileData, pngMagic, sizeof(pngMagic)))
		returnCode = loadPng(fileData, fileSize);
	
	delete [] fileData;

	if(!returnCode) log->printf("Unsupported image: %s", filename);

	return returnCode;
}

//=============================================================================
// Deletes the image
//=============================================================================
void Image::free()
{
	delete [] data;
	data = nullptr;
	format = IMAGE_EMPTY;
	height = width = 0;
}

//=============================================================================
// Returns the data of the image
//=============================================================================
const byte *Image::getImage() const
{
	return data;
}

//=============================================================================
// Returns the size of the image in bytes
//=============================================================================
unsigned Image::getSize() const
{
	unsigned bytesPerPixel;
	switch(format)
	{
	case IMAGE_RGB:
		bytesPerPixel = 3;
		break;

	case IMAGE_RGBA:
		bytesPerPixel = 4;
		break;

	case IMAGE_ALPHA:
		bytesPerPixel = 1;
		break;

	default:
		return 0;
	};

	return width * height * bytesPerPixel;
}

//=============================================================================
// Returns the format of the image
//=============================================================================
Image::ImageFormat Image::getFormat() const
{
	return format;
}

//=============================================================================
// Returns the height of the image
//=============================================================================
int Image::getHeight() const
{
	return height;
}

//=============================================================================
// Returns the width of the image
//=============================================================================
int Image::getWidth() const
{
	return width;
}

//=============================================================================
// Returns if the image is empty
//=============================================================================
bool Image::isEmpty() const
{
	return format == IMAGE_EMPTY;
}

//=============================================================================
// Load a bitmap image
//=============================================================================
bool Image::loadBitmap(byte *rawData, unsigned size)
{
	struct BmpHeader { //10 bytes
		udword	fileSize;
		uword	creator1;
		uword	creator2;
		udword	offset;
	} *bmpHeader = reinterpret_cast<BmpHeader*>(rawData + 2);

	if(bmpHeader->fileSize != size)	{ //Corrupted file
		log->printf("Bitmap size and file size does not match");
		return false;
	}

	struct BmpDibHeaderV3 { //40 bytes
		udword	headerSize;
		dword	width;
		dword	height;
		uword	planes;
		uword	bpp;
		udword	compression;
		udword	bmpSize;
		dword	hres;
		dword	vres;
		udword	nColors;
		udword	nImpColors;
	} *bmpDibHeader = reinterpret_cast<BmpDibHeaderV3*>(rawData + sizeof(BmpHeader) + 2);

	//Some controls
	bool unsupported = false;
	//if(bmpDibHeader->headerSize != 40)	unsupported = true; //V3 header only
	if(bmpDibHeader->height <= 0)		unsupported = true;
	if(bmpDibHeader->width <= 0)		unsupported = true;
	if(bmpDibHeader->planes != 1)       unsupported = true;
	if(bmpDibHeader->bpp != 24)			unsupported = true; //Only 24 bpp
	if(bmpDibHeader->compression != 0)	unsupported = true;

	if(unsupported) {
		log->printf("Unsupported bitmap");
		return false;
	}

	width	= bmpDibHeader->width;
	height	= bmpDibHeader->height;

	if(max<int>(width, height) > 4096) {
		log->printf("Bitmap too big");
		return false;
	}

	data	= new byte[width * height * 3];
	format	= IMAGE_RGB;

	//Each row is extended to a 32-bit (4-byte) boundary
	unsigned rowBytes = bmpDibHeader->bmpSize / bmpDibHeader->height;
	unsigned padding = rowBytes - width * 3;

	//NOTE:
	//Pixels are stored starting in the lower left corner, going from left to
	//right, and then row by row from the bottom to the top of the image

	//Inverts the image
	byte *pImage = data;
	for(int i = height - 1; i > 0; i--) {
		byte *pBitmap = rawData + bmpHeader->offset + rowBytes * i;
		for(int j = 0; j < width; j++) {
			//BGR to RGB
			*pImage++ = pBitmap[2];
			*pImage++ = pBitmap[1];
			*pImage++ = pBitmap[0];

			pBitmap += 3;
		}
	}

	return true;
}

//=============================================================================
// Loads a jpeg image
//=============================================================================
bool Image::loadJpeg(byte *rawData, unsigned size)
{
	jpeg_error_mgr	error;
	jpeg_decompress_struct	decompress;
	JSAMPARRAY		rowPointers = nullptr;

	decompress.err = jpeg_std_error(&error);

	//Override default error functions
	error.error_exit = jpeg_error_exit_override;
	error.output_message = jpeg_output_message_override;

	//In case of error libjpeg jumps here
	if(setjmp(jpeg_error_jmp)) {
		delete [] rowPointers;
		delete [] data;
		data = nullptr;
		format = IMAGE_EMPTY;
		return false;
	}

	//Init libjpeg
	jpeg_create_decompress(&decompress);

	//Set data source to memory
	jpeg_mem_src(&decompress, rawData, size);

	//Read jpeg header
	jpeg_read_header(&decompress, TRUE);

	//Init decompression
	if(!jpeg_start_decompress(&decompress)) {
		jpeg_destroy_decompress(&decompress);
		return false;
	}

	//Some controls
	bool unsupported = false;
	if(decompress.output_components != 3)		unsupported = false;
	if(decompress.out_color_components != 3)	unsupported = false;
	if(unsupported) {
		jpeg_finish_decompress(&decompress);
		jpeg_destroy_decompress(&decompress);
		log->printf("Unsupported JPEG");
		return false;
	}

	//Get image width and height
	width	= decompress.output_width;
	height	= decompress.output_height;
	
	//Lenght of a row in bytes
	unsigned rowBytes = width * 3;

	data	= new byte[rowBytes * height];
	format	= IMAGE_RGB;
	
	//Create a pointer for each row
	rowPointers = new JSAMPROW[height];
	JSAMPROW p = data;// + rowBytes * (height - 1);
	for(int i = 0; i < height; i++) {
		rowPointers[i] = p;
		p += rowBytes;
	}
	
	//while(decompress.output_scanline < decompress.output_height) {
	jpeg_read_scanlines(&decompress, rowPointers + decompress.output_scanline, height);
	//}

	jpeg_finish_decompress(&decompress);
	jpeg_destroy_decompress(&decompress);

	delete [] rowPointers;
	return true;
}

//=============================================================================
// Loads a png
//=============================================================================
bool Image::loadPng(byte *rawData, unsigned size)
{
	png_structp	png;
	png_infop	info;
	png_bytepp	rowPointers = 0;

	//Init the png struct
	png = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, png_error_override, png_warning_override);
	if(!png) return false;

	//Init the info struct
	info = png_create_info_struct(png);
	if(!info) {
		png_destroy_read_struct(&png, 0, 0);
		return false;
	}

	//In case of error libpng jumps here
	if(setjmp(png_jmpbuf(png)))  {
		png_destroy_read_struct(&png, &info, 0);
		delete [] rowPointers;
		delete [] data;
		data = nullptr;
		format = IMAGE_EMPTY;
		return false;
	}

	png_read_count = 0;
	png_set_read_fn(png, static_cast<void*>(rawData), png_read_data_override);

	png_read_info(png, info);

	//Try to always get a RGB or RGBA color type
	switch(png_get_color_type(png, info)) {
	case PNG_COLOR_TYPE_GRAY:
	case PNG_COLOR_TYPE_GRAY_ALPHA:
		png_set_gray_to_rgb(png);
		break;

	case PNG_COLOR_TYPE_PALETTE:
		png_set_palette_to_rgb(png);
		break;
    }

	//Strip 16 bits precision to 8 bits
	if(16 == png_get_bit_depth(png, info)) png_set_strip_16(png);

	//If the image has a trasparency set convert it to an alpha channel
	if(png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);

	//Update info
	png_read_update_info(png, info);

	switch(png_get_color_type(png, info)) {
	case PNG_COLOR_TYPE_RGB:
		format = IMAGE_RGB;
		break;
	case PNG_COLOR_TYPE_RGB_ALPHA:
		format = IMAGE_RGBA;
		break;
	default:
		log->printf("Unsupported color type");
		png_destroy_read_struct(&png, &info, 0);
		return false;
	}

	//Get image width and height
	width	= png_get_image_width(png, info);
	height	= png_get_image_height (png, info);

	//Lenght of a row in bytes
	unsigned rowBytes = png_get_rowbytes(png, info);

	data = new byte[rowBytes * height];

	//Create a pointer for each row
	rowPointers = new png_bytep[height];
	png_bytep p = data;// + rowBytes * (height - 1);
	for(int i = 0; i < height; i++) {
		rowPointers[i] = p;
		p += rowBytes;
	}

	//Read the image
    png_read_image(png, rowPointers);

	png_destroy_read_struct(&png, &info, 0);

    delete [] rowPointers;
	return true;
}
