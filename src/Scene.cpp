#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Scene.hpp"

void Scene::print(const char* filename) { stbi_write_png(filename, width, height, 3, color_buffer.data(), width * 3); }