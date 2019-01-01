#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#define STB_IMAGE_IMPLEMENTATION
#//include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <unordered_map>

//OVERALL - Focus on doing things the right time the first time I type it

// Left off
// - Refactor my different shader / vao
// - can I pass multiple different vbos into a vao?? i.e. use different data sets
// - are particles 3D? I mean, their positioning is based on a 3D object
//      - yeah but, the thing I render to the screen? hmmm, in LoL I think
//      even if the camera moves / changes angles... man, I dunno
//      it looks like in this case the guy creates meshes?https://80.lv/articles/vfx-for-games-explained/
// check out: https://polycount.com/discussion/187518/game-effects-vfx-tutorials

//#include <learnopengl/shader_m.h>
#include "MeshManager.h"
#include "Profiler.h"
#include "Shader.h"
#include "TextureManager.h"
#include "Particle.h"
#include "ParticleManager.h"

#include <iostream>
#include <chrono>
#include <algorithm>
#include <vector>

#include <windows.h> // for page size

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


//LEFT OFF
// TODO - what is my overall goal? A particle system? An RTS engine?
//  - I think I start with a particle system, and build on that later....
//  - well, a rendering system (load in an arbitrary set of verts, and be able to spawn objects of that thing)
// TODO - lots of improve here, like how we lookup items in the unordered_map
// TODO - document
// TODO - understand the memory footprint a bit more
// TODO - how do we scale to more complicated objects? multiple meshes per object?
// TODO - next step, clean up Model stuff. Can I combine? Should I?




