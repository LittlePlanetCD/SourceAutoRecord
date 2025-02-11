#pragma once

#include <vector>
#include <string>
#include <map>
#include <set>

#include "Command.hpp"
#include "Rules.hpp"

struct SpeedrunCategory
{
    std::set<std::string> rules;
};

namespace SpeedrunTimer {
    void TestInputRules(std::string targetname, std::string classname, std::string inputname, std::string parameter, std::optional<int> triggerSlot);
    void TestZoneRules(Vector pos, int slot);
    void TestPortalRules(Vector pos, int slot, PortalColor portal);
    void TestFlagRules(int slot);
    void TestFlyRules(int slot);
    void TestLoadRules();
    void ResetCategory();
    void DrawTriggers();
    void InitCategories();
    SpeedrunRule *GetRule(std::string name);

    bool CreateCategory(std::string name);
    bool AddRuleToCategory(std::string category, std::string rule);
    bool CreateRule(std::string name, std::string type, std::map<std::string, std::string> params);
};

extern Command sar_speedrun_category;
