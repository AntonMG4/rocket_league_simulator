#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "CGShaderProgram.h"
#include "CGLight.h"
#include "CGMaterial.h"
#include "CGFigure.h"
#include "CGObject.h"

class CGScene {
public:
    CGScene();
    ~CGScene();
    CGObject* getCoche();
    CGFigure* getBall();
    void Draw(CGShaderProgram* program, glm::mat4 proj, glm::mat4 view, glm::mat4 shadowViewMatrix);
    void DrawShadow(CGShaderProgram* program, glm::mat4 shadowMatrix);
    glm::mat4 GetLightViewMatrix();

private:
    CGFigure* ground;
    CGFigure* wall1;
    CGFigure* wall2;
    CGFigure* wall3;
    CGFigure* wall4;
    CGFigure* wall5;
    CGFigure* wall6;

    CGFigure* fig0;
    CGFigure* fig4;

    CGMaterial* matb;
    CGMaterial* matg;
    CGMaterial* mat0;
    CGMaterial* mat1;
    CGMaterial* mat2;
    CGMaterial* mat3;
    CGMaterial* mat4;

    CGLight* light;
    CGObject* object;
};
