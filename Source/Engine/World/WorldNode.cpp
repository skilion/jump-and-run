#include "Platform.h"
#include "WorldNode.h"

#include "Render/Render.h"

using namespace myengine;


//=============================================================================
// Constructor
//=============================================================================
WorldNode::WorldNode()
{
	for(int i = 0; i < 8; i++) nodes[i] = nullptr;
}

WorldNode::WorldNode(unsigned size)
{
	this->size		= size;
	cx = cy = cz	= size;

	initialize();
}

WorldNode::WorldNode(unsigned size, unsigned cx, unsigned cy, unsigned cz)
{
	this->size	= size;
	this->cx	= cx;
	this->cy	= cy;
	this->cz	= cz;

	initialize();
}

//=============================================================================
// Destructor
//=============================================================================
WorldNode::~WorldNode()
{
	free();
}

//=============================================================================
// Returns true if the specified Aabox collides with the world
//=============================================================================
bool WorldNode::collide(const Aabox &aabox) const
{
	for(unsigned i = 0; i < 8; i++) {
		if(aabox.intersects(aabox)) {
			if(size == 1) {
				return leaves[i] != nullptr;
			} else if(nodes[i] && nodes[i]->collide(aabox)) {
				return true;
			}
		}
	}

	return false;
}

//=============================================================================
// Draw all the voxels of the node
//=============================================================================
void WorldNode::draw() const
{
	if(size == 1) {
		for(unsigned i = 0; i < 8; i++) {
			if(leaves[i]) {
				render->setDrawColor(*leaves[i]);
				render->drawBox(aabox[i].getMin(), aabox[i].getMax());
			}
		}
	} else {
		for(unsigned i = 0; i < 8; i++) {
			if(nodes[i]) nodes[i]->draw();
		}
	}
}

//=============================================================================
// Draw only the visible voxels 
//=============================================================================
void WorldNode::draw(ViewingFrustum &frustum) const
{
	if(size > 4) {
		for(unsigned i = 0; i < 8; i++) {
			WorldNode *node = nodes[i];
			if(node) {
				switch(frustum.test(aabox[i]))
				{
				case ViewingFrustum::INSIDE:
					node->draw();
					break;
				
				case ViewingFrustum::INTERSECT:
					node->draw(frustum);
					break;
				}
			}
		}
	} else {
		draw();
	}
}

//=============================================================================
// Recursively draw the borders of every node
//=============================================================================
const Vec3f DIFF(0.001f, 0.001f, 0.001f);
void WorldNode::drawGrid() const
{
	render->drawOutlinedBox(aabox[0].getMin() - DIFF, aabox[7].getMax() + DIFF);
	if(size > 1) {
		for(unsigned i = 0; i < 8; i++) {
			if(nodes[i]) nodes[i]->drawGrid();
		}
	}
}

//=============================================================================
// Returns the element at the specified position
//=============================================================================
Color *WorldNode::get(unsigned x, unsigned y, unsigned z) const
{
	if(size == 1) {
		return leaves[(x & 1) + ((y & 1) << 1) + ((z & 1) << 2)];
	} else {
		unsigned index = getIndex(x, y, z);
		WorldNode *node = nodes[index];
		if(node) return node->get(x, y, z);
	}

	return nullptr;
}
	
//=============================================================================
// Inserts an element at the specified position
//=============================================================================
void WorldNode::insert(unsigned x, unsigned y, unsigned z, Color *element)
{
	ASSERT(element);

	if(size == 1) {
		unsigned index = (x & 1) + ((y & 1) << 1) + ((z & 1) << 2);
		Color *leaf = leaves[index];
		if(leaf) delete leaf;
		leaves[index] = element;
	} else {
		unsigned index = getIndex(x, y, z);
		WorldNode *node = nodes[index];

		if(!node) {
			unsigned nsize = size >> 1;
			unsigned nx = cx, ny = cy, nz = cz;
			switch(index)
			{
			case 0:
				nx -= nsize;
				ny -= nsize;
				nz -= nsize;
				break;
			case 1:
				nx += nsize;
				ny -= nsize;
				nz -= nsize;
				break;
			case 2:
				nx -= nsize;
				ny += nsize;
				nz -= nsize;
				break;
			case 3:
				nx += nsize;
				ny += nsize;
				nz -= nsize;
				break;
			case 4:
				nx -= nsize;
				ny -= nsize;
				nz += nsize;
				break;
			case 5:
				nx += nsize;
				ny -= nsize;
				nz += nsize;
				break;
			case 6:
				nx -= nsize;
				ny += nsize;
				nz += nsize;
				break;
			//case 7:
			default:
				nx += nsize;
				ny += nsize;
				nz += nsize;
				break;
			}

			node = nodes[index] = new WorldNode(nsize, nx, ny, nz);
		}
		node->insert(x, y, z, element);
	}
}
	
