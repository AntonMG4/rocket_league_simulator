#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "CGShaderProgram.h"
#include "CGPiece.h"

//
// CLASE: CGObject
//
// DESCRIPCIÓN: Clase abstracta que representa un objeto formado por varias piezas
// 
class CGObject
{
protected:
	glm::mat4 model;  // Model matrix

	GLushort* indexes; // Array of indexes 
	GLfloat* vertices; // Array of vertices
	GLfloat* normals;  // Array of normals
	GLfloat* textures; // Array of texture coordinates

	GLuint numFaces;
	GLuint VAO;
	GLuint numVertices;
	GLuint VBO[4];
	glm::mat4 location;

public:
	void ResetLocation();
	void Init();
	void Translate(glm::vec3 t);
	void Rotate(GLfloat angle, glm::vec3 axis);
	void SetLocation(glm::mat4 loc);
	glm::mat4 GetLocation();
	void Draw(CGShaderProgram* program, glm::mat4 projection, glm::mat4 view);

	virtual int GetNumPieces() = 0;
	virtual CGPiece* GetPiece(int i) = 0;
	void DrawShadow(CGShaderProgram* program, glm::mat4 shadowMatrix);
	void SetPosition(GLfloat x, GLfloat y, GLfloat z);
	glm::vec3 GetRealPosition();
	void SetDirection(GLfloat xD, GLfloat yD, GLfloat zD, GLfloat xU, GLfloat yU, GLfloat zU);
	glm::vec3 GetDirection();
	void SetMoveStep(GLfloat step);

	GLfloat GetMoveStep();

	glm::vec3 GetUpDirection();

	glm::vec3 GetRightDirection();

	glm::vec3 getFWDirection();

	void MovAdelante();

	void MovAtras();

	void MoveLeft();

	void MoveRight();

	void Actualizar();

private:
	glm::vec3 Pos;
	glm::vec3 Dir;
	glm::vec3 Up;
	glm::vec3 Right;

	GLfloat moveStep;
	GLfloat turnStep;
	GLfloat cosAngle;
	GLfloat sinAngle;
};

