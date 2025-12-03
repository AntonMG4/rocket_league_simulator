#include "CGModel.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "CGCamera.h"
#include "CGScene.h"
#include "CGSkybox.h"
#include "resource.h"
#include <iostream>


//
// FUNCIÓN: CGModel::initialize(int, int)
//
// PROPÓSITO: Initializa el modelo 3D
//
void CGModel::initialize(int w, int h)
{
    // Crea el programa gráfico para el entorno
    skyboxProgram = new CGShaderProgram(IDR_SHADER1, IDR_SHADER2, -1, -1, -1);
    if (skyboxProgram->IsLinked() == GL_FALSE) return;

    // Crea el programa gráfico para la sombra
    shadowProgram = new CGShaderProgram(IDR_SHADER5, IDR_SHADER6, -1, -1, -1);
    if (shadowProgram->IsLinked() == GL_FALSE) return;

    // Crea el programa gráfico para la escena
    sceneProgram = new CGShaderProgram(IDR_SHADER3, IDR_SHADER4, -1, -1, -1);
    if (sceneProgram->IsLinked() == GL_FALSE) return;

    // Crea la cámara
    camera = new CGCamera(); // 5 y 50
    camera->SetPosition(0.0f, 30.0f, 80.0f);
    

    // Crea el skybox
    skybox = new CGSkybox();

    // Crea la escena
    scene = new CGScene();

    // Crea el Framebuffer de la sombra
    bool frameBufferStatus = InitShadowMap();
    if (!frameBufferStatus) return;

    // Inicializa la textura para antializasing
    BuildOffsetTex(4, 4, 4);

    // Asigna el viewport y el clipping volume
    resize(w, h);

    // Opciones de dibujo
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Inicializa el coche
    coche = scene->getCoche();

    scene->getBall()->SetUpStep(0.02f);
}

//
// FUNCIÓN: CGModel::finalize()
//
// PROPÓSITO: Libera los recursos del modelo 3D
//
void CGModel::finalize()
{
    delete camera;
    delete scene;
    delete skybox;
    delete sceneProgram;
    delete skyboxProgram;
    delete shadowProgram;
}

//
// FUNCIÓN: CGModel::resize(int w, int h)
//
// PROPÓSITO: Asigna el viewport y el clipping volume
//
void CGModel::resize(int w, int h)
{
    double fov = glm::radians(15.0);
    double sin_fov = sin(fov);
    double cos_fov = cos(fov);
    if (h == 0) h = 1;
    GLfloat aspectRatio = (GLfloat)w / (GLfloat)h;
    GLfloat wHeight = (GLfloat)(sin_fov * 0.2 / cos_fov);
    GLfloat wWidth = wHeight * aspectRatio;

    wndWidth = w;
    wndHeight = h;

    glViewport(0, 0, w, h);
    projection = glm::frustum(-wWidth, wWidth, -wHeight, wHeight, 0.2f, 400.0f);
}

