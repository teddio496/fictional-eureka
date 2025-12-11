# Among Us Gathering

## Personal Information

-   **Full Name:** Jaehyeon Heo
-   **UtorID:** heojaehy
-   **Student Number:** 1010364838
-   **Assignment Augmented:** A3 (Ray Tracing)

## Instructions

1. Navigate to the `Code/computer-graphics-ray-tracing` directory.
2. Create a build directory: `mkdir build && cd build`
3. Run CMake: `cmake ../ -DCMAKE_BUILD_TYPE=Release`
4. Build the project: `make`
    - This will build both the raytracer (`raytracing`) and the scene generator (`generate_scene`).
5. (Optional) Regenerate the scene: `./generate_scene > ../src/creative.json`
6. Run the executable with the creative scene file: `./raytracing ../src/creative.json`
7. The output image will be saved as `piece.ppm`.

## Description

I have augmented the ray tracing assignment to render a scene featuring multiple "Among Us" characters gathered on a planetary surface.

### Features Added:

1.  **Procedural Scene Generation:** Created a C++ program (`src/generate_scene.cpp`) to procedurally generate a complex scene (`src/creative.json`) containing:
    -   12 "Among Us" characters placed in a circle/gathering formation.
    -   Each character is composed of multiple spheres (body, visor, legs, backpack) and rotated to face towards the camera with some random variation.
    -   Randomized colors for the characters.
    -   A scattered field of "rocks" (small spheres) on the ground.
    -   A starry background with distant emissive spheres.
2.  **Text Overlay:** Implemented a custom text overlay system in C++ (`include/text_overlay.h`) to draw the piece name, author name, and course info directly onto the rendered image. This removes the need for external post-processing tools.
    -   Implemented a simple 5x7 bitmap font.
    -   Added `draw_text` function to rasterize text onto the image buffer.
    -   Integrated into `main.cpp` to add the required overlay.
3.  **Scene Composition:**
    -   Modified `main.cpp` to use the generated `creative.json` and output to `piece.ppm`.
    -   The scene features a dark, space-like atmosphere with multiple light sources (key light, fill light, and back light) to enhance the 3D form of the spheres.

The code for these features is applied in:

-   `src/generate_scene.cpp`: Logic for creating the JSON scene description.
-   `include/text_overlay.h`: Font data and text drawing logic.
-   `main.cpp`: Integration of text overlay and output filename change.

## Acknowledgements

-   Base code from CSC317 Assignment 3 (Ray Tracing).
-   Eigen library for linear algebra.
-   "Among Us" character design inspired by Innersloth.
