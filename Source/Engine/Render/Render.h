#pragma once
#ifndef __RENDER_H__
#define __RENDER_H__


#include "Math/Vector.h"


namespace myengine {
	
	class Camera;
	class Color;
	
	class Render
	{
	public:
			//Render();
			virtual ~Render();

		static Render		*create();

		virtual bool		initialize(bool createWindow = true) = 0;
		virtual void		shutdown() = 0;

		virtual void		frameBegin() = 0;
		virtual void		frameEnd() = 0;
		
		//virtual void		getScreenshot(Image &screenshot);
		
		const Vec2i			&getResolution();
		virtual int			getColors() = 0;
		virtual bool		isFullscreen() = 0;

		virtual void		setResolution(const Vec2i &resolution);

		virtual void		setBgColor(const Color &color) = 0;
		virtual void		setDrawColor(const Color &color) = 0;

		// ==== 2D graphics ==== //
		virtual void		setup2D(const Vec2i &screen) = 0;
		virtual void		translate(const Vec2i &translation) = 0;

		virtual void		drawPoint(const Vec2i &position) = 0;
		virtual void		drawLine(const Vec2i &start, const Vec2i &end) = 0;
		virtual void		drawFilledRect(const Vec2i &position, const Vec2i &size) = 0;
		virtual void		drawOutlinedRect(const Vec2i &position, const Vec2i &size) = 0;

		virtual void		startLines() = 0;
		virtual void		startPolygon() = 0;
		virtual void		vertex(const Vec2i &position) = 0;
		virtual void		vertex(const Vec2f &position) = 0;
		virtual void		endLines() = 0;
		virtual void		endPolygon() = 0;

		virtual void		setClipZone(const Vec2i &position, const Vec2i &size) = 0;
		virtual void		disableClipZone() = 0;

		// ==== 3D graphics ==== //
		virtual void		setupCamera(const Camera &camera) = 0;

		virtual void		rotate(float x, float y, float z) = 0;
		virtual void		translate(float x, float y, float z) = 0;
		virtual void		drawAxis(float x, float y, float z) = 0;
		virtual void		drawLine(const Vec3f &start, const Vec3f &end) = 0;
		virtual void		drawFilledRect(float x, float y, float z, float w, float h) = 0;
		virtual void		drawBox(const Vec3f &position, float size) = 0;
		virtual void		drawBox(const Vec3f &min, const Vec3f &max) = 0;
		virtual void		drawOutlinedBox(const Vec3f &position, float size) = 0;
		virtual void		drawOutlinedBox(const Vec3f &min, const Vec3f &max) = 0;


	private:
		Vec2i	resolution;
	};

	extern Render	*render;

}


#endif //!__RENDER_H__
