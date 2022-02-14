#include "event.h"

void Events::RegisterReceiver(EventReceiver* recv) {
	Events::Receivers.push_back(recv);
}

void Events::RemoveReceiver(EventReceiver* recv) {
	auto _ = std::remove_if(
		Events::Receivers.begin(), Events::Receivers.end(),
		[recv](const auto& v) { return v == recv; }
	);
}
