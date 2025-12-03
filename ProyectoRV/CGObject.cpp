#include "CGObject.h"
#include <GL/glew.h>
#include <FreeImage.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


//
// FUNCIÓN: CGObject::ResetLocation()
//
// PROPÓSITO: Asigna la posición inicial del objecto 
//
void CGObject::ResetLocation()
{
	model = glm::mat4(1.0f);
}

void CGObject::Init() {
	Pos = glm::vec3(0.0f, 0.0f, 0.0f);
	Dir = glm::vec3(0.0f, 0.0f, -1.0f);
	Up = glm::vec3(0.0f, 0.0f, 0.0f);
	Right = glm::vec3(1.0f, 0.0f, 0.0f);

	moveStep = 0.0f;

	
	turnStep = 2.5f;
	cosAngle = (GLfloat)cos(glm::radians(turnStep));
	sinAngle = (GLfloat)sin(glm::radians(turnStep));
}

//
// FUNCIÓN: CGObject::SetLocation(glm::mat4 loc)
//
// PROPÓSITO: Asigna la posición del objecto 
//
void CGObject::SetLocation(glm::mat4 loc)
{
	model = loc;
}

//
// FUNCIÓN: CGObject::GetLocation()
//
// PROPÓSITO: Obtiene la posición del objecto 
//
glm::mat4 CGObject::GetLocation()
{
	return model;
}

//
// FUNCIÓN: CGObject::Translate(glm::vec3 t)
//
// PROPÓSITO: Añade un desplazamiento a la matriz de posición del objeto 
//
void CGObject::Translate(glm::vec3 t)
{
	model = glm::translate(model, t);
}

//
// FUNCIÓN: CGObject::Rotate(GLfloat angle, glm::vec3 axis)
//
// PROPÓSITO: Añade una rotación a la matriz de posición del objeto 
//
void CGObject::Rotate(GLfloat angle, glm::vec3 axis)
{
	model = glm::rotate(model, glm::radians(angle), axis);
}

//
// FUNCIÓN: CGObject::Draw(CGShaderProgram * program, glm::mat4 projection, glm::mat4 view)
//
// PROPÓSITO: Dibuja el objeto
//
void CGObject::Draw(CGShaderProgram* program, glm::mat4 projection, glm::mat4 view)
{
	int num = GetNumPieces();
	for (int i = 0; i < num; i++)
	{
		GetPiece(i)->Draw(program, projection, view, model);
	}
}

//
// FUNCIÓN: CGObject::DrawShadow(CGShaderProgram * program, glm::mat4 shadowMatrix)
//
// PROPÓSITO: Dibuja la sombra del objeto dibujando la sombra de las piezas que lo forman
//
void CGObject::DrawShadow(CGShaderProgram* program, glm::mat4 shadowMatrix)
{
	int num = GetNumPieces();
	for (int i = 0; i < num; i++)
	{
		GetPiece(i)->DrawShadow(program, shadowMatrix * model);
	}
}

//
// FUNCIÓN: CGObject::SetPosition(GLfloat x, GLfloat y, GLfloat z)
//
// PROPÓSITO: Establece una nueva posicion al objeto
//
void CGObject::SetPosition(GLfloat x, GLfloat y, GLfloat z)
{
	Pos = glm::vec3(x, y, z);
}

//
// FUNCIÓN: CGObject::GetRealPosition()
//
// PROPÓSITO: Devuelve la posicion del objeto definida en la matriz model
//
glm::vec3 CGObject::GetRealPosition() {
	return glm::vec3(model[3][0], model[3][1], model[3][2]);
}

//
// FUNCIÓN: CGObject::SetDirection(GLfloat xD, GLfloat yD, GLfloat zD, GLfloat xU, GLfloat yU, GLfloat zU)
//
// PROPÓSITO: Establece la dirección del objeto a partir de una serie de coordenadas
//
void CGObject::SetDirection(GLfloat xD, GLfloat yD, GLfloat zD, GLfloat xU, GLfloat yU, GLfloat zU)
{
	Dir = glm::vec3(xD, yD, zD);
	Up = glm::vec3(xU, yU, zU);
	Right = glm::cross(Up, Dir);
}