//
// FUNCIÓN: CGModel::render()
//
// PROPÓSITO: Genera la imagen
//
void CGModel::render()
{
    //*********************************************************//
    //                  Genera el ShadowMap                    //
    //*********************************************************//

    // Activa el framebuffer de la sombra
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

    // Activa el programa que genera el shadowmap
    shadowProgram->Use();

    // Asigna las matrices Viewport, View y Projection de la luz.
    glm::mat4 lightViewMatrix = scene->GetLightViewMatrix();
    glm::mat4 lightPerspective = glm::ortho(-150.0f, 150.0f, -150.0f, 150.0f, 0.0f, 400.0f);
    glm::mat4 lightMVP = lightPerspective * lightViewMatrix;

    // Limpia la información de profundidad
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Activa front-face culling
    glCullFace(GL_FRONT);

    //Asigna el viewport
    glViewport(0, 0, 1024, 1024);

    // Dibuja la escena
    scene->DrawShadow(shadowProgram, lightMVP);

    //*********************************************************//
    //                  Dibuja el skybox                       //
    //*********************************************************//
    
    // Activa el framebuffer de la imagen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Limpia el framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Activa back-face culling
    glCullFace(GL_BACK);

    // Asigna el viewport
    glViewport(0, 0, wndWidth, wndHeight);

    // Dibuja el skybox
    glm::mat4 view = camera->ViewMatrix();
    skyboxProgram->Use();
    skybox->Draw(skyboxProgram, projection, view);

    //*********************************************************//
    //                  Dibuja la escena                       //
    //*********************************************************//

    // Activa el programa de la escena
    sceneProgram->Use();
    sceneProgram->SetUniformI("ShadowMap", 1);

    // Asigna las variables para anitializasing
    glm::vec3 offsetTexSize = glm::vec3(4, 4, 8);
    sceneProgram->SetUniformI("OffsetTex", 1);
    sceneProgram->SetUniformVec3("OffsetTexSize", offsetTexSize);
    sceneProgram->SetUniformF("Radius", 0.003);

    // Dibuja la escena
    glm::mat4 viewMatrix = camera->ViewMatrix();
    scene->Draw(sceneProgram, projection, viewMatrix, lightMVP);
}

//
// FUNCIÓN: CGModel::update()
//
// PROPÓSITO: Anima la escena
//
void CGModel::update()
{
    colocarCamara();
    CarConstraints();
    BallConstraints();
    BallCarConstraints();
    coche->Actualizar();
    scene->getBall()->MoveFront();

}

//
// FUNCIÓN: CGModel::key_pressed(int)
//
// PROPÓSITO: Respuesta a acciones de teclado
//
void CGModel::key_pressed(int key)
{
    switch (key)
    {
    case GLFW_KEY_UP:
        coche = scene->getCoche();
        coche->MovAdelante();        
        break;
    case GLFW_KEY_DOWN:
        coche = scene->getCoche();
        coche->MovAtras();
        break;
    case GLFW_KEY_LEFT:
        coche = scene->getCoche();
        if (coche->GetMoveStep() > 0.1f || coche->GetMoveStep() < -0.1f) {
            coche->MoveLeft();
        }
        break;
    case GLFW_KEY_RIGHT:
        coche = scene->getCoche();
        if (coche->GetMoveStep() > 0.1f || coche->GetMoveStep() < -0.1f) {
            coche->MoveRight();
        }
        break;
    }

}

//
//  FUNCIÓN: CGModel:::mouse_button(int button, int action)
//
//  PROPÓSITO: Respuesta del modelo a un click del ratón.
//
void CGModel::mouse_button(int button, int action)
{
}

//
//  FUNCIÓN: CGModel::mouse_move(double xpos, double ypos)
//
//  PROPÓSITO: Respuesta del modelo a un movimiento del ratón.
//
void CGModel::mouse_move(double xpos, double ypos)
{
}

//
//  FUNCIÓN: CGModel::colocarCamara()
//
//  PROPÓSITO: Coloca la camara detrás del coche en cada momento
//
void CGModel::colocarCamara() {
    glm::vec3 posR = scene->getCoche()->GetRealPosition();
    glm::vec3 forward = scene->getCoche()->getFWDirection();
    glm::vec3 up = scene->getCoche()->GetUpDirection();
    glm::vec3 right = scene->getCoche()->GetRightDirection();
    glm::vec3 camPos = posR - (forward * 75.0f) + (up * 15.0f);

    camera->SetPosition(camPos.x, camPos.y, camPos.z);
    
    camera->SetD(-forward);
    
}

