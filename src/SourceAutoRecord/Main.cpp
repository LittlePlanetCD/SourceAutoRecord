#include "Modules/Client.hpp"
#include "Modules/Console.hpp"
#include "Modules/Engine.hpp"
#include "Modules/InputSystem.hpp"
#include "Modules/Scheme.hpp"
#include "Modules/Server.hpp"
#include "Modules/Surface.hpp"
#include "Modules/Tier1.hpp"
#include "Modules/VGui.hpp"

#include "Features/Config.hpp"

#include "Cheats.hpp"
#include "Command.hpp"
#include "Commands.hpp"
#include "Game.hpp"
#include "Interfaces.hpp"
#include "Variable.hpp"

#ifdef _WIN32
unsigned __stdcall Main(void* args)
#else
int __attribute__((constructor)) Main()
#endif
{
    if (!Console::Init())
        return 1;

    Interfaces::Init();

    if (Game::IsSupported()) {
        if (Tier1::Init()) {

            Commands::Init();
            Cheats::Init();

            auto vars = Variable::RegisterAll();
            auto commands = Command::RegisterAll();
            Console::DevMsg("SAR: Registered %i ConVars and %i ConCommands!\n", vars, commands);

            Client::Hook();
            Engine::Hook();
            InputSystem::Hook();
            Scheme::Hook();
            Server::Hook();
            Surface::Hook();
            VGui::Hook();

            Config::Load();

            SAR::IsPlugin();

            Console::PrintActive("Loaded SourceAutoRecord, Version %s (by NeKz)\n", SAR_VERSION);
            return 0;
        } else {
            Console::Warning("SAR: Could not register any commands!\n");
        }
    } else {
        Console::Warning("SAR: Game not supported!\n");
    }

    Console::Warning("SAR: Failed to load SourceAutoRecord!\n");
    return 1;
}

void Cleanup()
{
    VGui::Unhook();
    Surface::Unhook();
    Server::Unhook();
    Scheme::Unhook();
    InputSystem::Unhook();
    Engine::Unhook();
    Client::Unhook();

    Cheats::Unload();

    Variable::UnregisterAll();
    Command::UnregisterAll();

    Tier1::Shutdown();

    Console::Print("SAR: Cya!\n");
}

#ifdef _WIN32
BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(module);
        CreateThread(0, 0, LPTHREAD_START_ROUTINE(Main), 0, 0, 0);
    }
    else if (reason == DLL_PROCESS_DETACH) {
        Cleanup();
    }
    return TRUE;
}
#else
int __attribute__((constructor)) Exit()
{
    Cleanup();
}
#endif