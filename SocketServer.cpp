#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <stdio.h>

using namespace std;

int main()
{
    //Create Socket
    int slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //bind IP&Port
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(7000);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);

    if (::bind(slisten, (struct sockaddr* )&sin, sizeof(sin)) == -1)
    {
        printf("bind error !");
        exit(1);
    }

    //Start Listening
    if (listen(slisten,20) == -1)
    {
        cout << "Listening Error !" << endl;
        return 0;
    }

    int sClient;
    sockaddr_in remoteAddr;
    socklen_t naddrlen = sizeof(remoteAddr);
    
    while (true)
    {  
        //Wait Connection
        cout << "Wait Connection !" << endl;
        sClient = accept(slisten, (struct sockaddr* )&remoteAddr, &naddrlen);
        if (sClient < 0)
        {
            cout << "Accept Error !" << endl;
            continue;
        }
        else
        {
            char addr[20] = { 0, };
            inet_ntop(AF_INET, (void*)&remoteAddr.sin_addr, addr, 16);
            cout << "Connection Succeed :" << inet_ntoa(remoteAddr.sin_addr) << endl;
            break;
        }
    }

    while (true)
    {
        cout << "Receive Message" << endl;
        char rcvData[255] = { 0, };
        int ret = recv(sClient, rcvData, 255, 0);
        if (ret>0)
        {
            rcvData[ret] = 0x00;
            cout << rcvData << endl;
        }
        const char* SendData = "Hello, This is TCP Server !";
        send(sClient, SendData, strlen(SendData), 0);
    }

    close(sClient);
    close(slisten);
    return 0;
}