int main()
{
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    std::cout << "Memory page size on this system: " << si.dwPageSize << std::endl;
    //InitializeSubsystems(si.dwPageSize);
    Profiler profiler;

    std::cout << "Size of glm::mat4: " << sizeof(glm::mat4) << std::endl;
    std::cout << "Size of glm::vec3: " << sizeof(glm::vec3) << std::endl;
    std::cout << "Size of std::chrono::system_clock::time_point: " << sizeof(std::chrono::system_clock::time_point) << std::endl;
    std::cout << "Size of ptr: " << sizeof(void*) << std::endl;

    int sizeOfParticle = sizeof(glm::mat4) + sizeof(glm::vec3) + sizeof(std::chrono::system_clock::time_point) + sizeof(void*);
    std::cout << "Total size of particle: " << sizeOfParticle << std::endl;
    std::cout << "Particles per page: " << si.dwPageSize / sizeOfParticle << std::endl;

    return 1;

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

                                                         // glfw window creation
                                                         // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("Shaders/6.3.coordinate_systems.vs", "Shaders/6.3.coordinate_systems.fs");
    Shader triShader("Shaders/2.1.hello_triangle.vs", "Shaders/2.1.hello_triangle.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // (x,y,z,u,v) (position and texture coordinates)
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    // My debug orange triangle
    float vertices2[] = {
        -0.5f, -0.5f, 0.f,
        0.5f, -0.5f, 0.f,
        0.5f,  0.5f, 0.f
    };

    // My debug orange triangle with texture
    float vertices3[] = {
        -0.5f, -0.5f, 0.f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.f, 1.0f, 0.0f,
        0.5f,  0.5f, 0.f, 1.0f, 1.0f
    };

    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    ParticleManager::Create();
    ParticleManager::Instance().LoadParticleDefinitions();

    MeshManager meshMgr;
    meshMgr.CreateMesh("cubes", vertices, sizeof(vertices), true);
    //meshMgr.CreateMesh("tris", vertices2, sizeof(vertices2), false);
    meshMgr.CreateMesh("tris", vertices3, sizeof(vertices3), true);

    //TextureManager textureMgr;
    TextureManager::Create();
    const char* t1FilePath = "C:\\Users\\jbrock\\Github\\Practice\\Debug\\container.jpg";
    const char* t2FilePath = "C:\\Users\\jbrock\\Github\\Practice\\Debug\\awesomeface.png";
    unsigned int texture1 = TextureManager::Instance().LoadFile(t1FilePath, false);
    unsigned int texture2 = TextureManager::Instance().LoadFile(t2FilePath, true);

    
    //LEFT OFF HERE (11/30) - ok, I think I understand how we load a texture, bind that to a texture unit (a texture location),
    //bind a texture sampler (the shader variable) to that texture unit.
    //So basically I understand the entire pipe of "load this texture file" to using that texture in a shader
    //TODO - refactor, and simplify texture code
    // (DONE) extract loading of texture to a class?
    // I need to do the bindings (active/bind texture unit, and shbader.setInt) once I think...outside of the render loop.
    //  ---> NVM, the "bind texture sampler to texture unit" only needs to happen once in the program
    //  ---> BUT the "activate texture unit / bind texture unit to a texture happens right before draw call
    //  ---> which makes sense, we setup what textures are in each texture unit before drawing
    //  ---> (and then probably switching them for the next shader / draw call)
 
        
    // might need some thought around what this looks like for multiple shaders
    // ALSO TODO - clean up my notes around this. LOOK FOR "KEY EXPLANATION / KEY DEFITIONS"

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    // Configure shader uniform texture variables
    // Int param is which texture unit to bind to (i.e. GL_TEXTURE0, but not that enum value)
    ourShader.use(); // don't forget to activate/use the shader before setting uniforms! jbrock: can confirm, this messed me up
    ourShader.setInt("texture1", 0); //0 = Texture Unit 0
    ourShader.setInt("texture2", 1); //1 = Texture Unit 1
    
    triShader.use();
    triShader.setInt("texture1", 0);

    //TODO - need to understand the memory of texture units. I don't think it is simple though,
    // I need to understand the whole pipeline in general

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    long long totalFrames = 0;
    while (!glfwWindowShouldClose(window))
    {
        profiler.StartFrame();
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        

        //START DRAW OF BOXES

        // activate shader
        ourShader.use();

        // create transformations
        glm::mat4 view;
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        // pass transformation matrices to the shader
        ourShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame,
        //but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("view", view);

        // render boxes
        // all of my "particles" have the same mesh ATM, so I do this before the loop
        GLuint meshVAO = meshMgr.GetMesh("cubes");
        glBindVertexArray(meshVAO);

        //for (unsigned int i = 0; i < particles.size(); ++i)
        for (const auto& particle : ParticleManager::Instance().mParticles)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model;
            //model = glm::translate(model, cubePositions[i]);
            model = glm::translate(model, particle.mPosition);
            
            model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
            ourShader.setMat4("model", model);

            // bind textures on corresponding texture units
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, particle.definition->mTextures[0]);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, particle.definition->mTextures[1]);

            // uh, do this for each model? Can I do it after the loop? I need to understand this a bit more
            // can I draw all the ones with the same texture unit mappings at once?

            // https://www.khronos.org/opengl/wiki/Vertex_Rendering#Multi-Draw
            // Multi-draw is useful for circumstances where you know that you are going to draw a lot 
            // of separate primitives of the same kind that all use the same shader. 
            // Typically, this would be a single conceptual object that you would always 
            // draw together in the same way. You simply pack all of the vertex data into 
            // the same VAO and buffer objects, using the various offsets to pick and choose between them.

            // TODO - I'll figure that out later

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //END DRAW OF BOXES
        


        //START DRAW OF TRIANGLE

        // activate shader
        triShader.use();
        triShader.setMat4("projection", projection);
        triShader.setMat4("view", view);

        GLuint meshVAO2 = meshMgr.GetMesh("tris");
        glBindVertexArray(meshVAO2);

        for (const auto& particle : ParticleManager::Instance().mParticles)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model;
            //temp hack so I can always see the triangles on top of the cubes
            glm::vec3 temp(particle.mPosition);// .z += 1;
            temp.z += 1;
            model = glm::translate(model, temp);

            model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
            triShader.setMat4("model", model);

            // bind textures on corresponding texture units
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, particle.definition->mTextures[0]);
            //glActiveTexture(GL_TEXTURE1);
            //glBindTexture(GL_TEXTURE_2D, particle.mTextures[1]);

            // uh, do this for each model? Can I do it after the loop? I need to understand this a bit more
            // can I draw all the ones with the same texture unit mappings at once?

            // https://www.khronos.org/opengl/wiki/Vertex_Rendering#Multi-Draw
            // Multi-draw is useful for circumstances where you know that you are going to draw a lot 
            // of separate primitives of the same kind that all use the same shader. 
            // Typically, this would be a single conceptual object that you would always 
            // draw together in the same way. You simply pack all of the vertex data into 
            // the same VAO and buffer objects, using the various offsets to pick and choose between them.

            // TODO - I'll figure that out later

            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
        
        ParticleManager::Instance().Update();
     
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, texture1);

        //glDrawArrays(GL_TRIANGLES, 0, 3);

        
        //END DRAW OF TRIANGLE


        // LEFT OFF (12/2) TODO - improve reporting. Track number of slow frames, track part of frame timings,
        // so like, how much time is in the draw call for exampleS
        // Also, figure out a better way to print an interval rather than every 10k frames
        // After profiling, lets try to clean up code and start building a simple authoring pipeline for particles!
        // (read from file, how do divide things into our object model so things are done efficiently)
        // (minimize VAOs, draw calls, etc...)

        // LEFT OFF (12/8)
        // I got json reading in there, started particle manager. TODO - convert over boxes so they are using it
        // too. And actually create some useful json files for them. Lots of cleanup around that stuff

        // LEFT OFF (12/9)
        // Flush out the Particle class more? Particles of different type (i.e. vertices/shapes, VAOs, etc...)
        // How do I store them efficiently so I can loop through and draw all the ones of the same type
        // together?

        profiler.EndFrame();
        ++totalFrames;
        if (totalFrames > 10000)
        {
            std::cout << "Running avg of past 100 frames (ns): " << profiler.GetRunningAvg() << std::endl;
            std::cout << "Running avg of past 100 frames (ms): " << profiler.GetRunningAvg2() << std::endl;
            totalFrames = 0;
        }
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //std::cout << "Total Frames: " << totalFrames << std::endl;

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    meshMgr.DeleteMeshes();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}