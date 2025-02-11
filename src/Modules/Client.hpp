#pragma once
#include <cstdint>

#include "Module.hpp"

#include "Command.hpp"
#include "Interface.hpp"
#include "Utils.hpp"
#include "Variable.hpp"

class Client : public Module {
private:
    Interface* g_ClientDLL = nullptr;
    Interface* g_pClientMode = nullptr;
    Interface* g_pClientMode2 = nullptr;
    Interface* g_HUDChallengeStats = nullptr;
    Interface* g_HUDQuickInfo = nullptr;
    Interface* s_EntityList = nullptr;
    Interface* g_Input = nullptr;
    Interface* g_HudChat = nullptr;
    Interface* g_HudMultiplayerBasicInfo = nullptr;

public:
    using _GetClientEntity = void*(__rescall*)(void* thisptr, int entnum);
    using _KeyDown = int(__cdecl*)(void* b, const char* c);
    using _KeyUp = int(__cdecl*)(void* b, const char* c);
    using _GetAllClasses = ClientClass* (*)();
    using _ShouldDraw = bool(__rescall*)(void* thisptr);
    using _ChatPrintf = void(*)(void* thisptr, int iPlayerIndex, int iFilter, const char* fmt, ...);

    _GetClientEntity GetClientEntity = nullptr;
    _KeyDown KeyDown = nullptr;
    _KeyUp KeyUp = nullptr;
    _GetAllClasses GetAllClasses = nullptr;
    _ShouldDraw ShouldDraw = nullptr;
    _ChatPrintf ChatPrintf = nullptr;

    void* in_jump = nullptr;
    std::string lastLevelName;

public:
    DECL_M(GetAbsOrigin, Vector);
    DECL_M(GetAbsAngles, QAngle);
    DECL_M(GetLocalVelocity, Vector);
    DECL_M(GetViewOffset, Vector);

    void* GetPlayer(int index);
    void CalcButtonBits(int nSlot, int& bits, int in_button, int in_ignore, kbutton_t* button, bool reset);
    bool ShouldDrawCrosshair();
    void Chat(TextColor color, const char* fmt, ...);
    void QueueChat(TextColor color, const char* fmt, ...);
    void FlushChatQueue();
    float GetCMTimer();

public:
    // CHLClient::LevelInitPreEntity
    DECL_DETOUR(LevelInitPreEntity, const char *levelName);

    // ClientModeShared::CreateMove
    DECL_DETOUR(CreateMove, float flInputSampleTime, CUserCmd* cmd);
    DECL_DETOUR(CreateMove2, float flInputSampleTime, CUserCmd* cmd);

    // CHud::GetName
    DECL_DETOUR_T(const char*, GetName);

    // CHudMultiplayerBasicInfo::ShouldDraw
    DECL_DETOUR_T(bool, ShouldDraw_BasicInfo);

    // CHudChat::MsgFunc_SayText2
    DECL_DETOUR(MsgFunc_SayText2, bf_read &msg);

    // CInput::_DecodeUserCmdFromBuffer
    DECL_DETOUR(DecodeUserCmdFromBuffer, int nSlot, int buf, signed int sequence_number);

    // CInput::CreateMove
    DECL_DETOUR(CInput_CreateMove, int sequence_number, float input_sample_frametime, bool active);

    // CInput::GetButtonBits
    DECL_DETOUR(GetButtonBits, bool bResetState);

    // ClientModeShared::OverrideView
    DECL_DETOUR(OverrideView, CPortalViewSetup1* m_View);

    DECL_DETOUR_COMMAND(playvideo_end_level_transition);

    bool Init() override;
    void Shutdown() override;
    const char* Name() override { return MODULE("client"); }

private:
    std::vector<std::pair<TextColor, std::string>> chatQueue;
};

extern Client* client;

extern Variable cl_showpos;
extern Variable cl_sidespeed;
extern Variable cl_forwardspeed;
extern Variable in_forceuser;
extern Variable cl_fov;
