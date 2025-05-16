#include "Ships.h"

Ships::Ships(int s, Rotation r, std::pair<int, int> c) : size(s), rotation(r), sunken(false) {
	if (rotation == Rotation::HOR) {
		for (int i = 0; i < size; i++) {
			segments.emplace_back( false, std::make_pair( c.first, c.second + i ) );
		}
	}
	else
	{
		for (int i = 0; i < size; i++) {
			segments.emplace_back( false, std::make_pair( c.first + i, c.second ) );
		}
	}
}

bool Ships::isSunken() {
	for (const auto& segment : segments) {
		if (!segment.first) {
			return false;
		}
	}
	sunken = true;
	return true;
}

bool Ships::isHit(std::pair<int, int> cord) {
	for (auto& segment : segments) {
		if (segment.second == cord) {
			segment.first = true;
			return true;
		}
	}
	return false;
}
