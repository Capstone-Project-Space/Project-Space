#include <stdio.h>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <gl/GL.h>
#include <stb_image.h>
#include <glfw3/glfw3.h>

#define TESTING

#if defined(TESTING)

#include "./testing/tests.h"

#define SIZE_DUMP(type) fprintf(stderr, "sizeof(%s): %llu\n", #type, sizeof(type))

int main(int argc, char** args) {
	Test();
	return 0;
}
#else



int main(int argc, char** args) {
	printf("Hello World!\n");
	return 0;
}

#endif