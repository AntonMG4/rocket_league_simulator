#pragma once

#include <GL/glew.h>
#include "CGObject.h"
#include "CGPiece.h"

class Moskvitch : public CGObject {
private:
	CGMaterial* mtl[1];
	CGPiece* pieces[1];

public:
	Moskvitch();
	~Moskvitch();
	virtual int GetNumPieces();
	virtual CGPiece* GetPiece(int i);
};

