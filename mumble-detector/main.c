#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
//#include <WinSock2.h>
#include <iphlpapi.h>
//#include <rpcdce.h>

#include "mtr.h"

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Rpcrt4.lib")

ULONG getTable(PMIB_TCPTABLE);
void saveData();


ULONG getTable(PMIB_TCPTABLE *table)
{
    DWORD dwSize = 0;
    DWORD result = 0;

    *table = (MIB_TCPTABLE *)malloc(sizeof(MIB_TCPTABLE));

    if (*table == NULL)
    {
        printf("error allocating memory");
        return 1;
    }

    dwSize = sizeof(MIB_TCPTABLE);

    result = GetTcpTable(*table, &dwSize, TRUE);

    if (result == ERROR_INSUFFICIENT_BUFFER)
    {
        free(*table);
        *table = (MIB_TCPTABLE *)malloc(dwSize);
    }

    result = GetTcpTable(*table, &dwSize, TRUE);
    return result;
}

void saveData()
{
    PROCESS_INFORMATION pInfo = startProcess(L"WinMTR.exe");
    const HWND mtr = getHandle();
    Sleep(60000);
    exportText(mtr);
    Sleep(1000);
    const HWND saveAs = FindWindow(NULL, L"Save As");
    printf("%d\n", saveAs);

    wchar_t *path[1000];
    DWORD result = GetEnvironmentVariable(L"UserProfile", path, 200);
    printf("%ld\n", result);
    lstrcat(path, L"\\desktop\\");
    printf("%ls\n", path);

    UUID uuid;
    memset(&uuid, 0, sizeof(UUID));
    UuidCreate(&uuid);
    wchar_t guid[80];
    wsprintfW(guid, L"%d", uuid.Data1);

    lstrcat(path, guid);
    lstrcat(path, L".txt");

    SAVEASCONTROLS controls;
    enumSaveAsChildren(saveAs, &controls);
    saveFile(path, controls);
    Sleep(3000);
    closeProcess(&pInfo);
}

int main(int argc, char *argv[])
{
    const char* SERVER_IP = "66.206.19.222";
    char szLocalAddr[128];
    char szRemoteAddr[128];
    struct in_addr IpAddr;
    BOOL connected = TRUE;

    while (connected)
    {
        connected = FALSE;
        MIB_TCPTABLE *table = NULL;

        DWORD result = getTable(&table);

        if (result == NO_ERROR)
        {
            for (unsigned int i = 0; i < table->dwNumEntries; i++)
            {
                IpAddr.S_un.S_addr = (u_long)table->table[i].dwRemoteAddr;
                strcpy_s(szRemoteAddr, sizeof(szRemoteAddr), inet_ntoa(IpAddr));
                if (strcmp(szRemoteAddr, SERVER_IP) == 0)
                {
                    if (table->table[i].dwState == MIB_TCP_STATE_ESTAB)
                        connected = TRUE;
                }
            }
        }
        free(table);
        Sleep(1000);
    }
    saveData();

    system("pause");
    return 0;
}