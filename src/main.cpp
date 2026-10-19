#include "Plugin.h"
#include "Data_Source.h"
#include "Module_Data.h"
#include "Named_List.h"
#include <fstream>
#include <iomanip>
#include <sstream>

// Bridges Appeal audio commands and exported entry points to a dynamically loaded plugin.

    Plugin Module;
    bool Module_Debug = false;

    // Backend functions resolved by Init_Globals; initialize before dispatching commands.
    void (*Back)();
    void (*Debug)(bool Enable);
    void (*Load_Play_List)(std::string Play_List);
    void (*Load_SFX)(std::string Name, std::string SFX_File);
    void (*Load_Song)(std::string Name, std::string Music_File);
    void (*Mute)();
    void (*Next)();
    void (*Play)();
    void (*Play_SFX)();
    void (*Play_SFX_Name)(std::string Name);
    void (*Play_Song)(std::string Name);
    void (*Play_Track)(int Track);
    void (*Set_Paths)(std::string Resources);
    void (*Set_Playlist_Path)(std::string Path);
    void (*Stop)();
    void (*UnMute)();
    void (*Update)();
    void (*Set_Volume)(int Volume);
    int (*Get_Volume)();
    void (*Set_Volume_Percent)(int Percent);
    int (*Get_Volume_Percent)();
    void (*Set_Current_Loop)(int Loop);
    bool (*Current_Looping)();
    void (*Auto_Play_Next)(int Auto_Play);
    bool (*Auto_Play_Next_Status)();
    std::string (*Current_Song)();
    bool (*Is_Muted)();
    bool (*Is_Playing)();

    // Borrowed interpreter globals; nullptr disables publication of Audio.* values.
    Named_List<Module_Data> *Globals = nullptr;

    // Insert or replace a typed, string-encoded value in the Audio namespace.
    void Set_Global(std::string Name, std::string Type, std::string Value)
    {
        if(Globals == nullptr) return;

        std::string Full_Name = "Audio." + Name;
        Module_Data Data;
        Data.Module_Name = "Audio";
        Data.Name = Name;
        Data.Type = Type;
        Data.Value = Value;

        if(Globals->Find(Full_Name))
        {
            Globals->Current->Value = Data;
        }
        else
        {
            Globals->Add_Node(Full_Name, Data);
        }
    }

    // Seed script-visible defaults without changing the backend's playback state.
    void Set_Default_Audio_Globals()
    {
        Set_Global("Current_Song", "string", "");
        Set_Global("Volume", "int", "100");
        Set_Global("Volume_Percent", "int", "100");
        Set_Global("Current_Looping", "bool", "0");
        Set_Global("Auto_Play_Next_Status", "bool", "0");
        Set_Global("Muted", "bool", "0");
        Set_Global("Is_Playing", "bool", "0");
    }

    // Override globals from optional Name Type Value records, supporting quoted values.
    // The configuration path is relative to the process working directory.
    void Load_Default_Audio_Config()
    {
        std::ifstream File("Resources/Modules/Appeal_Module_Audio/Default_Audio_Config.Appeal");
        if(!File.is_open()) return;

        std::string Line;
        while(std::getline(File, Line))
        {
            std::istringstream Stream(Line);
            std::string Name;
            std::string Type;
            std::string Value;

            if(!(Stream >> Name)) continue;
            if(Name == "//" || Name[0] == '#') continue;
            if(!(Stream >> Type)) continue;

            Stream >> std::ws;
            if(Stream.peek() == '"')
            {
                Stream >> std::quoted(Value);
            }
            else
            {
                Stream >> Value;
            }

            Set_Global(Name, Type, Value);
        }
    }

    // Sample available backend getters into globals; Is_Playing is handled separately.
    void Update_Audio_Globals()
    {
        if(Current_Song != nullptr) Set_Global("Current_Song", "string", Current_Song());
        if(Get_Volume != nullptr) Set_Global("Volume", "int", std::to_string(Get_Volume()));
        if(Get_Volume_Percent != nullptr) Set_Global("Volume_Percent", "int", std::to_string(Get_Volume_Percent()));
        if(Current_Looping != nullptr) Set_Global("Current_Looping", "bool", Current_Looping() ? "1" : "0");
        if(Auto_Play_Next_Status != nullptr) Set_Global("Auto_Play_Next_Status", "bool", Auto_Play_Next_Status() ? "1" : "0");
        if(Is_Muted != nullptr) Set_Global("Muted", "bool", Is_Muted() ? "1" : "0");
    }

 // Bind backend exports and populate the host's shared globals with configured defaults.
 extern "C" void Init_Globals(std::string Name, Named_List<Module_Data> *Shared_Globals)
    {
        Globals = Shared_Globals;
        Module.Load(Name);
        Module.Assign("Back", Back);
        Module.Assign("Debug", Debug);
        Module.Assign("Load_Play_List", Load_Play_List);
        Module.Assign("Load_SFX", Load_SFX);
        Module.Assign("Load_Song", Load_Song);
        Module.Assign("Mute", Mute);
        Module.Assign("Next", Next);
        Module.Assign("Play", Play);
        Module.Assign("Play_SFX", Play_SFX);
        Module.Assign("Play_SFX_Name", Play_SFX_Name);
        Module.Assign("Play_Song", Play_Song);
        Module.Assign("Play_Track", Play_Track);             
        Module.Assign("Set_Paths", Set_Paths);
        Module.Assign("Set_Playlist_Path", Set_Playlist_Path);
        Module.Assign("Stop", Stop);
        Module.Assign("UnMute", UnMute);
        Module.Assign("Update", Update);
        Module.Assign("Set_Volume", Set_Volume);
        Module.Assign("Get_Volume", Get_Volume);
        Module.Assign("Set_Volume_Percent", Set_Volume_Percent);
        Module.Assign("Get_Volume_Percent", Get_Volume_Percent);
        Module.Assign("Set_Current_Loop", Set_Current_Loop);
        Module.Assign("Current_Looping", Current_Looping);
        Module.Assign("Auto_Play_Next", Auto_Play_Next);
        Module.Assign("Auto_Play_Next_Status", Auto_Play_Next_Status);
        Module.Assign("Current_Song", Current_Song);
        Module.Assign("Is_Muted", Is_Muted);
        Module.Assign("Is_Playing", Is_Playing);

        Set_Default_Audio_Globals();
        Load_Default_Audio_Config();
    }

 // Initialize without a shared global table for callers using the basic entry point.
 extern "C" void Init(std::string Name)
    {
        Init_Globals(Name, nullptr);
    }

