#pragma once
#include "Features/Feature.hpp"

#include "JumpStats.hpp"
#include "StepStats.hpp"
#include "VelocityStats.hpp"

class Stats : public Feature {
public:
    JumpStats* jumps;
    StepStats* steps;
    VelocityStats* velocity;

public:
    Stats();
    ~Stats();
    void ResetAll();
};

extern Stats* stats;
