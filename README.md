<h1>Blockzone <img src="Docs/Images/logo-jb.png" width="auto" height="40" style="float: right" /></h1>

----
This is an open world voxel game I am developing using C++ and OpenGL.

<img src="Docs/Images/development_early_blocks.png" width="auto" height="200" />

I started out by learning how to work with OpenGL and was able to render some simple blocks.

<img src="Docs/Images/development_early_procedual_generation.png" width="auto" height="200" />

To make something out of these blocks I used a noise function to generate a height map to created simple procedual rolling hills.

<img src="Docs/Images/development_early_rendering_optimization.png" width="auto" height="200" />

My first approcah at rendering the blocks and chunks was very inefficient, there was serious lag with only 4 chunks! To fix this, I added a geometry shader in my rendering pipeline so I could pass in 1 vertex with a normal and it would generate the 2D plane on the graphics card. This reduced the amount of data I needed to pass to the GPU from 4 vertices each with 8 floats of data to 1 vertex. I also added a step to generate a mesh for each chunk which would only need to updated if the chunk was changed. These 2 steps made it so I could easily have 16 by 16 chunks with no lag.

### Dependiencies
- C++
- OpenGL 3.3
- GLFW (OpenGL Library)
- GLEW (OpenGL Library)
- SOIL2 (for loading textures)
- GLM (for advanced vector and matrix math)