// stuff for GLFW and glad  (glad is for loading openGL extensions. need to read more about that)
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h> 
#include <glad/glad.h>
#include <linmath.h>
 
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

// my header files



//graphics pipeline

//1. Vertex Data
//2. Vertex shader. takes vertices and their positions from vertex data and applies transformations
//3. Shape assembler takes all vertex positions and assembles them into a "primitive" -> a primitive is just a shape. a triangle requires three points. a line two. etc
//4. geometry shader
//5. Rasterizer. turns all the mathematical shapes into pixels
//6. Fragment shaders. Adds color to the pixels from the rasterizer
//7. Fragment Blending. Some pixels from the shader migh have multiple colors do to shader overlap. the fragment blender resolves this


//coordinate system is normalized x axis:[-1,1] y axis:[-1,1]


typedef struct Vertex
{
    vec2 pos;
    vec3 col;
} Vertex;
 
static const Vertex vertices[3] =
{
    { { -0.6f, -0.4f }, { 1.f, 0.f, 0.f } },
    { {  0.6f, -0.4f }, { 0.f, 1.f, 0.f } },
    { {   0.f,  0.6f }, { 0.f, 0.f, 1.f } }
};
 

//shaders are objects
static const char* vertex_shader_text =
"#version 330\n"
"uniform mat4 MVP;\n"
"in vec3 vCol;\n"
"in vec2 vPos;\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";
 
static const char* fragment_shader_text =
"#version 330\n"
"in vec3 color;\n"
"out vec4 fragment;\n"
"void main()\n"
"{\n"
"    fragment = vec4(color, 1.0);\n"
"}\n";
 
static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
 
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
 
int main(void)
{
    glfwSetErrorCallback(error_callback);
 
    if (!glfwInit())
        exit(EXIT_FAILURE);
 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);   //openGL profiles are like a package of functions 1.core  or 2.compatibility
 
    GLFWwindow* window = glfwCreateWindow(300, 300, "OpenGL Triangle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
 
    glfwSetKeyCallback(window, key_callback);  // WHAT DOES THIS DO?
 
    glfwMakeContextCurrent(window); // applies opengl context to the window we created
    gladLoadGL();
    glfwSwapInterval(1);
 
    // NOTE: OpenGL error checks have been omitted for brevity

    //creates a buffer with our vertex data    
    GLuint vertex_buffer;                                                       // vertex buffer is what gets passed to gpu for graphics pipeline renderring
    glGenBuffers(1, &vertex_buffer);                                            // creates the buffer object. 1 = becase we only have one 3d object. pouits to the GLuint reference
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);                               // binding makes a certain object the current object. This binds the GL_ARRAY_BUFFER to the vertex_buffer object? i think?
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // stores our vertices into the vertex buffer. specify type of buffer. size of buffer in bytes. the actual vertices. and the use of the data? 
 
    const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);    // this creates a vertex shader object
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);      // tells where the shader source code is. (its up higher in the text blocks)
    glCompileShader(vertex_shader);                                   // gpu doesn't understand how the shader works so shader source code needs to be compiled
 
    const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);
 
    const GLuint program = glCreateProgram();                        //this creates a shader program
    glAttachShader(program, vertex_shader);                          // if we want to use the shaders we made we have to attach the shaders to the shader program
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);                                          // this wraps up the shader program we created

    glDeleteShader(vertex_shader);                                   // we can delete the shaders we created now since they are part of the shader program
    glDeleteShader(fragment_shader);
 
    const GLint mvp_location = glGetUniformLocation(program, "MVP");  // these three functions specify a GLint location for each of the programs in the top of this file.
    const GLint vpos_location = glGetAttribLocation(program, "vPos");
    const GLint vcol_location = glGetAttribLocation(program, "vCol");
 
    GLuint vertex_array;                                            // creating a vertexArray object. a vertexArray points to multiple vertex buffer objects and tells openGL what to do with them. vertex arrays exist so we can quickly switch between vertex buffer objects
    glGenVertexArrays(1, &vertex_array);                            // creates vertex array. and only points to one object
    glBindVertexArray(vertex_array);                                
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void*) offsetof(Vertex, pos));
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,             // vertex attribute is a way of communitacting with a vertex shader from the outside.  
                          sizeof(Vertex), (void*) offsetof(Vertex, col));
 
    while (!glfwWindowShouldClose(window))  // while loop runs until window is closed or until window is closed by a function
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        const float ratio = width / (float) height;
 
        glViewport(0, 0, width, height);  // tells openGL what portion of the window to render into
        glClear(GL_COLOR_BUFFER_BIT);     // clears the back buffer im pretty sure.
 
        mat4x4 m, p, mvp;
        mat4x4_identity(m);
        printf("%f",(float)glfwGetTime());
        mat4x4_rotate_Y(m, m, (float) glfwGetTime());
        //mat4x4_rotate(m, m, 4.0,(float) glfwGetTime(),0.0,0.0);
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);
 
        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) &mvp);
        glBindVertexArray(vertex_array);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);        //swaps the back buffer with the front buffer
        glfwPollEvents();               //checks if events have occured
    }
 
    glfwDestroyWindow(window);
 
    glfwTerminate();
    exit(EXIT_SUCCESS);
}