#pragma once

#include <src/engine/data_storage/filestorage.h>
#include <src/engine/io/event_receiver.h>
#include <src/engine/graphics/window.h>
#include <src/engine/graphics/ui/component_manager.h>

#include <stack>

class GameState : public FileStorage, public EventReceiver {
public:
	GameState(const std::string& saveFileName);
	GameState(const std::shared_ptr<Window> window, const std::string& saveFileName);
	virtual ~GameState();

	virtual void onResume(std::shared_ptr<Window> window);

	virtual void update(float delta) = 0;
	virtual void render(float delta) = 0;

	virtual void onPause();

	void changeWindow(std::shared_ptr<Window> window);

	virtual bool onMouseButtonPressed(const MouseButton& button) override;
	virtual bool onMouseButtonReleased(const MouseButton& button) override;

	template<typename T, typename... Args>
	static std::shared_ptr<GameState> CreateState(Args&&... args) {
		T* t = new T(std::forward<Args>(args)...);
		return std::shared_ptr<GameState>(reinterpret_cast<GameState*>(t));
	}

protected:
	friend struct State;
	std::shared_ptr<Window> window;
	ComponentManager componentManager;
};

struct State {
	static std::shared_ptr<GameState> CurrentState;
	static std::stack<std::shared_ptr<GameState>> StateStack;

	static void ChangeState(std::shared_ptr<GameState> state);
	static void RestoreState();
	static void ResetStateTo(std::shared_ptr<GameState> state);
	static void Close();

	static void Draw(float delta);
};
