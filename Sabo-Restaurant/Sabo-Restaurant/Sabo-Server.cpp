/*
*	Sabo's Restaurant
*	lang=KOR, ENG		create by Re-kim
*	The program used a TCP network.
*	Link: https://github.com/Re-kim/Sabo_Restaurant
*/
#pragma comment(lib, "ws2_32")
#include <winsock2.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <Windows.h>

#define SERVERPORT 9000  // My Server PORT
#define BUFSIZE 512      // Data maximum size

// Global variables
int A_dish; // Food(A)
int B_dish; // Food(B)
int total; // Foot price total

// Exit after print error on sock()
void err_quit(char *msg)  
{  
    LPVOID lpMsgBuf;  
    FormatMessage(   
        FORMAT_MESSAGE_ALLOCATE_BUFFER|  
        FORMAT_MESSAGE_FROM_SYSTEM,  
        NULL, WSAGetLastError(),  
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  
        (LPTSTR)&lpMsgBuf, 0, NULL);  
    MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);  
    LocalFree(lpMsgBuf);  
    exit(-1);  
}  
  
// Print error on sock() 
void err_display(char *msg)  
{  
    LPVOID lpMsgBuf;  
    FormatMessage(   
        FORMAT_MESSAGE_ALLOCATE_BUFFER|  
        FORMAT_MESSAGE_FROM_SYSTEM,  
        NULL, WSAGetLastError(),  
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  
        (LPTSTR)&lpMsgBuf, 0, NULL);  
    printf("[%s] %s", msg, (LPCTSTR)lpMsgBuf);  
    LocalFree(lpMsgBuf);  
}  

// Food(A) Function:
void jajangmyeon(int dish) {
	if (dish >= 1) {
		printf("[The waiter]: ¥���(A)�� %d �׸��� �ֹ��߽��ϴ�.\n",dish); // Success order
		A_dish += dish; // Add local variable to Global variable
	} else {
		printf("[The waiter]: �ֹ��� ����߽��ϴ�.\n"); // Cancel order
	}
} 

int main(int argc, char* argv[])  
{  
    int retval;  
  
    // Reset Winsock 
    WSADATA wsa;  
    if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)  
        return -1;  
  
    // socket()  
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);  
    if(listen_sock == INVALID_SOCKET) err_quit("socket()");   
      
    // bind()  
    SOCKADDR_IN serveraddr;  
    ZeroMemory(&serveraddr, sizeof(serveraddr));  
    serveraddr.sin_family = AF_INET;  
    serveraddr.sin_port = htons(SERVERPORT);  
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);  
    retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));  
    if(retval == SOCKET_ERROR) err_quit("bind()");  
      
    // listen()  
    retval = listen(listen_sock, SOMAXCONN);  
    if(retval == SOCKET_ERROR) err_quit("listen()");  
  
    // for data 
    SOCKET client_sock;  
    SOCKADDR_IN clientaddr;  
    int addrlen;  
    char buf[BUFSIZE+1];  
  
    while(1) {  // while_0 close
        // accept()  
        addrlen = sizeof(clientaddr);  
        client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);  
        if(client_sock == INVALID_SOCKET){  
            err_display("accept()");  
            break;  
        }  

        // TCP Connect with Client
        while(1){ // while_1 open 
			buf[retval] = '\0';
			system("cls"); // Console Window Clear
			printf("Sabo's Restaurant: Welcome! %s\n", inet_ntoa(clientaddr.sin_addr));
			printf("[The waiter]: �մ�, �ֹ��Ͻðڽ��ϱ�?(Y/N)\n");
			printf("buf=%s\n", buf);
			// Receive data
			// Wait for client's input
			retval = recv(client_sock, buf, BUFSIZE, 0);
			if(retval == SOCKET_ERROR){ break; } else if(retval == 0) break;
			while(1) { // while_2 open

				if (strnicmp(buf,"Y", 1) == 0) { // if_2 open
					system("cls"); // Console Window Clear
					printf("[The waiter]: �޴��� ���� �ֽ��ϴ�.\n");
					printf("¥���(A) - 4000��,	«��(B) - 4500��\n");
					printf("[The waiter]: �ֹ��Ͻ� ���� �޴��� �����Ͻʽÿ�.\n");

					while(1) { // while_3 open
						// Receive data 
						retval = recv(client_sock, buf, BUFSIZE, 0);
						if(retval == SOCKET_ERROR){ break; } else if(retval == 0) break;
						system("cls"); // Console Window Clear

						if (strnicmp(buf, "A", 1) == 0) { // if_3 open
							printf("[The waiter]: ¥���(A)�� �� �׸� �ֹ��Ͻðڽ��ϱ�?\n");
							// Receive data   
							retval = recv(client_sock, buf, BUFSIZE, 0);
							if(retval == SOCKET_ERROR){ break; } else if(retval == 0) break;
							jajangmyeon(atoi(buf));

							while(1) { // while_4 open
								printf("[The waiter]: ����Ͻðڽ��ϱ�?(Y)\n");
								// Receive data   
								retval = recv(client_sock, buf, BUFSIZE, 0);
								if(retval == SOCKET_ERROR){ break; } else if(retval == 0) break;

								if (strnicmp(buf, "Y", 1) == 0) { // if_4 open
									// Later, i'll add MySQL for cash data. - Re-kim
									total = A_dish*4000; // total food price
									printf("[The waiter]: �� ������ %d �Դϴ�.\n", total);
									printf("[The waiter]: ���ݸ� �޽��ϴ�.\n"); // Only cash!
									// Receive data   
									retval = recv(client_sock, buf, BUFSIZE, 0);
									if(retval == SOCKET_ERROR){ break; } else if(retval == 0) break;

									if (atoi(buf) == total) { // if_5 open
										printf("[The waiter]: ���������� ������ �Ǿ����ϴ�.\n");
										break;
									} else {
										printf("[The waiter]: ���� �����մϴ�.\n");
									} // if_5 close

								} // if_4 close

							} // while_4 close

						} else if (strnicmp(buf, "B", 1) == 0) { 
							// Copy to Food(A)
							printf("[The waiter]: «��(B)�� �� �׸� �ֹ��Ͻðڽ��ϱ�?\n");
						} else {
							printf("[The waiter]: �˼������� �׷� �޴��� �����ϴ�.\n");
							Sleep(1000);
							break;
						} // if_3 close

					} // while_3 close

				} else { 
					printf("[The waiter]: �˰ڽ��ϴ�. ���߿� �����ּ���.\n");
					Sleep(1000);
					break;
				} // if_2 close

			} // while_2 close

		} // while_1 close
	} // while_0 close

    // closesocket()  
    closesocket(client_sock);  
    printf("Sabo's Restaurant: GoodBye! %s\n", inet_ntoa(clientaddr.sin_addr));

    // closesocket()  
    closesocket(listen_sock);  
  
    // Exit Winsock  
    WSACleanup();  
    return 0;  
}  