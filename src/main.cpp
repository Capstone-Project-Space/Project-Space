#include <stdio.h>

#include <glm/glm.hpp>
#include <stb_image.h>

#include <src/engine/graphics/model.h>

#define TESTING

#if defined(TESTING)

#include <src/testing/tests.h>

#define SIZE_DUMP(type) fprintf(stderr, "sizeof(%s): %llu\n", #type, sizeof(type))

int main(int argc, char** args) {
	Test();

	std::shared_ptr<Model> model = Model::CreateModel("./resources/models/capsule.obj");

	return 0;
}
#else

int main(int argc, char** args) {
	printf("Hello World!\n");
	return 0;
}

#endif
