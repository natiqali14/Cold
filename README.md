# Cold Engine

Welcome to the **Cold Engine** repository! 🎨🚀  
This is a work-in-progress **3D graphics engine** built using **C++** and **OpenGL**, designed for developers and enthusiasts exploring real-time rendering.

---

## Features

1. **OBJ File Support**  
   You can upload and render **.obj** files directly with the engine.

2. **Basic Lighting**  
   - Implemented **Phong Shading Model** for realistic lighting and material interactions.  
   - **Shadows** are currently under development to enhance scene realism.

3. **Cross-Platform Compatibility**  
   - Works on **macOS** (optimized for Apple M-series chips) and **Windows** systems.  

---

## Getting Started

### Prerequisites

- **C++17** or higher (tested on macOS and Windows environments).
- Write now su OpenGL 3.3+ compatible GPU and drivers.  
- Required libraries:
  - [GLFW](https://www.glfw.org/) for windowing and input handling.
  - [GLM](https://github.com/g-truc/glm) for mathematics.

### Build Instructions

1. Clone this repository:  
   ```bash
   git clone https://github.com/yourusername/cold-engine.git
   cd cold-engine
   mkdir build && cd build
   cmake ..
   cmake --build