//=============================================================================
// Removes an element at the specified position
//=============================================================================
void WorldNode::remove(unsigned x, unsigned y, unsigned z)
{
	if(size == 1) {
		unsigned index = (x & 1) + ((y & 1) << 1) + ((z & 1) << 2);
		delete leaves[index];
		leaves[index] = nullptr;
	} else {
		unsigned index = getIndex(x, y, z);
		delete nodes[index];
		nodes[index] = nullptr;
	}
}
	
//=============================================================================
// Frees the memory
//=============================================================================
void WorldNode::free()
{
	if(size == 1) {
		for(int i = 0; i < 8; i++) {
			delete nodes[i];
			nodes[i] = nullptr;
		}
	} else {
		for(int i = 0; i < 8; i++) {
			delete leaves[i];
			leaves[i] = nullptr;
		}
	}
}


//=========================================================================
// Save the world in the selected file
//=========================================================================
void WorldNode::readFromFile(File *file)
{
	free();
	
	file->readUnsignedInt(size);
	file->readUnsignedInt(cx);
	file->readUnsignedInt(cy);
	file->readUnsignedInt(cz);

	initialize();

	char tmp;
	if(size > 1) {
		for(unsigned i = 0; i < 8; i++) {
			file->readChar(tmp);
			if(tmp) {
				nodes[i] = new WorldNode;
				nodes[i]->readFromFile(file);
			} else nodes[i] = 0;
		}
	} else {
		for(unsigned i = 0; i < 8; i++) {
			file->readChar(tmp);
			if(tmp) {
				leaves[i] = new Color;
				leaves[i]->readFromFile(file);
			} else leaves[i] = 0;
		}
	}
}

//=========================================================================
// Save the world in the selected file
//=========================================================================
void WorldNode::writeToFile(File *file) const
{
	file->writeUnsignedInt(size);
	file->writeUnsignedInt(cx);
	file->writeUnsignedInt(cy);
	file->writeUnsignedInt(cz);
	
	if(size > 1) {
		for(unsigned i = 0; i < 8; i++) {
			if(nodes[i]) {
				file->writeChar(1);
				nodes[i]->writeToFile(file);
			} else file->writeChar(0);
		}
	} else {
		for(unsigned i = 0; i < 8; i++) {
			file->writeChar(1);
			if(leaves[i]) {
				file->writeChar(1);
				leaves[i]->writeToFile(file);
			} else file->writeChar(0);
		}
	}
}

//=============================================================================
// Returns the index according to the current size
//=============================================================================
unsigned WorldNode::getIndex(unsigned x, unsigned y, unsigned z) const
{
	unsigned a = (x & size) != 0;
	unsigned b = (y & size) != 0;
	unsigned c = (z & size) != 0;
	return a + (b << 1) + (c << 2);
}


//=============================================================================
// Intializes the WorldNode
//=============================================================================
void WorldNode::initialize()
{
	ASSERT(isPow2(size));
	for(int i = 0; i < 8; i++) nodes[i] = nullptr;

	float cx		= static_cast<float>(this->cx);
	float cy		= static_cast<float>(this->cy);
	float cz		= static_cast<float>(this->cz);
	float cx_dec	= static_cast<float>(this->cx - size);
	float cy_dec	= static_cast<float>(this->cy - size);
	float cz_dec	= static_cast<float>(this->cz - size);
	float cx_inc	= static_cast<float>(this->cx + size);
	float cy_inc	= static_cast<float>(this->cy + size);
	float cz_inc	= static_cast<float>(this->cz + size);

	aabox[0].set(Vec3f(cx_dec, cy_dec, cz_dec), Vec3f(cx    , cy    , cz    ));
	aabox[1].set(Vec3f(cx    , cy_dec, cz_dec), Vec3f(cx_inc, cy    , cz    ));
	aabox[2].set(Vec3f(cx_dec, cy    , cz_dec), Vec3f(cx    , cy_inc, cz    ));
	aabox[3].set(Vec3f(cx    , cy    , cz_dec), Vec3f(cx_inc, cy_inc, cz    ));
	aabox[4].set(Vec3f(cx_dec, cy_dec, cz    ), Vec3f(cx    , cy    , cz_inc));
	aabox[5].set(Vec3f(cx    , cy_dec, cz    ), Vec3f(cx_inc, cy    , cz_inc));
	aabox[6].set(Vec3f(cx_dec, cy    , cz    ), Vec3f(cx    , cy_inc, cz_inc));
	aabox[7].set(Vec3f(cx    , cy    , cz    ), Vec3f(cx_inc, cy_inc, cz_inc));
}