//
// FUNCIÓN: CGObject::GetDirection()
//
// PROPÓSITO: Devuelve la direccion (Dir) del objeto
//
glm::vec3 CGObject::GetDirection()
{
	return Dir;
}

//
// FUNCIÓN: CGObject::SetMoveStep(GLfloat step)
//
// PROPÓSITO: Establece una nueva velocidad de movimiento (moveStep)
//
void CGObject::SetMoveStep(GLfloat step)
{
	moveStep = step;
}

//
// FUNCIÓN: CGObject::GetMoveStep()
//
// PROPÓSITO: Devuelve la velocidad de movimiento (moveStep)
//
GLfloat CGObject::GetMoveStep() {
	return moveStep;
}

//
// FUNCIÓN: CGObject::GetUpDirection()
//
// PROPÓSITO: Devuelve la dirección sobre el eje y
//
glm::vec3 CGObject::GetUpDirection() {
	return glm::normalize(glm::vec3(model[1][0], model[1][1], model[1][2]));
}

//
// FUNCIÓN: CGObject::GetRightDirection()
//
// PROPÓSITO: Devuelve la dirección sobre el eje x
//
glm::vec3 CGObject::GetRightDirection() {
	return glm::normalize(glm::vec3(model[0][0], model[0][1], model[0][2]));
}

//
// FUNCIÓN: CGObject::getFWDirection()
//
// PROPÓSITO: Devuelve la dirección sobre el eje z
//
glm::vec3 CGObject::getFWDirection() {
	return glm::normalize(glm::vec3(model[2][0], model[2][1], model[2][2]));
}

//
// FUNCIÓN: CGObject::MovAdelante()
//
// PROPÓSITO: Aumenta la velocidad hacia delante (decrementa el moveStep porque el coche está girado)
//
void CGObject::MovAdelante() {
		moveStep -= 0.1f;
}

//
// FUNCIÓN: CGObject::MovAtras()
//
// PROPÓSITO: Aumenta la velocidad hacia atras (aumenta el moveStep porque el coche está girado)
//
void CGObject::MovAtras() {
		moveStep += 0.1f;
}

//
// FUNCIÓN: CGObject::MovLeft()
//
// PROPÓSITO: Rota a la izquierda 
//
void CGObject::MoveLeft()
{
	Dir.x = cosAngle * Dir.x + sinAngle * Right.x;
	Dir.y = cosAngle * Dir.y + sinAngle * Right.y;
	Dir.z = cosAngle * Dir.z + sinAngle * Right.z;
	Right = glm::cross(Up, Dir);

	this->Rotate(turnStep, glm::vec3(0.0f, 1.0f, 0.0f));
}

//
// FUNCIÓN: CGObject::MovRight()
//
// PROPÓSITO: Rota a la derecha 
//
void CGObject::MoveRight()
{
	Dir.x = cosAngle * Dir.x - sinAngle * Right.x;
	Dir.y = cosAngle * Dir.y - sinAngle * Right.y;
	Dir.z = cosAngle * Dir.z - sinAngle * Right.z;
	Right = glm::cross(Up, Dir);

	this->Rotate(-turnStep, glm::vec3(0.0f, 1.0f, 0.0f));
}

//
// FUNCIÓN: CGObject::Actualizar()
//
// PROPÓSITO: Actualiza la posicion del coche, aplicando una fuerza de rozamiento sobre el moveStep 
//
void CGObject::Actualizar() {
	if (moveStep != 0.0f) {
		GLfloat rozamiento = 0.002f;
		if (moveStep > 0.0f) {
			if ((moveStep - rozamiento) >= 0.0f) {
				moveStep -= rozamiento;
			}
			
		}
		else if (moveStep < 0.0f){
			if ((moveStep - rozamiento) <= 0.0f) {
				moveStep += rozamiento;
			}
			
		}
	}
	Pos = glm::vec3(0.0f, 0.0f, 0.0f);
	Pos += moveStep * Dir;
	if (moveStep != 0.0f) {
		this->Translate(Pos);
	}
	
}
