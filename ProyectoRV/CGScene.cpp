#include "CGScene.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "CGShaderProgram.h"
#include "CGFigure.h"
#include "CGLight.h"
#include "CGMaterial.h"
#include "CGPorteria.h"
#include "CGSphere.h"
#include "CGGround.h"
#include "resource.h"
#include "Moskvitch.h"

//
// FUNCIÓN: CGScene::CGScene()
//
// PROPÓSITO: Construye el objeto que representa la escena
//
CGScene::CGScene()
{

    // Iluminacion

    glm::vec3 Ldir = glm::vec3(0.0f, -1.0f, 0.8f);
    Ldir = glm::normalize(Ldir);
    light = new CGLight();
    light->SetLightDirection(Ldir);
    light->SetAmbientLight(glm::vec3(0.2f, 0.2f, 0.2f));
    light->SetDifusseLight(glm::vec3(0.8f, 0.8f, 0.8f));
    light->SetSpecularLight(glm::vec3(1.0f, 1.0f, 1.0f));

    // Texturas

    matg = new CGMaterial();
    matg->SetAmbientReflect(1.0f, 1.0f, 1.0f);
    matg->SetDifusseReflect(1.0f, 1.0f, 1.0f);
    matg->SetSpecularReflect(0.8f, 0.8f, 0.8f);
    matg->SetShininess(16.0f);
    matg->InitTexture("textures/ground.jpg");

    mat0 = new CGMaterial();
    mat0->SetAmbientReflect(3.0f, 3.0f, 3.0f);
    mat0->SetDifusseReflect(1.0f, 1.0f, 1.0f);
    mat0->SetSpecularReflect(0.8f, 0.8f, 0.8f);
    mat0->SetShininess(150.0f);
    mat0->InitTexture("textures/wall.jpg");

    mat1 = new CGMaterial();
    mat1->SetAmbientReflect(5.0f, 5.0f, 5.0f);
    mat1->SetDifusseReflect(1.0f, 1.0f, 1.0f);
    mat1->SetSpecularReflect(0.8f, 0.8f, 0.8f);
    mat1->SetShininess(16.0f);
    mat1->InitTexture("textures/wall2.jpg");

    mat2 = new CGMaterial();
    mat2->SetAmbientReflect(5.0f, 5.0f, 5.0f);
    mat2->SetDifusseReflect(1.0f, 1.0f, 1.0f);
    mat2->SetSpecularReflect(0.8f, 0.8f, 0.8f);
    mat2->SetShininess(16.0f);
    mat2->InitTexture("textures/wall3.jpg");

    mat3 = new CGMaterial();
    mat3->SetAmbientReflect(5.0f, 5.0f, 5.0f);
    mat3->SetDifusseReflect(1.0f, 1.0f, 1.0f);
    mat3->SetSpecularReflect(0.8f, 0.8f, 0.8f);
    mat3->SetShininess(16.0f);
    mat3->InitTexture("textures/porteria_text.jpg");

    mat4 = new CGMaterial();
    mat4->SetAmbientReflect(1.0f, 1.0f, 1.0f);
    mat4->SetDifusseReflect(1.0f, 1.0f, 1.0f);
    mat4->SetSpecularReflect(0.8f, 0.8f, 0.8f);
    mat4->SetShininess(16.0f);
    mat4->InitTexture("textures/ball.jpg");

    // Suelo

    ground = new CGGround(120.00f, 160.0f);
    ground->SetMaterial(matg);
    
    // Paredes

    wall1 = new CGGround(25.00f, 160.0f);
    wall1->SetMaterial(mat0);
    wall1->Translate(glm::vec3(-120.0f, 25.0f, 0.0f));
    wall1->Rotate(-90.0f, glm::vec3(0.0f, 0.0f, 1.0f));

    wall2 = new CGGround(25.00f, 160.0f);
    wall2->SetMaterial(mat0);
    wall2->Translate(glm::vec3(120.0f, 25.0f, 0.0f));
    wall2->Rotate(90.0f, glm::vec3(0.0f, 0.0f, 1.0f));

    wall3 = new CGGround(120.00f, 25.00f);
    wall3->SetMaterial(mat1);
    wall3->Translate(glm::vec3(0.0f, 25.0f, 160.0f));
    wall3->Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    wall4 = new CGGround(44.00f, 25.00f);
    wall4->SetMaterial(mat1);
    wall4->Translate(glm::vec3(-88.00f, 25.0f, -160.0f));
    wall4->Rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    wall5 = new CGGround(44.00f, 7.00f);
    wall5->SetMaterial(mat2);
    wall5->Translate(glm::vec3(0.00f, 43.0f, -160.0f));
    wall5->Rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    wall6 = new CGGround(44.00f, 25.00f);
    wall6->SetMaterial(mat1);
    wall6->Translate(glm::vec3(88.00f, 25.0f, -160.0f));
    wall6->Rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    // Coche

    object = new Moskvitch();
    object->Rotate(180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    object->Init();

    // Porteria

    fig0 = new CGPorteria(18.0f, 10.0f);
    fig0->SetMaterial(mat3);
    fig0->Translate(glm::vec3(18.0f * 1.5, 18.0f, -170.0f));
    fig0->Rotate(180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    
    // Pelota

    fig4 = new CGSphere(20, 40, 6.5f);
    fig4->SetMaterial(mat4);
    fig4->Translate(glm::vec3(0.0f, 6.5f, -20.0f));
    
}

//
// FUNCIÓN: CGScene3:~CGScene()
//
// PROPÓSITO: Destruye el objeto que representa la escena
//
CGScene::~CGScene()
{
    delete ground;
    delete wall1;
    delete wall2;
    delete wall3;
    delete wall4;
    delete wall5;
    delete wall6;
    
    delete fig0;
    delete fig4;
    
    delete light;
    delete matg;
    delete mat0;
    delete mat1;
    delete mat2;
    delete mat3;
    delete mat4;
    
    delete object;
}

//
// FUNCIÓN: CGScene::getCoche()
// PROPÓSITO: Devuelve el objeto coche
//
CGObject* CGScene::getCoche() {
    return object;
}

//
// FUNCIÓN: CGScene::getBall()
// PROPÓSITO: Devuelve la figura pelota
//
CGFigure* CGScene::getBall() {
    return fig4;
}

//
// FUNCIÓN: CGScene::Draw()
//
// PROPÓSITO: Dibuja la escena
//
void CGScene::Draw(CGShaderProgram* program, glm::mat4 proj, glm::mat4 view, glm::mat4 shadowViewMatrix)
{
    light->SetUniforms(program);

    ground->Draw(program, proj, view, shadowViewMatrix);
    wall1->Draw(program, proj, view, shadowViewMatrix);
    wall2->Draw(program, proj, view, shadowViewMatrix);
    wall3->Draw(program, proj, view, shadowViewMatrix);
    wall4->Draw(program, proj, view, shadowViewMatrix);
    wall5->Draw(program, proj, view, shadowViewMatrix);
    wall6->Draw(program, proj, view, shadowViewMatrix);
    fig0->Draw(program, proj, view, shadowViewMatrix);
    fig4->Draw(program, proj, view, shadowViewMatrix);
    object->Draw(program, proj, view);
}

//
// FUNCIÓN: CGScene::DrawShadow()
//
// PROPÓSITO: Dibuja las sombras de la escena
//
void CGScene::DrawShadow(CGShaderProgram* program, glm::mat4 shadowMatrix)
{
    wall1->DrawShadow(program, shadowMatrix);
    wall2->DrawShadow(program, shadowMatrix);
    wall3->DrawShadow(program, shadowMatrix);
    wall4->DrawShadow(program, shadowMatrix);
    wall5->DrawShadow(program, shadowMatrix);
    wall6->DrawShadow(program, shadowMatrix);
    fig0->DrawShadow(program, shadowMatrix);
    fig4->DrawShadow(program, shadowMatrix);
    object->DrawShadow(program, shadowMatrix);
    
}

//
// FUNCIÓN: CGScene::GetLightViewMatrix()
//
// PROPÓSITO: Obtiene la matriz de posicionamiento de la luz
//
glm::mat4 CGScene::GetLightViewMatrix()
{
    glm::vec3 Zdir = -(light->GetLightDirection());
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 Xdir = glm::normalize(glm::cross(Up, Zdir));
    glm::vec3 Ydir = glm::cross(Zdir, Xdir);
    glm::vec3 Zpos = 150.0f * Zdir;
    glm::vec3 Center = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::mat4 view = glm::lookAt(Zpos, Center, Ydir);
    return view;
}