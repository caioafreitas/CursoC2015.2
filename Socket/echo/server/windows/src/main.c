#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT_NO 9090

#define MAX_SOCKETS 10

SOCKET create_socket(void);
int bind_socket(SOCKET* sockfd, int port, int timeout_recv);
int wait_connection(SOCKET* sockfd);
int do_processing(SOCKET* sockfd);
void disconnect_socket(SOCKET* sockfd);
void destroy_socket(SOCKET* sockfd);
int send_socket(SOCKET* sockfd, char* buffer, int to_send);
int recv_socket(SOCKET* sockfd, char* buffer, int to_read);

int main(int argc, char *argv[])
{
    SOCKET sockfd = create_socket();

    if (sockfd != INVALID_SOCKET)
    {
        if (bind_socket(&sockfd, PORT_NO, 10))
        {
            SOCKET sockets[MAX_SOCKETS] = {INVALID_SOCKET};
            int opened_sockets = 0;

            while (1)
            {
                int idx, max;
				
                if (opened_sockets < MAX_SOCKETS)
                {
                    SOCKET new_sockfd = wait_connection(&sockfd);

                    if (new_sockfd != INVALID_SOCKET)
                    {
                        sockets[opened_sockets++] = new_sockfd;
                    }
                    else if (WSAGetLastError() != WSAETIMEDOUT)
                    {
                        //printf("ERROR waiting\n");
                    }
                }

                for (idx = 0, max = opened_sockets; idx < max; idx++)
                {
                    if (sockets[idx] != INVALID_SOCKET)
                    {
                        if (do_processing(&sockets[idx]) < 0)
                        {
							printf("Disconnecting...\n");
							disconnect_socket(&sockets[idx]);
							sockets[idx] = sockets[opened_sockets-1];
							sockets[opened_sockets-1] = INVALID_SOCKET;
							opened_sockets--;
                        }
                    }
                }
            }
        }
        else
        {
            perror("ERROR binding");
        }

        disconnect_socket(&sockfd);
    }
    else
    {
        perror("ERROR opening socket");
    }

    destroy_socket(&sockfd);

    return 0;
}

SOCKET create_socket(void)
{
    SOCKET sock = INVALID_SOCKET;
    WSADATA wsaData = {0};

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) == 0)
    {
        sock = socket(AF_INET, SOCK_STREAM, 0);
    }

    return sock;
}

int bind_socket(SOCKET* sockfd, int port, int timeout_recv)
{
    int result = 0;

    if (*sockfd != INVALID_SOCKET)
    {
        struct sockaddr_in serv_addr = {0};
        struct timeval tv = {0};
        
        tv.tv_usec = 100;
        setsockopt(*sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*) &tv, sizeof(struct timeval));

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(port);

        result = bind(*sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == 0;
    }

    return result;
}

int wait_connection(SOCKET* sockfd)
{
    SOCKET new_sockfd = INVALID_SOCKET;

    if (*sockfd != INVALID_SOCKET)
    {
        int clilen;
        struct sockaddr_in cli_addr;
		
		struct timeval tv = {0};
		tv.tv_usec = 100;
		
		FD_SET fdset = {0};
		FD_SET (*sockfd, &fdset);

        listen(*sockfd, MAX_SOCKETS);
        clilen = sizeof(cli_addr);

		if (select (0, &fdset, NULL, NULL, &tv) > 0)
		{	
			new_sockfd = accept(*sockfd, (struct sockaddr *) &cli_addr, (socklen_t*) &clilen);
            printf("New connection incoming: %s : %d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
        }
    }

    return new_sockfd;
}

int do_processing(SOCKET* sockfd)
{
    int result = 0;

    if (*sockfd != INVALID_SOCKET)
    {
        int read;
        char buffer[256] = {0};
		
		struct timeval tv = {0};
		tv.tv_usec = 100;
		
		FD_SET fdset = {0};
		FD_SET (*sockfd, &fdset);
		
		result = select (0, &fdset, NULL, NULL, &tv);
		
		if (result > 0)
		{	
			if ((read = recv_socket(sockfd, buffer, 255)) >= 0)
			{
				send_socket(sockfd, buffer, read);
				result = 1;
			}
		}
    }

    return result;
}

void disconnect_socket(SOCKET* sock)
{
    if (sock)
    {
        closesocket(*sock);
    }
}

void destroy_socket(SOCKET* sock)
{
    WSACleanup();
}

int send_socket(SOCKET* sock, char* buffer, int to_send)
{
    int sent = 0;

    if (sock && buffer && (to_send > 0))
    {
        sent = send(*sock, buffer, to_send, 0);
    }

    return sent;
}

int recv_socket(SOCKET* sock, char* buffer, int to_read)
{
    int read = 0;

    if (sock && buffer && (to_read > 0))
    {
        read = recv(*sock, buffer, to_read, 0);
    }

    return read;
}
