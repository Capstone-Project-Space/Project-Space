struct MouseButton {
	int buttonCode;
};

struct Mouse {
	bool buttonStates[8] = { false };
	float x = 0.0, y = 0.0;
	inline bool isButtonDown(int buttonCode) { return buttonStates[buttonCode]; }
};