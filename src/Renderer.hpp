#pragma once

// Creates a line break if an error is found
#define ASSERT(x) if(!(x)) assert(x);

// GLCALL wraps a function between our error handling functions
#if defined(DEBUG)
    #define GLCALL(x) GLClearErrors(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
    #define GLCALL(x) 
#endif

void GLClearErrors();
bool GLLogCall(const char*, const char*, int);