#include "Moskvitch.h"
#include "Moskvitch_pieces.h"
#include <GL/glew.h>
#include "CGObject.h"
#include "CGPiece.h"

Moskvitch::Moskvitch()
{
	model = glm::mat4(1.0f);

	mtl[0] = new CGMaterial();
	mtl[0]->SetAmbientReflect(0.588f, 0.588f, 0.588f);
	mtl[0]->SetDifusseReflect(0.588f, 0.588f, 0.588f);
	mtl[0]->SetSpecularReflect(0.0f, 0.0f, 0.0f);
	mtl[0]->SetShininess(10.0f);
	mtl[0]->InitTexture("textures/amarillo2.jpg");

	pieces[0] = new Moskvitch_0(mtl[0]);
}

Moskvitch::~Moskvitch()
{
	for (int i = 0; i < 1; i++) delete pieces[i];
	delete[] pieces;
	for (int i = 0; i < 1; i++) delete mtl[i];
	delete[] mtl;
}

int Moskvitch::GetNumPieces()
{
	return 1;
}

CGPiece* Moskvitch::GetPiece(int index)
{
	return pieces[index];
}

