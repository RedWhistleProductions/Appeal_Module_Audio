#include <iostream>

bool Muted = false;

// Turn Debug On or Off
extern "C" void Debug(bool Enable)
{
	std::cout << "Debug " << Enable << std::endl;
}

// Go Back in the Playlist by 1 song
extern "C" void Back()
{
	std::cout << "Back" << std::endl;
}

// Loads a Playlist
extern "C" void Load_Play_List(std::string File_Name)
{
	std::cout << "Load_Play_List " << File_Name << std::endl;
}

// Load a Sound Effect
extern "C" void Load_SFX(std::string Name, std::string File_Name)
{
	std::cout << "Load_SFX " << Name << " " << File_Name << std::endl;
}

// Load a Song
extern "C" void Load_Song(std::string Name, std::string File_Name)
{
	std::cout << "Load_Song " << Name << " " << File_Name << std::endl;
}

// Mute the Audio
extern "C" void Mute()
{
	std::cout << "Mute" << std::endl;
	Muted = true;
}

// Play the Next Song in the Playlist
extern "C" void Next()
{
	std::cout << "Next" << std::endl;
}

// Play the Current Song in the Playlist
extern "C" void Play()
{
	std::cout << "Play" << std::endl;
}

// Play The Currently Selected Sound Effect
extern "C" void Play_SFX()
{
	std::cout << "Play_SFX" << std::endl;
}

// Select a Loaded Sound Effect by Name and play it
extern "C" void Play_SFX_Name(std::string Name)
{
	std::cout << "Play_SFX_Name " << Name << std::endl;
}

// Select a Song from a loaded Playlist and Play it
extern "C" void Play_Song(std::string Name)
{
	std::cout << "Play_Song " << Name << std::endl;
}

// Selects a Song in a loaded Playlist by its Track Number and Plays it
extern "C" void Play_Track(int Track_Number)
{
	std::cout << "Play_Track " << Track_Number << std::endl;
}

// Changes the Path to Audio Files
extern "C" void Set_Paths(std::string Path)
{
	std::cout << "Set_Paths " << Path << std::endl;
}

// Stop Playing the Current Song 
extern "C" void Stop()
{
	std::cout << "Stop" << std::endl;
}	

// Unmute the Audio
extern "C" void UnMute()
{
	std::cout << "Unmute" << std::endl;
	Muted = false;
}

// Toggle Mute On or Off
extern "C" void Toggle_Mute()
{
	std::cout << "Toggle_Mute" << std::endl;
	Muted = !Muted;
}

// Set Volume 0 to 100
extern "C" void Set_Volume(int Volume)
{
	std::cout << "Set_Volume " << Volume << std::endl;
}

// Set the Current Song to Loop or not Loop
extern "C" void Set_Current_Loop(int Loop)
{
	std::cout << "Set_Current_Loop " << Loop << std::endl;
}

// Play through the playlist or stop when this Song ends
extern "C" void Set_Auto_Play(int Auto_Play)
{
	std::cout << "Set_Auto_Play " << Auto_Play << std::endl;
}

// Returns the current Volume 0 to 100
extern "C" int Get_Volume()
{
	std::cout << "Get_Volume" << std::endl;
	return 0;
}

// Returns the Current Song's looping status
extern "C" bool Current_Looping()
{
	std::cout << "Current_Looping" << std::endl;
	return true;
}

// Returns the status of Auto_Play
extern "C" bool Auto_Playing()
{
	std::cout << "Auto_Playing" << std::endl;
	return true;
}

// Returns the Name of the Current Song in the playlist
extern "C" std::string Current_Song()
{
	std::cout << "Current_Song" << std::endl;
	return "";
}

// Returns if the status of Muted
extern "C" bool Is_Muted()
{
	std::cout << "Is_Muted" << std::endl;
	return true;
}
