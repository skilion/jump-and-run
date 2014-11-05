#pragma once
#ifndef __GL_HEADER_H__
#define __GL_HEADER_H__


#if defined(WIN32)
	#include "System/WindowsHeader.h"
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glext.h>
	#include <GL/wglext.h>
#elif defined(LINUX)
	#include <GL/glx.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glext.h>
#endif


#endif //!__GL_HEADER_H__