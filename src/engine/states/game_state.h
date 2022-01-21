#include <src/engine/data_storage/filestorage.h>
#include <src/engine/graphics/ui.h>

class GameState : FileStorage {
public:

	GameState(const std::string& name);

private:
	std::vector<UI> uis;
};