// Unmangled exports forward directly to the backend without refreshing shared globals.
extern "C" void Audio_Back() { Back(); }
extern "C" void Audio_Next() { Next(); }
extern "C" void Audio_Play() { Play(); }
extern "C" void Audio_Stop() { Stop(); }
extern "C" void Audio_Mute() { Mute(); }
extern "C" void Audio_UnMute() { UnMute(); }
extern "C" void Audio_Update() { Update(); }
extern "C" void Audio_Set_Volume(int Volume) { Set_Volume(Volume); }
extern "C" int Audio_Get_Volume() { return Get_Volume(); }
extern "C" void Audio_Set_Volume_Percent(int Percent) { Set_Volume_Percent(Percent); }
extern "C" int Audio_Get_Volume_Percent() { return Get_Volume_Percent(); }
extern "C" void Audio_Set_Current_Loop(int Loop) { Set_Current_Loop(Loop); }
extern "C" bool Audio_Current_Looping() { return Current_Looping(); }
extern "C" void Audio_Auto_Play_Next(int Auto_Play) { Auto_Play_Next(Auto_Play); }
extern "C" bool Audio_Auto_Play_Next_Status() { return Auto_Play_Next_Status(); }
extern "C" void Audio_Set_Playlist_Path(std::string Path) { Set_Playlist_Path(Path); }
extern "C" std::string Audio_Current_Song() { return Current_Song(); }
extern "C" bool Audio_Is_Muted() { return Is_Muted(); }
extern "C" bool Audio_Is_Playing() { return Is_Playing(); }

// Release the backend and detach the borrowed global table; it remains host-owned.
extern "C" void Shutdown()
{
    Module.Unload();
    Globals = nullptr;
}

