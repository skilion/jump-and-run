#pragma once
#ifndef __WORLD_NODE_H__
#define __WORLD_NODE_H__


#include <Math/Aabox.h>
#include <Math/Math.h>
#include <Math/ViewingFrustum.h>

#include "Render/Color.h"


namespace myengine {
	
	class WorldNode
	{
	public:
			WorldNode(unsigned size);
			~WorldNode();

		bool	collide(const Aabox &aabox) const;

		void	draw() const;
		void	draw(ViewingFrustum &frustum) const;
		void	drawGrid() const;

		Color	*get(unsigned x, unsigned y, unsigned z) const;
		void	insert(unsigned x, unsigned y, unsigned z, Color *element);
		void	remove(unsigned x, unsigned y, unsigned z);

		void	free();

		void	readFromFile(File *file);
		void	writeToFile(File *file) const;


	private:
			WorldNode();
			WorldNode(unsigned size, unsigned cx, unsigned cy, unsigned cz);

		unsigned	getIndex(unsigned x, unsigned y, unsigned z) const;
		void		initialize();

		unsigned	size;
		
		union {
			WorldNode	*nodes[8];
			Color		*leaves[8];	//size == 1
		};

		unsigned	cx, cy, cz; //Center of the node
		Aabox		aabox[8];
	};

}


#endif //!__WORLD_NODE_H__
