# 🏎️ Rocket League Training - Custom Graphics Engine 
# (C++ & OpenGL)

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![OpenGL](https://img.shields.io/badge/OpenGL-%23FFFFFF.svg?style=for-the-badge&logo=opengl)
![Visual Studio](https://img.shields.io/badge/Visual%20Studio-5C2D91.svg?style=for-the-badge&logo=visual-studio&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-yellow.svg?style=for-the-badge)

This project is a 3D simulation inspired by the video game **Rocket League**, built entirely from scratch implementing a **custom graphics engine** in C++ and OpenGL.

Unlike projects built on commercial engines (Unity/Unreal), this repository demonstrates the implementation of low-level graphics architecture, including shader management, vector mathematics for camera control, and custom physics for collision handling.

![Gameplay Screenshot](YOUR_IMAGE_URL_HERE.png)
*Third-person view of the car and ball featuring custom lighting and shadow mapping.*

## 📋 Project Overview

The simulation recreates a 3D arena where the player controls a vehicle to push a giant ball into a goal. The core focus of the project is the interaction between object kinematics and the rendering pipeline.

### Key Features
* **Custom Rendering Pipeline:** Implementation of VAOs (Vertex Array Objects) and VBOs for optimized rendering.
* **Shaders & Lighting:** Custom GLSL shaders handling ambient, diffuse, and specular lighting, as well as shadow mapping.
* **Physics Engine:**
    * **Car Physics:** Movement with friction/inertia simulation.
    * **Collisions:** Euclidean distance calculations for Sphere-Box (Ball-Car) and Box-Box (Car-Walls) collisions.
    * **Response:** Dynamic momentum transfer when the car hits the ball.
* **Smart Camera:** A third-person camera system that smoothly tracks the vehicle using vector operations (`Position = CarPos - Forward*75 + Up*15`).

## 🛠️ Tech Stack & Dependencies

The project is built using **Visual Studio** and standard C++ libraries.

* **Language:** C++ (Modern Standard)
* **Graphics API:** OpenGL 3.3+
* **Math:** GLM (OpenGL Mathematics)
* **Window & Input:** GLFW
* **Extension Loader:** GLEW
* **Textures:** FreeImage

## 🎮 Controls

The movement logic includes acceleration and friction variables (`moveStep`) to simulate realistic driving weight.

| Action | Key | Description |
| :--- | :---: | :--- |
| **Accelerate** | `⬆️` Up Arrow | Increases forward velocity vector. |
| **Brake / Reverse** | `⬇️` Down Arrow | Decreases velocity or moves backward. |
| **Turn Left** | `⬅️` Left Arrow | Rotates direction vector on the Y-axis. |
| **Turn Right** | `➡️` Right Arrow | Rotates direction vector on the Y-axis. |

## 🚀 Installation & Execution

1.  **Prerequisites:**
    * Visual Studio 2019/2022 with "Desktop development with C++" workload.
    * A graphics card supporting OpenGL 3.3 or higher.

2.  **Clone the repository:**
    ```bash
    git clone [https://github.com/your-username/Rocket-League-Training-CPP.git](https://github.com/your-username/Rocket-League-Training-CPP.git)
    ```

3.  **Setup:**
    * Open `ProyectoRV.sln` in Visual Studio.
    * Set the build configuration to **Release** and **x64**.
    * **Important:** Ensure the required DLLs (`glew32.dll`, `glfw3.dll`, `FreeImage.dll`) are in the executable folder (`x64/Release`) or in your system PATH.

4.  **Run:**
    * Press `F5` or click "Local Windows Debugger".

## 📂 Project Structure

The architecture isolates rendering logic from game physics.

```text
ProyectoRV/
├── shaders/          # GLSL Source code (.vert, .frag)
├── textures/         # Assets for ground, skybox, and models
├── CGApplication.cpp # Main game loop and window management
├── CGScene.cpp       # Scene graph, lighting, and object rendering
├── CGCamera.cpp      # View matrix and camera vector math
├── CGModel.cpp       # Physics update loop and collision constraints
├── CGObject.cpp      # Dynamic objects (Car) logic
├── CGFigure.cpp      # Static geometry (Walls, Goal) logic
├── main.cpp          # Entry point
└── ... (.h y .cpp from motor)
