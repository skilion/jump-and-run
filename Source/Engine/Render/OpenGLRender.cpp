#include "OpenGLHeader.h" //

#include "Log.h"
#include "Color.h"
#include "Image.h"
#include "Camera.h"
#include "Memory.h"
#include "System.h"
#include "myString.h"
#include "OpenGLRender.h"
#include "OpenGLTexture.h"

#include "Config/Config.h"
#include "File/File.h"
#include "Math/Math.h"


#include "memdbg.h"

using namespace myengine;


//OpenGLConfig myengine::glConfig;


//=============================================================================
// Constructor
//=============================================================================
OpenGLRender::OpenGLRender()
{
	started = false;
	colors = depth = 0;
	multisamples = 0;
	fullscreen = vsync = false;
	clipZoneCount = 0;
	glExtensions = nullptr;
}

//=============================================================================
// Destructor
//=============================================================================
//OpenGLRender::~OpenGLRender() {}

//=============================================================================
// Initialize
//=============================================================================
bool OpenGLRender::initialize(bool createWindow)
{
	//if(!Render::initialize()) return false;
	if(started) return true;

	int desktopWidth, desktopHeight, desktopColors;
	system->getDesktopInfo(desktopWidth, desktopHeight, desktopColors);
	
	//Screen resolution
	if(config.get("ScreenWidth").isEmpty() | config.get("ScreenHeight").isEmpty()) {
		config.get("ScreenWidth")	= desktopWidth;
		config.get("ScreenHeight")	= desktopHeight;
		setResolution(Vec2i(desktopWidth, desktopHeight));
	} else {
		setResolution(Vec2i(config.get("ScreenWidth").toInt(), config.get("ScreenHeight").toInt()));
	}

	//Screen colors
	if(config.get("ScreenColors").isEmpty()) {
		colors = desktopColors;
		config.get("ScreenColors")	= desktopColors;
	} else {
		colors = config.get("ScreenColors").toInt();
	}

	//Screen depth
	if(config.get("ScreenDepth").isEmpty()) {
		depth = (desktopColors >= 32) ? 24 : 16;
		config.get("ScreenDepth") = depth;
	} else {
		depth = config.get("ScreenDepth").toInt();
	}

	//Fullscreen
	if(config.get("Fullscreen").isEmpty()) {
		fullscreen = true;
		config.get("Fullscreen") = "1";
	} else {
		fullscreen = config.get("Fullscreen").toInt() != 0;
	}

	//Vertical sync
	if(config.get("Vsync").isEmpty()) {
		vsync = true;
		config.get("Vsync") = "1";
	} else {
		vsync = config.get("Vsync").toInt() != 0;
	}

	//Multisampling
	if(config.get("Multisamples").isEmpty()) {
		multisamples = 0;
		config.get("Multisamples") = "0";
	} else {
		multisamples = config.get("Multisamples").toInt();
	}

	log->printf("Initializing OpenGL...");

	if(createWindow) {
		if(!this->createWindow()) {
			return false;
		}
	}

	glExtensions = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
	
	if(!isExtAvailable("GL_ARB_texture_non_power_of_two")) {
		log->print("GL_ARB_texture_non_power_of_two is required !");
	}

	/*if(!isExtAvailable("GL_ARB_texture_rectangle")) {
		log->print("GL_ARB_texture_rectangle is required");
		return false;
	}*/

	//Get OpenGL features
	/*glConfig.vendor		= reinterpret_cast<const char*>(glGetString(GL_VENDOR));
	glConfig.renderer	= reinterpret_cast<const char*>(glGetString(GL_RENDERER));
	glConfig.version	= reinterpret_cast<const char*>(glGetString(GL_VERSION));
	glConfig.extensions	= reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
	glConfig.glVersion	= static_cast<float>(atof(glConfig.version));

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glConfig.maxTextureSize);
	//glGetIntegerv(GL_MAX_LIGHTS, &info.max_lights);

	//OpenGL supports non power-of-two texture since version 2.0
	if(glConfig.glVersion < 2.0f) {
		glConfig.textureCompatibility = true;
	} else {
		glConfig.textureCompatibility = false;
	}

	log->printf(" vendor: %s", glConfig.vendor);
	log->printf(" renderer: %s", glConfig.renderer);
	log->printf(" version: %s", glConfig.version);
	log->printf("max lights: %i", glConfig.max_lights);
	log->printf(" max texture size: %i", glConfig.maxTextureSize);
	log->printf(" ext: %s\n",glConfig.extensions);*/

	log->printf(" OpenGL vendor: %s", glGetString(GL_VENDOR));
	log->printf(" OpenGL renderer: %s", glGetString(GL_RENDERER));
	log->printf(" OpenGL version: %s", glGetString(GL_VERSION));

	setupOpenGL();

	started = true;
	return true;
}

