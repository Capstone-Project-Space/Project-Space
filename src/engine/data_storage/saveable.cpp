#include "saveable.h"

Saveable::Saveable(const std::string& identifier) : identifier(identifier) {}

void Saveable::save(void* data) const { }
void Saveable::load(const void* data, size_t size) {}
size_t Saveable::getSize() { return 0; }

const std::string& Saveable::getIdentifier() const {
	return this->identifier;
}
