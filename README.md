<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project
This project was develouped as an university assignment for my Rendering Engine's cource. The goal of the project was to create a custom real-time 3D rendering engine in C++ using OpenGL. The engine features three distinct changable scenes, Phong lighting and an ImGui control panel for dynamically changing different variables withought recompiling. 

<img width="2108" height="1332" alt="Scene 0" src="https://github.com/user-attachments/assets/55caf4db-2873-4e72-89ef-69aefd093e2e" />
<img width="2548" height="1319" alt="Grayscale" src="https://github.com/user-attachments/assets/ef1512c5-b313-470e-a7c9-89cfc30e8716" />
<img width="2544" height="1313" alt="Scene 2" src="https://github.com/user-attachments/assets/b8a21cd7-82c2-4b6d-9cbd-3f4b554bf2ea" />



<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Built With

* [![C++][C++.img]][C++-url]
* [![OpenGL][OpenGL.img]][OpenGL-url]
* [ImGui][ImGui-url]

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Getting Started

### Prerequisites

* Windows with Visual Studio 2022 (or later)
* vcpkg installed and integrated

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- USAGE EXAMPLES -->
## Usage
Once running, a control panel appears in the top left corner of the window. Use it to switch between different scenes adn to change different settings.
**Controls:**
* W / A / S / D - Forward / Left / Backwards / Right movement
* Space - Upwards movement
* Right mouse button (hold) - Look around
* Z / X - Zoom in / out
* Esc - quit the application

**Scenes:** 
* Scene 0 - textured objects
* Scene 1 - Unlit models (showing normals)
* Scene 2 - Phone lighting with light controls

**Post-Processing Effects**
* Grayscale - convert everything to black and white
* Invert - invert all colours
* Pixelize - pixelize the screen; change the amount with a slider
* Edge detection - show the edges of objects

<p align="right">(<a href="#readme-top">back to top</a>)</p>

[C++.img]: https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=C%2B%2B&logoColor=white
[C++-url]: https://isocpp.org/
[OpenGL.img]: https://img.shields.io/badge/OpenGL-5586A4?style=for-the-badge&logo=opengl&logoColor=white
[OpenGL-url]: https://learnopengl.com/
[ImGui-url]: https://github.com/ocornut/imgui
