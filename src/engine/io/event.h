#pragma once

#include <vector>
#include <type_traits>

#include "event_receiver.h"
#include <src/engine/states/game_state.h>

enum class EventType {
	KEY_PRESSED,
	KEY_RELEASED,
	KEY_REPEATED,
	MOUSE_BUTTON_PRESSED,
	MOUSE_BUTTON_RELEASED,
	MOUSE_DOUBLE_CLICK,
	MOUSE_WHEEL_SCROLL,
	MOUSE_POSITION,
	WINDOW_RESIZE
};

template<typename T>
struct Event {
	
	const EventType type;
	T eventData = {0};
	Event(EventType type, T eventData) : type(type) {
		if constexpr(std::is_pointer<T>::value || std::is_array<T>::value) {
			memcpy(this->eventData, eventData, sizeof(T));
		} else {
			this->eventData = eventData;
		}
	}
};

struct Events {
	static std::vector<EventReceiver*> Receivers;

	/**
	 * @brief Dispatches an Event to the State::CurrentState and then to any registered listeners until something returns true saying the event has been handled.
	 * 
	 * @tparam T The data stored within the type of event.
	 * @param e The event which has occured.
	 */
	template<typename T>
	static void DispatchEvent(const Event<T>& e) {
		float* vals;
		switch (e.type) {
		case EventType::KEY_PRESSED:
			if (State::CurrentState->onKeyPressed((const Key)e.eventData)) return;
			break;
		case EventType::KEY_RELEASED:
			if (State::CurrentState->onKeyReleased((const Key)e.eventData)) return;
			break;
		case EventType::KEY_REPEATED:
			if (State::CurrentState->onKeyRepeated((const Key)e.eventData)) return;
			break;
		case EventType::MOUSE_BUTTON_PRESSED:
			if (State::CurrentState->onMouseButtonPressed((const MouseButton)e.eventData)) return;
			break;
		case EventType::MOUSE_BUTTON_RELEASED:
			if (State::CurrentState->onMouseButtonReleased((const MouseButton)e.eventData)) return;
			break;
		case EventType::MOUSE_DOUBLE_CLICK:
			if (State::CurrentState->onMouseDoubleClick((const MouseButton) e.eventData)) return;
			break;
		case EventType::MOUSE_WHEEL_SCROLL:
			vals = (float*)(e.eventData);
			if (State::CurrentState->onMouseWheelScroll(vals[0], vals[1])) return;
			break;
		case EventType::MOUSE_POSITION:
			vals = (float*)(e.eventData);
			if (State::CurrentState->onMouseMoved(vals[0], vals[1], vals[2], vals[3])) return;
			break;
		case EventType::WINDOW_RESIZE:
			vals = (float*)(e.eventData);
			State::CurrentState->onWindowResize(vals[0], vals[1], vals[2], vals[3]);
			break;
		}


		auto it = Receivers.crbegin();
		for (; it != Receivers.crend(); it++) {
			const auto recv = *it;
			switch (e.type) {
			case EventType::KEY_PRESSED:
				if (recv->onKeyPressed((const Key&)e.eventData)) return;
				break;
			case EventType::KEY_RELEASED:
				if (recv->onKeyReleased((const Key&)e.eventData)) return;
				break;
			case EventType::KEY_REPEATED:
				if (recv->onKeyRepeated((const Key&)e.eventData)) return;
				break;
			case EventType::MOUSE_BUTTON_PRESSED:
				if (recv->onMouseButtonPressed((const MouseButton)e.eventData)) return;
				break;
			case EventType::MOUSE_BUTTON_RELEASED:
				if (recv->onMouseButtonReleased((const MouseButton)e.eventData)) return;
				break;
			case EventType::MOUSE_DOUBLE_CLICK:
				if (recv->onMouseDoubleClick((const MouseButton)e.eventData)) return;
				break;
			case EventType::MOUSE_WHEEL_SCROLL:
				vals = (float*)(e.eventData);
				if (recv->onMouseWheelScroll(vals[0], vals[1])) return;
				break;
			case EventType::MOUSE_POSITION:
				vals = (float*)(e.eventData);
				if (recv->onMouseMoved(vals[0], vals[1], vals[2], vals[3])) return;
				break;
			case EventType::WINDOW_RESIZE:
				vals = (float*)(e.eventData);
				recv->onWindowResize(vals[0], vals[1], vals[2], vals[3]);
				break;
			}
		}
	}

	/**
	 * @brief Register an EventReceiver to receive events.
	 * 
	 * @param recv The EventReceiver to register.
	 */
	static void RegisterReceiver(EventReceiver* recv);

	/**
	 * @brief Un-register an EventReceiver to no longer receive events.
	 * 
	 * @param recv The EventReceiver to un-register.
	 */
	static void RemoveReceiver(EventReceiver* recv);
};
