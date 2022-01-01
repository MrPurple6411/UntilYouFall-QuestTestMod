#include "main.hpp"

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup

MAKE_HOOK_FIND_CLASS_UNSAFE_INSTANCE(OnPlayerHit, "SG.Claymore.Combat.Blocking","PlayerDefense", "OnPlayerHit", Il2CppObject*, Il2CppObject* self, Il2CppObject* damageHandle)
{
    // intercepted hit and ignored it!"
    return nullptr;
}

MAKE_HOOK_FIND_CLASS_UNSAFE_INSTANCE(KillPlayer, "SG.Claymore.Entities","PlayerHealth", "KillPlayer", void, Il2CppObject* self)
{
    // intercepted KillPlayer and cancelled it!"
}

MAKE_HOOK_FIND_CLASS_UNSAFE_INSTANCE(HitResult, "SG.Claymore.NPCs","Enemy", "HandleHitCollision", void, Il2CppObject* self, Il2CppObject* hitData)
{
    // Added Kill command to every hit!
    il2cpp_utils::RunMethod(self, "Kill",false);
    HitResult(self, hitData);
}

// Loads the config from disk using our modInfo, then returns it for use
Configuration &getConfig()
{
    static Configuration config(modInfo);
    config.Load();
    return config;
}

// Returns a logger, useful for printing debug messages
Logger &getLogger()
{
    static Logger *logger = new Logger(modInfo);
    return *logger;
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo &info)
{
    info.id = ID;
    info.version = VERSION;
    modInfo = info;

    getConfig().Load(); // Load the config file
    getLogger().log(Logging::INFO,"Completed setup!");
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load()
{
    il2cpp_functions::Init();

    getLogger().log(Logging::INFO,"Installing hooks...");
    // Install our hooks (none defined yet)
    
    INSTALL_HOOK(getLogger(), OnPlayerHit)
    INSTALL_HOOK(getLogger(), KillPlayer)
    INSTALL_HOOK(getLogger(), HitResult)

    getLogger().log(Logging::INFO,"Installed all hooks!");
}