#include <src/engine/data_storage/filestorage.h>
#include <src/engine/graphics/ui.h>

#include <stack>

class GameState : FileStorage {
public:
	std::shared_ptr<GameState> CurrentState;
	std::stack<std::shared_ptr<GameState>> StateStack;

	GameState() = default;

	virtual void update(float delta) = 0;
	virtual void render(float delta) = 0;

private:
	std::vector<UI> uis;
	void renderUIs(float delta);
};

namespace State {
	void ChangeState(std::shared_ptr<GameState> state);
	void RestoreState();
	void ResetStateTo(std::shared_ptr<GameState> state);

	void Draw(float delta);
};
