# 3D Base Glut

## Dependencies
- **Glut** : is a C++ library for OpenGL programs.
- **Eigen** : is a C++ library for linear algebra (Version 3.4.0 Standalone).
- **Happly** : is a C++ library for reading and writing PLY files (Version 2.0 Standalone).

## Controls
- **W** : Switch to Wireframe mode.
- **P** : Switch to Point mode.
- **N** : Print Normals.
- **F** : Toggle Full Screen mode.
- **Mouse Left Click** : Rotate the camera.
- **Mouse Right Click** : Move the camera (X,Y).
- **Mouse Wheel** : Move the camera (Z).

## Compilation
- **make -j && ./tp <mesh.ply> <qp>**

## Params program
- **first param** : Path to the input PLY mesh.
- **second param** : Quantization parameter. 