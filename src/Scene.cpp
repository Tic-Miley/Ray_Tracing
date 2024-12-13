#define STB_IMAGE_WRITE_IMPLEMENTATION
#include  "Scene.hpp"

void Scene::print(){stbi_write_png("../image/output.png", width, height, 3, color_buffer.data(), width * 3);}