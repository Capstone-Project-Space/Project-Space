#pragma once

#include <vector>
#include <type_traits>

#include "event_receiver.h"
#include <src/engine/states/game_state.h>

enum class EventType {
	KEY_PRESSED,
	KEY_RELEASED,
	KEY_REPEATED,
	KEY_CHARACTER,
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
		if constexpr(std::is_array<T>::value) {
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
	static void DispatchEvent(const Event<T>& e);

	template<>
	static void DispatchEvent(const Event<char*>& e) {
		switch (e.type) {
			case EventType::KEY_CHARACTER:
				if (State::CurrentState->onKeyChar(e.eventData)) return;
				break;
			default:
				throw "Unknown EventType with char* value.";
		}
	}

	template<>
	static void DispatchEvent(const Event<float*>& e) {
		switch (e.type) {
			case EventType::MOUSE_WHEEL_SCROLL:
				if (State::CurrentState->onMouseWheelScroll(e.eventData[0], e.eventData[1])) return;
				break;
			case EventType::MOUSE_POSITION:
				if (State::CurrentState->onMouseMoved(e.eventData[0], e.eventData[1], e.eventData[2], e.eventData[3])) return;
				break;
			case EventType::WINDOW_RESIZE:
				State::CurrentState->onWindowResize(e.eventData[0], e.eventData[1], e.eventData[2], e.eventData[3]);
				break;
			default:
				throw "Unknown EventType with float* value.";
		}

		for (const auto& recv : Receivers) {
			switch (e.type) {
				case EventType::MOUSE_WHEEL_SCROLL:
					if (recv->onMouseWheelScroll(e.eventData[0], e.eventData[1])) return;
					break;
				case EventType::MOUSE_POSITION:
					if (recv->onMouseMoved(e.eventData[0], e.eventData[1], e.eventData[2], e.eventData[3])) return;
					break;
				case EventType::WINDOW_RESIZE:
					recv->onWindowResize(e.eventData[0], e.eventData[1], e.eventData[2], e.eventData[3]);
					break;
			}
		}
	}

	template<>
	static void DispatchEvent(const Event<Key>& e) {
		switch (e.type) {
			case EventType::KEY_PRESSED:
				if (State::CurrentState->onKeyPressed(*((const Key*)&e.eventData))) return;
				break;
			case EventType::KEY_RELEASED:
				if (State::CurrentState->onKeyReleased(*((const Key*)&e.eventData))) return;
				break;
			case EventType::KEY_REPEATED:
				if (State::CurrentState->onKeyRepeated(*((const Key*)&e.eventData))) return;
				break;
			default:
				throw "Unknown EventType with Key value.";
		}

		for (const auto& recv : Receivers) {
			switch (e.type) {
				case EventType::KEY_PRESSED:
					if (recv->onKeyPressed(*((const Key*)&e.eventData))) return;
					break;
				case EventType::KEY_RELEASED:
					if (recv->onKeyReleased(*((const Key*)&e.eventData))) return;
					break;
				case EventType::KEY_REPEATED:
					if (recv->onKeyRepeated(*((const Key*)&e.eventData))) return;
					break;
			}
		}
	}

	template<>
	static void DispatchEvent(const Event<MouseButton>& e) {
		switch (e.type) {
			case EventType::MOUSE_BUTTON_PRESSED:
				if (State::CurrentState->onMouseButtonPressed(*((const MouseButton*)&e.eventData))) return;
				break;
			case EventType::MOUSE_BUTTON_RELEASED:
				if (State::CurrentState->onMouseButtonReleased(*((const MouseButton*)&e.eventData))) return;
				break;
			case EventType::MOUSE_DOUBLE_CLICK:
				if (State::CurrentState->onMouseDoubleClick(*((const MouseButton*)&e.eventData))) return;
				break;
			default:
				throw "Unknown EventType with MouseButton value.";
		}

		for (const auto& recv : Receivers) {
			switch (e.type) {
				case EventType::MOUSE_BUTTON_PRESSED:
					if (recv->onMouseButtonPressed(*((const MouseButton*)&e.eventData))) return;
					break;
				case EventType::MOUSE_BUTTON_RELEASED:
					if (recv->onMouseButtonReleased(*((const MouseButton*)&e.eventData))) return;
					break;
				case EventType::MOUSE_DOUBLE_CLICK:
					if (recv->onMouseDoubleClick(*((const MouseButton*)&e.eventData))) return;
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
