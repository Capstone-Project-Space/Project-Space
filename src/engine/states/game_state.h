#pragma once
#include <src/engine/data_storage/filestorage.h>
#include <src/engine/graphics/ui.h>
#include <src/engine/io/event_receiver.h>

#include <stack>

class GameState : public FileStorage, public EventReceiver {
public:
	GameState(const std::string& saveFileName);
	virtual ~GameState();

	virtual void update(float delta) = 0;
	virtual void render(float delta) = 0;

	template<typename T, typename... Args>
	static std::shared_ptr<GameState> CreateState(Args&&... args) {
		T* t = new T(std::forward<Args>(args)...);
		return std::shared_ptr<GameState>(reinterpret_cast<GameState*>(t));
	}

private:
	std::vector<UI> uis;
	void renderUIs(float delta);
};

struct State {
	static std::shared_ptr<GameState> CurrentState;
	static std::stack<std::shared_ptr<GameState>> StateStack;

	static void ChangeState(std::shared_ptr<GameState> state);
	static void RestoreState();
	static void ResetStateTo(std::shared_ptr<GameState> state);

	static void Draw(float delta);
};
