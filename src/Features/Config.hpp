#pragma once
#include "Feature.hpp"

#include <string>

#define SAVE_CVAR(cvar, value) \
    file << #cvar " " << cvar.Get##value() << "\n";

class Config : public Feature {
public:
    std::string filePath;

public:
    Config();
    bool Save();
    bool Load();
};

extern Config* config;
