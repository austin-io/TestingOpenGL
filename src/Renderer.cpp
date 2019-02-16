#include <pch.h>

#include "Renderer.hpp"

void GLClearErrors(){
    // Remove all previous error messages if any
    while(GLenum error = glGetError());
}

bool GLLogCall(const char* functionName, const char* filepath, int lineNumber){
    // Output error messages if any and their location
    while(GLenum error = glGetError()){
        std::cout << "[OpenGL Error] - (" << error << ") '" 
                  << functionName 
                  << "' In file: '" << filepath 
                  << "' line:" << lineNumber << std::endl;
        return false;
    }
    return true;
}