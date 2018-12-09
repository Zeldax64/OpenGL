#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
