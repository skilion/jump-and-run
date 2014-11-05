#pragma once
#ifndef __GL_RENDER_H__
#define __GL_RENDER_H__


#include "Render.h"


namespace myengine {
	
	class OpenGLRender: public Render
	{
	public:
			OpenGLRender();
			//virtual ~OpenGLRender();

		virtual bool		initialize(bool createWindow);
		virtual void		shutdown();

		virtual bool		createWindow() = 0;
		virtual void		destroyWindow() = 0;

		virtual void		frameBegin();
		virtual void		frameEnd();
		
		virtual int			getColors();
		virtual bool		isFullscreen();

		virtual void		setResolution(const Vec2i &resolution);
				
		virtual void		setBgColor(const Color &color);
		virtual void		setDrawColor(const Color &color);

		// ==== 2D graphics ==== //
		virtual void		setup2D(const Vec2i &screen);
		virtual void		translate(const Vec2i &translation);

		virtual void		drawPoint(const Vec2i &position);
		virtual void		drawLine(const Vec2i &start, const Vec2i &end);
		virtual void		drawFilledRect(const Vec2i &position, const Vec2i &size);
		virtual void		drawOutlinedRect(const Vec2i &position, const Vec2i &size);

		virtual void		startLines();
		virtual void		startPolygon();
		virtual void		vertex(const Vec2i &position);
		virtual void		vertex(const Vec2f &position);
		virtual void		endLines();
		virtual void		endPolygon();

		virtual void		setClipZone(const Vec2i &position, const Vec2i &size);
		virtual void		disableClipZone();

		// ==== 3D graphics ==== //
		virtual void		setupCamera(const Camera &camera);

		virtual void		rotate(float x, float y, float z);
		virtual void		translate(float x, float y, float z);
		virtual void		drawAxis(float x, float y, float z);
		virtual void		drawLine(const Vec3f &start, const Vec3f &end);
		virtual void		drawFilledRect(float x, float y, float z, float w, float h);
		virtual void		drawBox(const Vec3f &position, float size);
		virtual void		drawBox(const Vec3f &min, const Vec3f &max);
		virtual void		drawOutlinedBox(const Vec3f &position, float size);
		virtual void		drawOutlinedBox(const Vec3f &min, const Vec3f &max);


	protected:
		virtual bool	isExtAvailable(const char *extension);
		
		int			colors, depth;
		int			multisamples;
		bool		fullscreen, vsync;


	private:
		void		setupOpenGL();
		
		bool		started;
		float		aspect; //screenWidth / screenHeight
		
		const char	*glExtensions;
		unsigned	clipZoneCount;
	};

}


#endif //!__GL_RENDER_H__