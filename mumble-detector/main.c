#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
//#include <WinSock2.h>
#include <iphlpapi.h>

#include "mtr.h"

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")


int main(int argc, char *argv[])
{
    
    if (argc >= 1)
        return -1;

    const char* SERVER_IP = argv[1];

    PMIB_TCPTABLE table; 
    DWORD dwSize = 0;
    DWORD result = 0;
    char szLocalAddr[128];
    char szRemoteAddr[128];
    struct in_addr IpAddr;
    
    const HWND mtr = FindWindow(NULL, MTR_WINDOW_NAME);

    while (TRUE)
    {
        table = (MIB_TCPTABLE *)malloc(sizeof(MIB_TCPTABLE));

        if (table == NULL)
        {
            printf("error allocating memory");
            return 1;
        }

        dwSize = sizeof(MIB_TCPTABLE);

        result = GetTcpTable(table, &dwSize, TRUE);

        if (result == ERROR_INSUFFICIENT_BUFFER)
        {
            free(table);
            table = (MIB_TCPTABLE *)malloc(dwSize);
        }

        result = GetTcpTable(table, &dwSize, TRUE);

        if (result == NO_ERROR)
        {
            BOOL connected = FALSE;
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
            if (!connected)
                printf("Disconnected...\n");
        }
    }

    free(table);
    
    system("pause");
    return 0;
}