//=============================================================================
// Shutdown
//=============================================================================
void OpenGLRender::shutdown()
{
	if(!started) return;
	log->printf("Shutting down OpenGL...");

	destroyWindow();
	
	started = false;
	colors = depth = 0;
	multisamples = 0;
	fullscreen = vsync = false;
	clipZoneCount = 0;
	glExtensions = nullptr;
}

//=============================================================================
// Clears the screen
//=============================================================================
void OpenGLRender::frameBegin()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//=============================================================================
// Called when the frame ends
//=============================================================================
void OpenGLRender::frameEnd()
{
	unsigned errorCode = glGetError();
	if(errorCode != GL_NO_ERROR)
	{
		const char *errorString;
		switch(errorCode)
		{
		case GL_INVALID_ENUM:
			errorString = "invalid enum";
			break;
		case GL_INVALID_VALUE:
			errorString = "invalid value";
			break;
		case GL_INVALID_OPERATION:
			errorString = "invalid operation";
			break;
		case GL_STACK_OVERFLOW:
			errorString = "stack overflow";
			break;
		case GL_STACK_UNDERFLOW:
			errorString = "stack underflow";
			break;
		case GL_OUT_OF_MEMORY:
			errorString = "out of memory";
			break;
		/*case GL_TABLE_TOO_LARGE:
			strError = "GL_TABLE_TOO_LARGE";
			break;*/
		default:
			errorString = "unknow";
		}

		log->printf("OpenGL error: %s", errorString);
	}
}

//=============================================================================
// Returns some info about the render
//=============================================================================
int OpenGLRender::getColors()
{
	return colors;
}

//=============================================================================
// Returns some info about the render
//=============================================================================
bool OpenGLRender::isFullscreen()
{
	return fullscreen;
}

//=============================================================================
// Sets the resolution
//=============================================================================
void OpenGLRender::setResolution(const Vec2i &resolution)
{
	Render::setResolution(resolution);
	aspect = static_cast<float>(getResolution().getX()) / static_cast<float>(getResolution().getY());
}