// Consume one command and its arguments from the interpreter's data source.
// Playback and setting branches below reseed globals from defaults and configuration.
extern "C" void Interpreter(Data_Source *Data)
    {
        std::string Command;
        *Data >> Command;

        if(Command == "Init")
        {
            std::string Name;
            *Data >> Name;
            Init(Name);
        }
        else if(Command == "Debug")
        {
            std::string Value;
            *Data >> Value;
            if(Value == "1" || Value == "On" || Value == "on" || Value == "True" || Value == "true")
            {
                Module_Debug = true;
            }
            else if(Value == "0" || Value == "Off" || Value == "off" || Value == "False" || Value == "false")
            {
                Module_Debug = false;
            }
            if(Debug != nullptr) Debug(Module_Debug);
        }

        else if(Command == "Back")
        {
            Back();
            Set_Default_Audio_Globals();
        Load_Default_Audio_Config();
        }

        else if(Command == "Load_Play_List")
        {
            *Data >> Command;
            Load_Play_List(Command);
            Set_Default_Audio_Globals();
        Load_Default_Audio_Config();
        }

        else if(Command == "Load_SFX")
        {
            std::string Name;
            std::string SFX_File;
            *Data >> Name;
            *Data >> SFX_File;
            Load_SFX(Name, SFX_File);
        }

        else if(Command == "Load_Song")
        {
            std::string Name;
            std::string Music_File;
            *Data >> Name;
            *Data >> Music_File;
            Load_Song(Name, Music_File);
            Set_Default_Audio_Globals();
        Load_Default_Audio_Config();
        }

        else if(Command == "Mute")
        {
            Mute();
            Set_Default_Audio_Globals();
        Load_Default_Audio_Config();
        }

        else if(Command == "Next")
        {
            Next();
            Set_Default_Audio_Globals();
        Load_Default_Audio_Config();
        }

        else if(Command == "Play")
        {
            Play();
            Set_Default_Audio_Globals();
        Load_Default_Audio_Config();
        }

        else if(Command == "Play_SFX")
        {
            Play_SFX();
        }

        else if(Command == "Play_SFX_Name")
        {
            std::string Name;
            *Data >> Name;
            Play_SFX_Name(Name);
        }

        else if(Command == "Play_Song")
        {
            std::string Name;
            *Data >> Name;
            Play_Song(Name);
            Set_Default_Audio_Globals();
        Load_Default_Audio_Config();
        }

        else if(Command == "Play_Track")
        {
            int Track;
            *Data >> Track;
            Play_Track(Track);
            Set_Default_Audio_Globals();
        Load_Default_Audio_Config();
        }

        else if(Command == "Set_Paths")
        {
            std::string Resources;
            *Data >> Resources;
            Set_Paths(Resources);
        }

        else if(Command == "Set_Playlist_Path")
        {
            std::string Path;
            *Data >> Path;
            Set_Playlist_Path(Path);
        }

        else if(Command == "Stop")
        {
            Stop();
            Set_Default_Audio_Globals();
        Load_Default_Audio_Config();
        }

        else if(Command == "UnMute")
        {
            UnMute();
            Set_Default_Audio_Globals();
        Load_Default_Audio_Config();
        }
        else if(Command == "Update")
        {
            Update();
            Set_Default_Audio_Globals();
        Load_Default_Audio_Config();
        }
        else if(Command == "Toggle_Mute")
        {
            if(Is_Muted != nullptr && Is_Muted())
            {
                UnMute();
            }
            else
            {
                Mute();
            }
            Set_Default_Audio_Globals();
        Load_Default_Audio_Config();
        }
        else if(Command == "Set_Volume")
        {
            int Volume;
            *Data >> Volume;
            Set_Volume(Volume);
            Set_Default_Audio_Globals();
        Load_Default_Audio_Config();
        }
        else if(Command == "Set_Volume_Percent")
        {
            int Percent;
            *Data >> Percent;
            Set_Volume_Percent(Percent);
            Set_Default_Audio_Globals();
        Load_Default_Audio_Config();
        }
        else if(Command == "Set_Current_Loop")
        {
            int Loop;
            *Data >> Loop;
            Set_Current_Loop(Loop);
            Set_Default_Audio_Globals();
        Load_Default_Audio_Config();
        }
        else if(Command == "Auto_Play_Next")
        {
            int Auto_Play;
            *Data >> Auto_Play;
            Auto_Play_Next(Auto_Play);
            Set_Default_Audio_Globals();
        Load_Default_Audio_Config();
        }
        else if(Command == "Get_Volume")
        {
            if(Module_Debug) std::cout << Get_Volume() << std::endl;
        }
        else if(Command == "Get_Volume_Percent")
        {
            if(Module_Debug) std::cout << Get_Volume_Percent() << std::endl;
        }
        else if(Command == "Current_Looping")
        {
            if(Module_Debug) std::cout << Current_Looping() << std::endl;
        }
        else if(Command == "Auto_Play_Next_Status")
        {
            if(Module_Debug) std::cout << Auto_Play_Next_Status() << std::endl;
        }
        else if(Command == "Current_Song")
        {
            std::string Song = Current_Song != nullptr ? Current_Song() : "";
            Set_Global("Current_Song", "string", Song);
            Data->Return_Value(Song);
            if(Module_Debug) std::cout << Song << std::endl;
        }
        else if(Command == "Is_Muted")
        {
            if(Module_Debug) std::cout << Is_Muted() << std::endl;
        }
        else if(Command == "Is_Playing")
        {
            // Publish both the shared global and the result used by nested expressions.
            bool Playing = Is_Playing != nullptr && Is_Playing();
            Set_Global("Is_Playing", "bool", Playing ? "1" : "0");
            Data->Return_Value(Playing ? "1" : "0");
        }

        else
        {
          if(Module_Debug) std::cout << "\tError: "  + Command + " not found in Audio Dictionary" << std::endl;
        } 
    }   