//
//  FUNCIÓN: CGModel::CarConstraints()
//
//  PROPÓSITO: Establece las físicas de choque del coche con las paredes
//
void CGModel::CarConstraints() {
    glm::vec3 pos = scene->getCoche()->GetRealPosition();
    int constraint = 0;
    if (fueraCoche && scene->getCoche()->GetMoveStep() > 0)
        scene->getCoche()->SetMoveStep(-(scene->getCoche()->GetMoveStep()));

    if (mAtras && scene->getCoche()->GetMoveStep() < 0)
        scene->getCoche()->SetMoveStep(-(scene->getCoche()->GetMoveStep()));

    if (pos.x > 110.0f || pos.x < -110.0f || pos.z > 150.0f) {
        constraint = 1;
    }
    if (pos.z < -150.0f && (pos.x > 44.0f || pos.x < -44.0f))
        constraint = 1;
    if (pos.z < -165.0f && (pos.x <= 44.0f || pos.x >= -44.0f))
        constraint = 1;

    if (constraint == 1) {
        if (fueraCoche == false && mAtras == false && scene->getCoche()->GetMoveStep() > 0) {
            scene->getCoche()->SetMoveStep(-(scene->getCoche()->GetMoveStep() * 0.2));
            fueraCoche = true;
        }
        else if (fueraCoche == false && mAtras == false && scene->getCoche()->GetMoveStep() < 0) {
            scene->getCoche()->SetMoveStep(-(scene->getCoche()->GetMoveStep() * 0.2));
            mAtras = true;
        }
    }
    else if (constraint == 0) {
        fueraCoche = false;
        mAtras = false;
    }
    
}

//
//  FUNCIÓN: CGModel::BallConstraints()
//
//  PROPÓSITO: Establece las físicas de choque del balón con las paredes
//
void CGModel::BallConstraints() {
    glm::vec3 pos = scene->getBall()->GetRealPosition();
    int constraint = 0;
    bool px = false;
    bool pz = false;
    glm::vec3 dir = scene->getBall()->GetDirection();
    


    if (fueraBalon == true && scene->getBall()->GetMoveStep() > 0)
        scene->getBall()->SetMoveStep(-(scene->getBall()->GetMoveStep()));

    if (mAtrasB == true && scene->getBall()->GetMoveStep() < 0)
        scene->getBall()->SetMoveStep(-(scene->getBall()->GetMoveStep()));

    if (pos.x > (120.0f - 6.5f)) {
        constraint = 1;
        px = true;
    }
    if (pos.x < (6.5f - 120.0f)) {
        constraint = 1;
        px = true;
    }
    if (pos.z > (160.0f - 6.5f)) {
        constraint = 1;
        pz = true;
    }
    if (pos.z < (6.5f - 160.0f) && (pos.x > (-6.5f + 44.0f)
        || pos.x < (6.5f - 44.0f))) {
        constraint = 1;
        pz = true;
    }
    if (pos.z < (-160.0f) && ((pos.x <= (-6.5f + 44.0f) 
        || pos.x >= (6.5f - 44.0f)))) {
        std::cout << "GOL";
        exit(0);
    }
        

    if (constraint == 1) {
        if (scene->getBall()->GetMoveStep() > 0 && !mAtrasB && !fueraBalon) {
            if (px == true) {
                scene->getBall()->SetDirection(glm::vec3(dir.x, dir.y, -dir.z));
            }
            else if (pz == true) {
                scene->getBall()->SetDirection(glm::vec3(-dir.x, dir.y, dir.z));
            }
            scene->getBall()->SetMoveStep(-(scene->getBall()->GetMoveStep() * 0.4f));
            fueraBalon = true;
        }
        else if (scene->getBall()->GetMoveStep() < 0 && !fueraBalon && !mAtrasB) {
            if (px == true) {
                scene->getBall()->SetDirection(glm::vec3(dir.x, dir.y, -dir.z));
            }
            else if (pz == true) {
                scene->getBall()->SetDirection(glm::vec3(-dir.x, dir.y, dir.z));
            }
            scene->getBall()->SetMoveStep(-(scene->getBall()->GetMoveStep() * 0.4f));
            mAtrasB = true;
        }
    }
    else if (constraint == 0) {
        fueraBalon = false;
        mAtrasB = false;
    }

}

