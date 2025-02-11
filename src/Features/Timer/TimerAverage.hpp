#pragma once
#include <vector>
#include <string>

struct TimerAverageItem {
    int ticks;
    float time;
    std::string map;
};

class TimerAverage {
public:
    bool isEnabled;
    std::vector<TimerAverageItem> items;
    int averageTicks;
    float averageTime;

public:
    TimerAverage();
    void Start();
    void Add(int ticks, float time, std::string map);
};
