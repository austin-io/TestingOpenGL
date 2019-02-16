/*
 * Learning OpenGL 
 * Experimental program following TheCherno tutorial series
 * */

// Precompiled headers for faster compilation
#include "include/pch.h"

#define DEBUG

// OpenGL Rendering abstraction
#include "src/Renderer.hpp"
#include "src/VertexBuffer.hpp"
#include "src/IndexBuffer.hpp"

struct ShaderSources{
    std::string vertexShader;
    std::string fragShader;
};

enum ShaderType {
    NONE = -1, VERTEX = 0, FRAGMENT = 1
};

// Function called when event happens
void keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods);

static ShaderSources parseShader(const std::string filepath);
static unsigned int compileShader(const std::string& source, unsigned int type);
static unsigned int createShader(const std::string& vertexShader, const std::string& fragShader);

int main(){

    // Main window provided by GLFW (not created yet)
    GLFWwindow* win;

    // Checks if GLFW was initialized
    if(!glfwInit()) exit(-1);
    
    // Actually create the window (width, height, title, resizable, idk)
    win = glfwCreateWindow(640, 480, "It works!", NULL, NULL);
    if(!win){
        glfwTerminate();
        std::exit(-1);
    }

    // Select the window
    glfwMakeContextCurrent(win);
    
    // Match the framerate to the monitor refresh rate
    glfwSwapInterval(1);
    
    // Remeber to initialize glew AFTER selecting the context
    if(glewInit() != GLEW_OK){
        std::cout << "Error!\n";
        std::exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLCALL(std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;);

    // Vertices temp data
    float positions[8] = {
        -0.5, -0.5,
         0.5, -0.5,
         0.5,  0.5,
        -0.5,  0.5
    };
    
    // Index buffer positions
    unsigned int indices[6] = {
        0, 1, 2,
        2, 3, 0
    };

    /* Vertex array object
     * Links to the currently bound buffer when you call glVertexAttribPointer
     * Allows you to simply bind the vao and ibo without binding everything else all the time
     **/
    unsigned int vao;
    GLCALL(glGenVertexArrays(1, &vao));
    GLCALL(glBindVertexArray(vao));

    // Buffer is the ID for the data
    VertexBuffer vb(positions, 4 * 2 * sizeof(float));

    // TESTING!
    /*//////////////////////////////////////////////////////
    unsigned int buffer;
    // Create a new buffer
    GLCALL(glGenBuffers(1, &buffer));
    // Bind means select the buffer
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, buffer)); 
    // Populate the buffer with data (enum_type, size, pointer, draw_type)
    GLCALL(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW));
    //////////////////////////////////////////////////////*/
    
    /* Attributes are the different sections of your data
     * i.e. positions, colors, normals
     * Select the index for the Attribute
     **/
    GLCALL(glEnableVertexAttribArray(0)); 
    
    /* Specify how this Attribute should be interpreted
     * Create an index that can be selected,
     * Size is the dimension of vertices i.e Vec2 or Vec3 (only allows up to 4)
     * Type is the data type of each vertex
     * Normalized makes sure the value is between -1 and 1
     * Stride size of each vertex
     * Pointer is where this index starts
     * (index, size, type, normalized, stride, pointer)
     **/
    GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)0));
    
    // Generate and populate Index Buffer
    // ibo = Index Buffer Object
    IndexBuffer ib(indices, 3 * 2);

    // TESTING
    /*/////////////////////////////////////////////////////
    unsigned int ibo;
    // Create a new buffer
    GLCALL(glGenBuffers(1, &ibo));
    // Bind means select the buffer
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo)); 
    // Populate the buffer with data (enum_type, size, pointer, draw_type)
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));
    /////////////////////////////////////////////////////*/
    
    ShaderSources sources = parseShader("res/shaders/base.shader");
    // Returns the program id for both shaders
    unsigned int shaderProgram = createShader(sources.vertexShader, sources.fragShader);

    // Select the shader (bind)
    GLCALL(glUseProgram(shaderProgram));
    
    // Link to the color variable in our shaders
    GLCALL(int location = glGetUniformLocation(shaderProgram, "u_Color"));
    ASSERT(location != -1);

    // Update the shader variable
    GLCALL(glUniform4f(location, 0.3f, 0.5f, 1.0f, 1.0f));

    float colorValue = 0.05f;
    float i = 0.f;

    // Function keyCallback is called when input is detected
    glfwSetKeyCallback(win, keyCallback); 

    while(!glfwWindowShouldClose(win)){

        float ratio;
        int width, height;

        glfwGetFramebufferSize(win, &width, &height);
        ratio = width/(float)height;

        // How much of the window is seen
        GLCALL(glViewport(0, 0, width, height));
        
        // Clear screen to black
        GLCALL(glClear(GL_COLOR_BUFFER_BIT));

        // Select our index buffer
       // ib.Bind();

        // Update color uniform
        GLCALL(glUniform4f(location, colorValue, 0.5f, 0.5f, 1.0f));
        
        // Update color value
        colorValue = std::abs(std::sin(i+=0.01));

        // Draw the currently bound buffer
        // Draw elements is used for drawing with indices instead of raw positions
        // nullptr is used instead of ibo because its already bound
        GLCALL(glDrawElements(GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, nullptr));
        
        glfwSwapBuffers(win);
        glfwPollEvents();
    
    }

    // Clean up 
    GLCALL(glDeleteProgram(shaderProgram));
    glfwDestroyWindow(win);
    glfwTerminate();
}

void keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_Q && action == GLFW_PRESS){
        std::cout << "Close Program\n";
        glfwSetWindowShouldClose(win, GLFW_TRUE);
    }
}

unsigned int compileShader(const std::string& source, unsigned int type){
    
    GLCALL(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    
    // Select the shader address and compile
    GLCALL(glShaderSource(id, 1, &src, nullptr));
    GLCALL(glCompileShader(id));
    
    // Error handling
    int result;
    GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    
    // Something went wrong
    if(result == GL_FALSE){
        int length;
        GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        
        // message must be a const length
        // alloca() dynamically allocates this memory on the stack
        char* message = (char*)alloca(length * sizeof(char));
        GLCALL(glGetShaderInfoLog(id, length, &length, message));

        std::cout << "Failed to compile shader!\n" << message << std::endl;
        
        // Delete shader because it failed
        GLCALL(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int createShader(const std::string& vertexShader, const std::string& fragShader){
    
    // id of the current program
    GLCALL(unsigned int program = glCreateProgram());
    
    // vs and fs hold the id to their shaders
    unsigned int vs = compileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fs = compileShader(fragShader, GL_FRAGMENT_SHADER);

    // Give the shaders to our program
    GLCALL(glAttachShader(program, vs));
    GLCALL(glAttachShader(program, fs));
    
    // Make sure our program doesn't have any problems
    GLCALL(glLinkProgram(program));
    GLCALL(glValidateProgram(program));

    // Remove shaders after saving the program
    GLCALL(glDeleteShader(vs));
    GLCALL(glDeleteShader(fs));

    return program;

}

ShaderSources parseShader(const std::string filepath){

    std::ifstream inFile(filepath);
    if(!inFile) std::cout << "Failed to open '" << filepath << "'\n"; 

    std::string line;
    std::stringstream ss[2];
    
    ShaderType type = ShaderType::NONE;

    while(getline(inFile, line)){
        if(line.find("#shader") != std::string::npos){
            if(line.find("vertex") != std::string::npos){
                type = ShaderType::VERTEX;
            } else if(line.find("fragment") != std::string::npos){
                type = ShaderType::FRAGMENT;
            }
        } else {
            ss[(int)type] << line << std::endl;
        }
    }

    inFile.close();

    return { ss[ShaderType::VERTEX].str(), ss[ShaderType::FRAGMENT].str() };

}
