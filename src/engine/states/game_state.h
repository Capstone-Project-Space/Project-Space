#pragma once

#include <src/engine/data_storage/filestorage.h>
#include <src/engine/io/event_receiver.h>
#include <src/engine/graphics/window.h>
#include <src/engine/graphics/ui/component_manager.h>

#include <stack>

class GameState : public FileStorage, public EventReceiver {
public:
	//GameState(const std::string& saveFileName);
	
	/**
	 * @brief Construct a new Game State.
	 * 
	 * @param window The Window which the GameState will render to.
	 * @param saveFileName The name of the save file which this state will write it's persistent data to.
	 */
	GameState(const std::shared_ptr<Window> window, const std::string& saveFileName);
	virtual ~GameState();

	/**
	 * @brief This function is called when the GameState becomes State::CurrentState.
	 * 
	 * @param window The window the GameState will render to.
	 */
	virtual void onResume(std::shared_ptr<Window> window);

	/**
	 * @brief The function which will be called when the state should update it's internal state.
	 * 
	 * @param delta The amount of time since the last update.
	 */
	virtual void update(float delta) = 0;

	/**
	 * @brief The function which will be called when the state should render to the Window.
	 * 
	 * @param delta The amount of tmie since the last frame was rendered.
	 */
	virtual void render(float delta) = 0;

	/**
	 * @brief This function is called when the GameState is no longer State::CurrentState.
	 */
	virtual void onPause();

	/**
	 * @brief This will change the window this GameState will render to.
	 * 
	 * @param window The window to change this GameState to.
	 */
	void changeWindow(std::shared_ptr<Window> window);

	virtual bool onKeyPressed(const Key& key) override;
	virtual bool onKeyReleased(const Key& key) override;
	virtual bool onKeyRepeated(const Key& key) override;

	virtual bool onKeyChar(const char* text) override;

	virtual bool onMouseButtonPressed(const MouseButton& button) override;
	virtual bool onMouseButtonReleased(const MouseButton& button) override;
	virtual bool onMouseDoubleClick(const MouseButton& button) override;
	virtual bool onMouseWheelScroll(const float xoffset, const float yoffset) override;
	virtual bool onMouseMoved(float x, float y, float dx, float dy) override;
	virtual void onWindowResize(const float oldWidth, const float oldHeight, const float newWidth, const float newHeight) override;

	/**
	 * @brief Create a State and wrap it in a std::shared_ptr<GameState>
	 * 
	 * @tparam T The type of GameState to create.
	 * @tparam Args The arguments for the type T constructor.
	 * @param args The actual arguments for the type T constructor.
	 * @return A std::shared_ptr to a T(GameState).
	 */
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

	/**
	 * @brief Store the State::CurrentState into the StateStack then change State::CurrentState.
	 * 
	 * @param state The GameState to change State::CurrentState to.
	 */
	static void ChangeState(std::shared_ptr<GameState> state);
	
	/**
	 * @brief Pop the previous GameState off of the stack and make it the State::CurrentState. Returns the previous State::CurrentState so that it isn't immediately lost.
	 * 
	 * @return The previous current state.
	 */
	static std::shared_ptr<GameState> RestoreState();
	
	/**
	 * @brief Clears the StateStack and sets the State::CurrentState.
	 * 
	 * @param state The state to set State::CurrentState to.
	 */
	static void ResetStateTo(std::shared_ptr<GameState> state);

	/**
	 * @brief Clear the StateStack and CurrentState.
	 */
	static void Close();

	/**
	 * @brief Update then render the State::CurrentState.
	 * 
	 * @param delta The time since State::CurrentState was drawn.
	 */
	static void Draw(float delta);
};
