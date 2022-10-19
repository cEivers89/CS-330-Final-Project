# Final Project

For this project, we were instructed to create a 3D scene from a 2D picture using C++ and the OpenGL graphics library.

Original image:
![20220905_165233](https://user-images.githubusercontent.com/59654582/196763729-a8ad30f7-ca9a-410a-8f54-66a69cabff0e.jpg)

Completed scene:
![image](https://user-images.githubusercontent.com/59654582/196765062-51b8ce69-6de7-417f-95aa-0ed5f42455cc.png)


Design Approach:
I intended to make this project modular from the start so that I could reuse parts of it in future iterations. This necessitated separating the algorithms for form construction from the methods for rendering the scene. Furthermore, I believed that consolidating all scene-creating elements into their own class would make the program more user-friendly, allowing the user to focus on merely adding shapes and texturing them rather than scrolling through hundreds of lines of code. To do this, I refactored the GLMesh struct to accept all of the shape attributes, after which the mesh object is sent to the Shapes class to be created.

This configuration allowed me to save code and simply reuse algorithms. For example, while building a new cylinder, I just needed to create a new mesh object with the necessary charactistics (size, rotation, texture, and so on). The user may construct the complete in one single class (WorldScene) without modifying any other classes or files or placing any vertices manually, which alone could cause you to write hundreds of lines per object.

The application's navigation component has a number of methods that the user may use to change what is displayed on screen. The camera's typical motion employs the WASD keys for basic movement, Q and E for up and down, and the mouse to adjust the camera's view. These movements use the camera.h preset motions to move the camera around the scene.

Moreover, to provide some testing capability to the program, the user may utilize keys IJKL to alter the spotlight's position inside the scene's x and z planes, while U and O move it along the y axis. The light may be rotated across the area using the left and right alt keys, and the directional light from above can be switched on and off with the left and right brackets. Finally, the viewport may be changed using the O and P keys, and the forms can be viewed in wireframe mode with the left and right arrow keys.