//=============================================================================
// Sets the background color
//=============================================================================
void OpenGLRender::setBgColor(const Color &color)
{
	glClearColor(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
}

//=============================================================================
// Sets the drawing color
//=============================================================================
void OpenGLRender::setDrawColor(const Color &color)
{
	glColor4fv(color);
}

//=============================================================================
// Sets up for 2D drawing
//=============================================================================
void OpenGLRender::setup2D(const Vec2i &screen)
{
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(0, getResolution().getX(), getResolution().getY(), 0, -1, 1);
	glOrtho(0, screen.getX(), screen.getY(), 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//=============================================================================
// Translates the origin
//=============================================================================
void OpenGLRender::translate(const Vec2i &translation)
{
	glTranslatef(static_cast<float>(translation.getX()), static_cast<float>(translation.getY()), 0);
}

//=============================================================================
// Draws a point
//=============================================================================
void OpenGLRender::drawPoint(const Vec2i &position)
{
	glBegin(GL_POINTS);
	glVertex2i(position.getX(), position.getY());
	glEnd();
}

//=============================================================================
// Draws a line
//=============================================================================
void OpenGLRender::drawLine(const Vec2i &start, const Vec2i &end)
{
	glBegin(GL_LINES);
	glVertex2f(start.getX() + 0.5f, start.getY() + 0.5f);
	glVertex2i((end.getX() > start.getX()) ? (end.getX() + 1) : end.getX(),
		(end.getY() > start.getY()) ? (end.getY() + 1) : end.getY());
	/*glVertex2f(start.getX() + 5, start.getY() + 5);
	glVertex2f(end.getX() + 5, end.getY() + 5);*/
	glEnd();
}

//=============================================================================
// Draws a filled rect
//=============================================================================
void OpenGLRender::drawFilledRect(const Vec2i &position, const Vec2i &size)
{
	glBegin(GL_QUADS);
	glVertex2i(position.getX(), position.getY());
	glVertex2i(position.getX(), position.getY() + size.getY());
	glVertex2i(position.getX() + size.getX(), position.getY() + size.getY());
	glVertex2i(position.getX() + size.getX(), position.getY());
	glEnd();
}

//=============================================================================
// Draws an outlined rectangle
//=============================================================================
void OpenGLRender::drawOutlinedRect(const Vec2i &position, const Vec2i &size)
{
	glBegin(GL_LINE_LOOP);
	glVertex2i(position.getX() + 1, position.getY());
	glVertex2i(position.getX() + 1, position.getY() + size.getY());
	glVertex2i(position.getX() + size.getX(), position.getY() + size.getY() - 1);
	glVertex2i(position.getX() + size.getX(), position.getY());
	glEnd();
}


void OpenGLRender::startLines()
{
	glBegin(GL_LINE_STRIP);
}

void OpenGLRender::startPolygon()
{
	glBegin(GL_POLYGON);
}

void OpenGLRender::vertex(const Vec2i &position)
{
	glVertex2i(position.getX(), position.getY());
}

void OpenGLRender::vertex(const Vec2f &position)
{
	glVertex2f(position.getX(), position.getY());
}

void OpenGLRender::endLines()
{
	glEnd();
}


void OpenGLRender::endPolygon()
{
	glEnd();
}

//=============================================================================
// Sets a clipping zone
//=============================================================================
void OpenGLRender::setClipZone(const Vec2i &position, const Vec2i &size)
{
	glEnable(GL_STENCIL_TEST);

	if(clipZoneCount == 0) {			//Ensure stencil buffer is clear
		//glClearStencil(0);
		glClear(GL_STENCIL_BUFFER_BIT);
	}

	glColorMask(0, 0, 0, 0);
	glStencilFunc(GL_EQUAL, clipZoneCount, 0xFF);
	glStencilOp(GL_KEEP, GL_INCR, GL_INCR);

	drawFilledRect(position, size);
	clipZoneCount++;

	glColorMask(1, 1, 1, 1);
	glStencilFunc(GL_EQUAL, clipZoneCount, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

//=============================================================================
// Disables the clipping zone
//=============================================================================
void OpenGLRender::disableClipZone()
{
	if(clipZoneCount > 1) {
		glColorMask(0, 0, 0, 0);
		//glStencilFunc(GL_EQUAL, clipZoneCount, 0xFF);
		glStencilOp(GL_KEEP, GL_DECR, GL_DECR);

		drawFilledRect(Vec2i(), getResolution());
		clipZoneCount--;

		glColorMask(1, 1, 1, 1);
		glStencilFunc(GL_EQUAL, clipZoneCount, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	} else {
		clipZoneCount = 0;
		glDisable(GL_STENCIL_TEST);
	}
}

//=============================================================================
// Setup the camera view
//=============================================================================
void OpenGLRender::setupCamera(const Camera &camera)
{
	const Vec3f &position = camera.getPosition();
	const Angles &angles = camera.getAngles();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//gluPerspective(90, 640.0 / 480.0, 1, 100);
	float height = tan(camera.getFov() * DEG2RAD / 2) * 1e-3f;
	float width = height * aspect;
	glFrustum(-width, width, -height, height, 1e-3, 1e3);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(camera.computeRotationMatrix());
	glTranslatef(-position.getX(), -position.getY(), -position.getZ());

	glEnable(GL_DEPTH_TEST);
}


void OpenGLRender::rotate(float x, float y, float z)
{
	glRotatef(x, 1, 0, 0);
	glRotatef(y, 0, 1, 0);
	glRotatef(z, 0, 0, 1);
}


void OpenGLRender::translate(float x, float y, float z)
{
	glTranslatef(x, y, z);
}


void OpenGLRender::drawAxis(float x, float y, float z)
{
	float c[3] = {x, y, z};
	glBegin(GL_LINES);
	//X
	glColor3f(1, 0, 0);
	glVertex3fv(c);
	glVertex3f(x + 1, y, z);
	glVertex3f(x + 1, y, z);
	glVertex3f(x + 0.9f, y + 0.04f, z);
	glVertex3f(x + 1, y, z);
	glVertex3f(x + 0.9f, y - 0.04f, z);
	//Y
	glColor3f(0, 1, 0);
	glVertex3fv(c);
	glVertex3f(x, y + 1, z);
	glVertex3f(x, y + 1, z);
	glVertex3f(x - 0.04f, y + 0.9f, z);
	glVertex3f(x, y + 1, z);
	glVertex3f(x + 0.04f, y + 0.9f, z);
	//Z
	glColor3f(0, 0, 1);
	glVertex3fv(c);
	glVertex3f(x, y, z + 1);
	glVertex3f(x, y, z + 1);
	glVertex3f(x, y + 0.04f, z + 0.9f);
	glVertex3f(x, y, z + 1);
	glVertex3f(x, y - 0.04f, z + 0.9f);
	glEnd();
}


void OpenGLRender::drawLine(const Vec3f &start, const Vec3f &end)
{
	glBegin(GL_LINES);
	glVertex3fv(start);
	glVertex3fv(end);
	glEnd();
}

void OpenGLRender::drawFilledRect(float x, float y, float z, float w, float h)
{
	glBegin(GL_QUADS);
	glVertex3f(x, y, z);
	glVertex3f(x + w, y, z);
	glVertex3f(x + w, y, z + h);
	glVertex3f(x, y, z + h);
	glEnd();
}

void OpenGLRender::drawBox(const Vec3f &position, float size)
{
	float x1 = position.getX();
	float y1 = position.getY();
	float z1 = position.getZ();

	float x2 = x1 + size;
	float y2 = y1 + size;
	float z2 = z1 + size;

	glBegin(GL_QUADS);

	//FRONT
	glVertex3f(x1, y1, z1);
	glVertex3f(x2, y1, z1);
	glVertex3f(x2, y2, z1);
	glVertex3f(x1, y2, z1);

	//BEHIND
	glVertex3f(x1, y1, z2);
	glVertex3f(x1, y2, z2);
	glVertex3f(x2, y2, z2);
	glVertex3f(x2, y1, z2);

	//TOP
	glVertex3f(x1, y2, z1);
	glVertex3f(x2, y2, z1);
	glVertex3f(x2, y2, z2);
	glVertex3f(x1, y2, z2);

	//BOTTOM
	glVertex3f(x1, y1, z1);
	glVertex3f(x1, y1, z2);
	glVertex3f(x2, y1, z2);
	glVertex3f(x2, y1, z1);

	//LEFT
	glVertex3f(x1, y1, z1);
	glVertex3f(x1, y2, z1);
	glVertex3f(x1, y2, z2);
	glVertex3f(x1, y1, z2);

	//RIGHT
	glVertex3f(x2, y1, z1);
	glVertex3f(x2, y1, z2);
	glVertex3f(x2, y2, z2);
	glVertex3f(x2, y2, z1);

	glEnd();
}


void OpenGLRender::drawBox(const Vec3f &min, const Vec3f &max)
{
	float x1 = min.getX();
	float y1 = min.getY();
	float z1 = min.getZ();

	float x2 = max.getX();
	float y2 = max.getY();
	float z2 = max.getZ();

	glBegin(GL_QUADS);

	//FRONT
	glVertex3f(x1, y1, z1);
	glVertex3f(x2, y1, z1);
	glVertex3f(x2, y2, z1);
	glVertex3f(x1, y2, z1);

	//BEHIND
	glVertex3f(x1, y1, z2);
	glVertex3f(x1, y2, z2);
	glVertex3f(x2, y2, z2);
	glVertex3f(x2, y1, z2);

	//TOP
	glVertex3f(x1, y2, z1);
	glVertex3f(x2, y2, z1);
	glVertex3f(x2, y2, z2);
	glVertex3f(x1, y2, z2);

	//BOTTOM
	glVertex3f(x1, y1, z1);
	glVertex3f(x1, y1, z2);
	glVertex3f(x2, y1, z2);
	glVertex3f(x2, y1, z1);

	//LEFT
	glVertex3f(x1, y1, z1);
	glVertex3f(x1, y2, z1);
	glVertex3f(x1, y2, z2);
	glVertex3f(x1, y1, z2);

	//RIGHT
	glVertex3f(x2, y1, z1);
	glVertex3f(x2, y1, z2);
	glVertex3f(x2, y2, z2);
	glVertex3f(x2, y2, z1);

	glEnd();
}

void OpenGLRender::drawOutlinedBox(const Vec3f &position, float size)
{
	float x1 = position.getX();
	float y1 = position.getY();
	float z1 = position.getZ();

	float x2 = x1 + size;
	float y2 = y1 + size;
	float z2 = z1 + size;

	glBegin(GL_LINE_LOOP);
	glVertex3f(x1, y1, z1);
	glVertex3f(x2, y1, z1);
	glVertex3f(x2, y2, z1);
	glVertex3f(x1, y2, z1);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(x1, y1, z2);
	glVertex3f(x2, y1, z2);
	glVertex3f(x2, y2, z2);
	glVertex3f(x1, y2, z2);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(x1, y1, z1);
	glVertex3f(x1, y1, z2);
	glVertex3f(x2, y1, z1);
	glVertex3f(x2, y1, z2);
	glVertex3f(x2, y2, z1);
	glVertex3f(x2, y2, z2);
	glVertex3f(x1, y2, z1);
	glVertex3f(x1, y2, z2);
	glEnd();
}

void OpenGLRender::drawOutlinedBox(const Vec3f &min, const Vec3f &max)
{
	float x1 = min.getX();
	float y1 = min.getY();
	float z1 = min.getZ();

	float x2 = max.getX();
	float y2 = max.getY();
	float z2 = max.getZ();

	glBegin(GL_LINE_LOOP);
	glVertex3f(x1, y1, z1);
	glVertex3f(x2, y1, z1);
	glVertex3f(x2, y2, z1);
	glVertex3f(x1, y2, z1);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(x1, y1, z2);
	glVertex3f(x2, y1, z2);
	glVertex3f(x2, y2, z2);
	glVertex3f(x1, y2, z2);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(x1, y1, z1);
	glVertex3f(x1, y1, z2);
	glVertex3f(x2, y1, z1);
	glVertex3f(x2, y1, z2);
	glVertex3f(x2, y2, z1);
	glVertex3f(x2, y2, z2);
	glVertex3f(x1, y2, z1);
	glVertex3f(x1, y2, z2);
	glEnd();
}

//=============================================================================
// Returns true if a given extension is available
//=============================================================================
bool OpenGLRender::isExtAvailable(const char *extension)
{
	if(glExtensions && String::strstr(glExtensions, extension)) {
		return true;
	}

	return false;
}

//=============================================================================
// Initializes OpenGL
//=============================================================================
void OpenGLRender::setupOpenGL()
{
	/*if(fullscreen) {
		int w, h, c;
		system->getDesktopInfo(w, h, c);
		glViewport(0, 0, w, h);//resolution.getX(), resolution.getY());
	} else {*/
	glViewport(0, 0, getResolution().getX(), getResolution().getY());

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/*glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);*/

	if(multisamples) glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_SMOOTH);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}
