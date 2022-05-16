#include "test_gamestate.h"

#include <src/engine/states/game_state.h>
#include <src/engine/io/event.h>

Key GS_Last_Key_Pressed, GS_Last_Key_Released, GS_Last_Key_Repeated;
MouseButton GS_Last_MouseButton_Pressed, GS_Last_MouseButton_Released;
float GS_Mouse_X, GS_Mouse_Y;

class TestState : GameState {
public:

	const std::string n;

	TestState(const std::string& name) : GameState(nullptr, name), n(name) {}
	
	virtual void render(float delta) { }

	virtual void update(float delta) { }

	virtual bool onKeyPressed(const Key& key) { 
		GS_Last_Key_Pressed = key;
		return true;
	}

	virtual bool onKeyReleased(const Key& key) {
		GS_Last_Key_Released = key;
		return true;
	}
	
	virtual bool onKeyRepeated(const Key& key) {
		GS_Last_Key_Repeated = key;
		return true;
	}

	virtual bool onMouseButtonPressed(const MouseButton& button) {
		GS_Last_MouseButton_Pressed = button;
		return true;
	}

	virtual bool onMouseButtonReleased(const MouseButton& button) {
		GS_Last_MouseButton_Released = button;
		return true;
	}

	virtual bool onMouseMoved(const float x, const float y, const float dx, const float dy) {
		GS_Mouse_X = x;
		GS_Mouse_Y = y;
		return true;
	}
};

TestResult* GS_TestStateChanges() {
	TestResult* result = new TestResult{"Test State Changes"};
	std::shared_ptr<GameState> state1 = GameState::CreateState<TestState>("test1");
	std::shared_ptr<GameState> state2 = GameState::CreateState<TestState>("test2");
	std::shared_ptr<GameState> state3 = GameState::CreateState<TestState>("test4");

	if (State::CurrentState != nullptr) {
		result->msg = "Initial Current State is not nullptr.";
		return result;
	}
	State::ChangeState(state1);
	if (State::CurrentState != state1) {
		result->msg = "Changing to state1 did not succeed.";
		return result;
	}
	State::ChangeState(state2);
	if (State::CurrentState != state2) {
		result->msg = "Changing to state2 did not succeed.";
		return result;
	}

	State::RestoreState();
	if (State::CurrentState != state1) {
		result->msg = "Restoring the state did not return to state1.";
		return result;
	}

	State::ResetStateTo(state3);
	if (State::CurrentState != state3) {
		result->msg = "Resetting the state stack to state4 did not work.";
		return result;
	}

	State::RestoreState();
	if (State::CurrentState != nullptr) {
		result->msg = "After a reset bottom state doesn't receive nullptr.";
		return result;
	}

	result->succeeded = true;
	State::ResetStateTo(nullptr);
	return result;
}

TestResult* GS_TestEventReceiving() {
	TestResult* result = new TestResult{"Test State Receives Events"};
	std::shared_ptr<GameState> state = GameState::CreateState<TestState>("test");
	State::ChangeState(state);

	Events::DispatchEvent(Event<Key>{ EventType::KEY_PRESSED, { GLFW_KEY_F, 0 } });
	if (GS_Last_Key_Pressed.keyCode != GLFW_KEY_F) {
		result->msg = "Key Pressed event did not dispatch";
		return result;
	}

	Events::DispatchEvent(Event<Key>{ EventType::KEY_PRESSED, { GLFW_KEY_G, 0 } });
	Events::DispatchEvent(Event<Key>{ EventType::KEY_PRESSED, { GLFW_KEY_H, 0 } });
	if (GS_Last_Key_Pressed.keyCode != GLFW_KEY_H) {
		result->msg = "Double Key Pressed didn't update correctly";
		return result;
	}

	Events::DispatchEvent(Event<Key>{ EventType::KEY_RELEASED, { GLFW_KEY_F, 0 } });
	if (GS_Last_Key_Released.keyCode != GLFW_KEY_F) {
		result->msg = "Key Released event did not dispatch";
		return result;
	}

	Events::DispatchEvent(Event<Key>{ EventType::KEY_RELEASED, { GLFW_KEY_G, } });
	Events::DispatchEvent(Event<Key>{ EventType::KEY_RELEASED, { GLFW_KEY_H, 0 } });
	if (GS_Last_Key_Released.keyCode != GLFW_KEY_H) {
		result->msg = "Double Key Release didn't update correctly";
		return result;
	}

	Events::DispatchEvent(Event<Key>{ EventType::KEY_REPEATED, { GLFW_KEY_F, 0 } });
	if (GS_Last_Key_Repeated.keyCode != GLFW_KEY_F) {
		result->msg = "Key Repeated event did not dispatch";
		return result;
	}

	Events::DispatchEvent(Event<Key>{ EventType::KEY_REPEATED, { GLFW_KEY_G, 0 } });
	Events::DispatchEvent(Event<Key>{ EventType::KEY_REPEATED, { GLFW_KEY_H, 0 } });
	if (GS_Last_Key_Repeated.keyCode != GLFW_KEY_H) {
		result->msg = "Double Key Repeat didn't update correctly";
		return result;
	}

	Events::DispatchEvent(Event<MouseButton>{ EventType::MOUSE_BUTTON_PRESSED, GLFW_MOUSE_BUTTON_1 });
	if (GS_Last_MouseButton_Released != GLFW_MOUSE_BUTTON_1) {
		result->msg = "Mouse Button Pressed event did not dispatch";
		return result;
	}

	Events::DispatchEvent(Event<MouseButton>{ EventType::MOUSE_BUTTON_PRESSED, GLFW_MOUSE_BUTTON_2 });
	Events::DispatchEvent(Event<MouseButton>{ EventType::MOUSE_BUTTON_PRESSED, GLFW_MOUSE_BUTTON_3 });
	if (GS_Last_MouseButton_Pressed != GLFW_MOUSE_BUTTON_3) {
		result->msg = "Double Mouse Button Press didn't update correctly";
		return result;
	}

	Events::DispatchEvent(Event<MouseButton>{ EventType::MOUSE_BUTTON_RELEASED, GLFW_MOUSE_BUTTON_1 });
	if (GS_Last_MouseButton_Released != GLFW_MOUSE_BUTTON_1) {
		result->msg = "Mouse Button Released event did not dispatch";
		return result;
	}

	Events::DispatchEvent(Event<MouseButton>{ EventType::MOUSE_BUTTON_RELEASED, GLFW_MOUSE_BUTTON_2 });
	Events::DispatchEvent(Event<MouseButton>{ EventType::MOUSE_BUTTON_RELEASED, GLFW_MOUSE_BUTTON_3 });
	if (GS_Last_MouseButton_Released != GLFW_MOUSE_BUTTON_3) {
		result->msg = "Double Mouse Button Release didn't update correctly";
		return result;
	}

	float pos[4] = { .2345f, .453f, 0.0f, 0.0f };
	Events::DispatchEvent(Event<float*>{EventType::MOUSE_POSITION, pos});
	if (GS_Mouse_X != .2345f) {
		result->msg = "Mouse Position Event did not update mouse x";
		return result;
	}
	else if (GS_Mouse_Y != .453f) {
		result->msg = "Mouse Position Event did not update mouse y";
		return result;
	}

	result->succeeded = true;
	State::ResetStateTo(nullptr);
	return result;
}