//
//  FUNCIÓN: CGModel::BallCarConstraints()
//
//  PROPÓSITO: Establece las físicas de choque del balón con el coche
//
void CGModel::BallCarConstraints() {
    glm::vec3 posBall = scene->getBall()->GetRealPosition();
    glm::vec3 posCar = scene->getCoche()->GetRealPosition();

    float disx = posBall.x - posCar.x;
    float disy = posBall.y - posCar.y;
    float disz = posBall.z - posCar.z;
    float disxyz = sqrt((disx * disx) + (disz * disz) + (disy * disy));

    if (disxyz <= 12.0f) {
        if(scene->getCoche()->GetMoveStep() < 0.0f) // si el coche marcha atras
            scene->getBall()->SetDirection(glm::vec3(-disx, -disy, -disz));
        else
            scene->getBall()->SetDirection(glm::vec3(disx, disy, disz));

        scene->getBall()->SetMoveStep(scene->getCoche()->GetMoveStep() * 0.4f);
        scene->getBall()->SetUpStep((scene->getCoche()->GetMoveStep() * 0.30f)
            + scene->getBall()->GetUpStep());
    }
    
    
}

//
// FUNCIÓN: CGModel::InitShadowMap()
//
// PROPÓSITO: Inicializa el FBO para almacenar la textura de sombra
//
bool CGModel::InitShadowMap()
{
    GLfloat border[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLsizei shadowMapWidth = 1024;
    GLsizei shadowMapHeight = 1024;

    glGenFramebuffers(1, &shadowFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

    glGenTextures(1, &depthTexId);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthTexId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, shadowMapWidth,
        shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D, depthTexId, 0);

    glDrawBuffer(GL_NONE);

    bool result = true;
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        result = false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return result;
}

void CGModel::BuildOffsetTex(int texSize, int samplesU, int samplesV)
{
    float TWOPI = glm::radians(360.0);
    int size = texSize;
    int samples = samplesU * samplesV;
    int bufSize = size * size * samples * 2;
    float* data = new float[bufSize];

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            for (int k = 0; k < samples; k += 2)
            {
                int x1, y1, x2, y2;
                x1 = k % (samplesU);
                y1 = (samples - 1 - k) / samplesU;
                x2 = (k + 1) % samplesU;
                y2 = (samples - 1 - k - 1) / samplesU;

                glm::vec4 v;
                // Center on grid and jitter
                v.x = (x1 + 0.5f) + jitter();
                v.y = (y1 + 0.5f) + jitter();
                v.z = (x2 + 0.5f) + jitter();
                v.w = (y2 + 0.5f) + jitter();

                // Scale between 0 and 1
                v.x /= samplesU;
                v.y /= samplesV;
                v.z /= samplesU;
                v.w /= samplesV;

                // Warp to disk
                int cell = ((k / 2) * size * size + j * size + i) * 4;
                data[cell + 0] = sqrtf(v.y) * cosf(TWOPI * v.x);
                data[cell + 1] = sqrtf(v.y) * sinf(TWOPI * v.x);
                data[cell + 2] = sqrtf(v.w) * cosf(TWOPI * v.z);
                data[cell + 3] = sqrtf(v.w) * sinf(TWOPI * v.z);
            }
        }
    }
    glActiveTexture(GL_TEXTURE1);

    glGenTextures(1, &offsetTexId);
    glBindTexture(GL_TEXTURE_3D, offsetTexId);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, size, size, samples / 2, 0,
        GL_RGBA, GL_FLOAT, data);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    delete[] data;
}

//
// FUNCIÓN: CGModel::jitter() 
//
// PROPÓSITO: Genera un número aleatorio entre -0.5 y 0.5;
//
float CGModel::jitter()
{
    return ((float)rand() / RAND_MAX) - 0.5f;
}