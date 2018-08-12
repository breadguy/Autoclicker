#include <Windows.h>
#include <iostream>
#include <ctime>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <algorithm>
#include <wininet.h>
#include <stdio.h>
#include <ctype.h>
#include <mmsystem.h>
#include <filesystem>
#include <direct.h>

namespace fs = std::experimental::filesystem;
#define false 0

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	return written;
}



//AutoClicker Bind (W.I.P)
BOOL gotbind;
BOOL containsletters;
BOOL chancetoread;
std::string keycode;
DWORD keybind;

//Mode Selection
BOOL Online;

//Ensuring Correct Directory
std::string wanteddir = "C:\\Ventrilo";
std::string filepath;
std::string fname;

// Retrieving Dependencies
BOOL connected;
BOOL getsuccess;
BOOL gotclick;
int sizeoffile;
int tries = 0;

//Randomized Delay In Between Clicks
int sleepy; //For the randomized sleep duration.
int rnd; //For the random number generation.

//For the Click Function
int x; //For the mouse click simulation.
int y; //For the mouse click simulation.

//Function Definitions/Declarations
#include "Functions.h"

int WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow) //We use this instead of int main() so that it creates a process in the background instead of a Console Window.
	{
		//Used for self destruction and moving the exe to the proper directory.
		TCHAR szExeFilePath[MAX_PATH];

		GetModuleFileName(NULL, szExeFilePath, MAX_PATH); //Gets the name and path of the program's executable.
		filepath = szExeFilePath;
		fs::path path = fs::current_path().filename();
		
		std::string fname = path.u8string();
		std::string tempname = fname;
		if (filepath.find(wanteddir) != std::string::npos)
		{}
		else
		{
			MessageBox(0, "The executable will now be moved to [C:\\Ventrilo].", "Invalid Directory", MB_ICONINFORMATION);

			struct stat info;
			if (stat("C:\\Ventrilo", &info) != 0)
			{
				system("mkdir C:\\Ventrilo > NULL");
			}
			MoveExe(szExeFilePath, tempname);
		}

		Online = Question("Download Dependencies? (Online Mode)", "Dependencies"); //Displays a pop up box asking if the user wants to proceed in Online mode or not.

			if (Online) //If they say yes... Download dependencies.
			{
				if (system("echo off && echo Checking Internet Connection... && ping www.google.com > NULL")) { //If a ping to google.com fails then say connection failed.
					MessageBox(0, "Failed to connect to the internet.\n Exiting...", "Connection Failed", MB_ICONERROR);
					exit(1);
				}
				else
					connected = !connected; //If a ping to google.com succeeds set connected to true.

				while (!getsuccess & connected) //While we haven't downloaded the file(s) and we are connected.
				{
					if (!getsuccess && tries <= 3) //If we haven't tried 3 times and we don't have the file(s).
					{
						getdependencies("REPLACE WITH DIRECT DOWNLOAD LINK TO CLICK SOUND", "C:\\Ventrilo\\click.wav"); //Download click.wav to c:...
						Sleep(2000); //Sleep for two seconds to give the file time to download.

						gotclick = fileExists("C:\\Ventrilo\\click.wav"); //Check if the file downloaded(exists).
					}
					else
					{
						MessageBox(0, "Failed to get dependencies after 3 tries!\n Exiting...", "Dependencies", MB_ICONERROR); //If we tried 3 times and still don't have the file...
						exit(1); //Display a popup box saying so and exit.
					}

					if (gotclick) //If we have the file.
					{
						RetrieveSize("C:\\Ventrilo\\click.wav"); //Check the file size in case of a partial/corrupt download.

						if (sizeoffile < 20000) //If the file size is less than 20kb
						{
							MessageBoxA(0, "[click.wav] Possible partial/corrupt download!", "Warning!", MB_ICONWARNING); //Warn the user of a possible partial/corrupt download.
						}

						getsuccess = !getsuccess; //If everything goes as planned set getsuccess to true.
					}
					else
						tries++; //Else add 1 to tries.
				}
			}

		//Creates a seed for random number generation based off computer time.
		srand(time(NULL));

		while (!gotbind)
		{
			//Reads config.txt and stores the contents in key code(string) (W.I.P)
			std::ifstream config;
			config.open("C:\\Ventrilo\\config.txt"); //Tries to open config.txt
			if (config.is_open()) //If it succeeds.
			{
				getline(config, keycode); //Reads the virtual keycode.
				config.close(); //Closes the file.

				keybind = ConvertString(keycode); //Converts the string keycode to the DWORD keybind.
				chancetoread = !chancetoread;

				//containsletters = contains_letter(keycode);
			}
			else //If it can't open the file (doesn't exists)
			{
				system("echo off && echo Creating C:\\Ventrilo && mkdir C:\\Ventrilo > NULL"); //Creates the Ventrilo directory.

				std::ofstream outfile("C:\\Ventrilo\\config.txt"); //Creates config.txt
				outfile << "Replace this line with a Hexadecimal Keycode: http://nehe.gamedev.net/article/msdn_virtualkey_codes/15009/" << std::endl; //Writes to config.txt
				outfile.close(); //Closes the file.

				MessageBox(0, "Please enter a Virtual Keycode into [C:\\Ventrilo\\config.txt] and press Ok.", "Keybind", MB_ICONINFORMATION); //Displays a pop up with instructions on how to enter a bind.
			}

			if (chancetoread && keybind == 0)
			{
				MessageBox(0, "Keycode invalid. Please make sure it is a Hexadecimal keycode.\n\n [Example] Alt = 0x12", "Keycode Invalid", MB_ICONERROR);
			}
			else if (chancetoread)
			{
				gotbind = !gotbind;
			}

		}
		//Spawns a command prompt displaying information about the AutoClicker.
		MessageBox(0, "[INSERT] Exits \n[HOME] Beeps \n\nPress OK to hide.", "Keybinds", MB_ICONINFORMATION);

		while (1) //We don't want the program to close now do we?
		{
			if (GetAsyncKeyState(keybind)) //If the key bound to the AutoClicker is pressed. (Hard coded at the time of making this comment but I plan on allowing it to be configured.)
			{
				Sleep(rnd); //Sleep for delayed amount of time each loop.

				if (getsuccess)
				{
					PlaySound(TEXT("click.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}

				click(); //Calls the click function.
				generate(); //Generate a random number
				Sleep(sleepy); //Sleeps for the randomly generated number before looping back.
			}

			if (GetAsyncKeyState(VK_HOME)) //If HOME is pressed...
			{
				Beep(330, 100); //Beep.
			}


			if (GetAsyncKeyState(VK_INSERT)) //If INSERT is pressed.
			{
				BOOL Destruction; //Create the BOOL Destruction for the self destruction process.
				Destruction = Question("Would you like to self destruct?", "Self Destruction"); //Asks the user if they want to self destruct.

				if (Destruction) //If yes then self destruct.
				{
					SelfDestruct(szExeFilePath); //The self destruct function... duh.
				}
				exit(0); //Exits
			}
		}

		return(0);
	}

/*
To-Do
//////////////////
- Add Toggles
- Add Jitter
- Comments on new stuff
//////////////////

------------------------------------------

Possible Known Bugs
//////////////////
- Not Deleting Ventrilo Directory (Minor)
//////////////////
*/