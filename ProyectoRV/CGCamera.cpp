#include "CGCamera.h"
#include <GL/glew.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//
// FUNCIÓN: CGCamera::CGCamera()
//
// PROPÓSITO: Construye una cámara
//
// COMENTARIOS: 
//     La posición inicial es (0,0,0).
//     La orientación incial es el sistema de coordenadas del modelo
//     El tamaño del paso inicial es 0.1
//     El tamaño del giro inicial es 1.0 grados
//
CGCamera::CGCamera()
{
    Pos = glm::vec3(0.0f, 0.0f, 0.0f);
    Dir = glm::vec3(0.0f, 0.0f, 1.0f);
    Up = glm::vec3(0.0f, 1.0f, 0.0f);
    Right = glm::vec3(1.0f, 0.0f, 0.0f);
}

//
// FUNCIÓN: CGCamera::ViewMatrix()
//
// PROPÓSITO: Obtiene la matriz View para situar la cámara.
//
glm::mat4 CGCamera::ViewMatrix()
{
    return glm::lookAt(Pos, Pos - Dir, Up);
}

//
// FUNCIÓN: CGCamera::SetPosition(GLfloat x, GLfloat y, GLfloat z)
//
// PROPÓSITO: Asigna la posición de la cámara con respecto al 
//            sistema de coordenadas del modelo.
//
void CGCamera::SetPosition(GLfloat x, GLfloat y, GLfloat z)
{
    Pos = glm::vec3(x, y, z);
}

//
// FUNCIÓN: CGCamera::SetDirection(GLfloat xDir, GLfloat yDir, GLfloat zDir, 
//                                 GLfloat xUp, GLfloat yUp, GLfloat zUp)
//
// PROPÓSITO: Asigna la orientación de la cámara.
//
void CGCamera::SetDirection(GLfloat xD, GLfloat yD, GLfloat zD, GLfloat xU, GLfloat yU, GLfloat zU)
{
    Dir = glm::vec3(xD, yD, zD);
    Up = glm::vec3(xU, yU, zU);
    Right = glm::cross(Up, Dir);
}

void CGCamera::SetD(glm::vec3 v) {
    Dir = glm::vec3(v);
    Right = glm::cross(Up, Dir);
}








//
// FUNCIÓN: CGCamera::GetPosition()
//
// PROPÓSITO: Obtiene la posición de la cámara.
//
glm::vec3 CGCamera::GetPosition()
{
    return Pos;
}

//
// FUNCIÓN: CGCamera::GetDirection()
//
// PROPÓSITO: Obtiene la orientación de la cámara (eje Z).
//
glm::vec3 CGCamera::GetDirection()
{
    return Dir;
}

//
// FUNCIÓN: CGCamera::GetUpDirection()
//
// PROPÓSITO: Obtiene la orientación cenital de la cámara (eje Y).
//
glm::vec3 CGCamera::GetUpDirection()
{
    return Up;
}





