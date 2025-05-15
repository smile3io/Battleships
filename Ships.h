#pragma once

#include <vector>
#include <utility>

enum class Rotation{
    HOR = 0, VER = 1       // rotation
};

class Ships
{
public:
    int size;
    bool sunken;
    Rotation rotation;
    std::vector<std::pair<bool, std::pair<int, int>>> segments;

    Ships(int s, Rotation r, std::pair<int, int> c);

    // check if current ship is sunken  (checks segments and writes sunken var)
    bool isSunken();
    // checks if a shot is a hit on this ship and marks the segment as hit
    bool isHit(std::pair<int, int> cord);

    // void getSegments();
};