#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <errno.h>

#define PORT_NO 9090

#define INVALID_SOCKET -1
#define MAX_SOCKETS 10

int create_socket(void);
int bind_socket(int sockfd, int port, int timeout_recv);
int wait_connection(int sockfd);
int do_processing(int sockfd);
void disconnect_socket(int sockfd);
void destroy_socket(int sockfd);
int send_socket(int sockfd, char* buffer, int to_send);
int recv_socket(int sockfd, char* buffer, int to_read);
int keyboard_read(char* message, char* buffer, int to_read);

int main(int argc, char *argv[])
{
    int sockfd = create_socket();

    if (sockfd != INVALID_SOCKET)
    {
        if (bind_socket(sockfd, PORT_NO, 10))
        {
            int sockets[MAX_SOCKETS] = {INVALID_SOCKET};
            int opened_sockets = 0;

            while (1)
            {
                int idx, max;
                if (opened_sockets < MAX_SOCKETS)
                {
                    int new_sockfd = wait_connection(sockfd);

                    if (new_sockfd != INVALID_SOCKET)
                    {
                        sockets[opened_sockets++] = new_sockfd;
                    }
                    else if (errno != EAGAIN)
                    {
                        perror("ERROR waiting");
                    }
                }

                for (idx = 0, max = opened_sockets; idx < max; idx++)
                {
                    if (sockets[idx] != INVALID_SOCKET)
                    {
                        if (!do_processing(sockets[idx]))
                        {
                            if (errno != EAGAIN)
                            {
                                printf("Disconnecting...\n");
                                disconnect_socket(sockets[idx]);
                                sockets[idx] = sockets[opened_sockets-1];
                                sockets[opened_sockets-1] = INVALID_SOCKET;
                                opened_sockets--;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            perror("ERROR binding");
        }

        disconnect_socket(sockfd);
    }
    else
    {
        perror("ERROR opening socket");
    }

    destroy_socket(sockfd);

    return 0;
}

int create_socket(void)
{
    return socket(AF_INET, SOCK_STREAM, 0);
}

int bind_socket(int sockfd, int port, int timeout_recv)
{
    int result = 0;

    if (sockfd != INVALID_SOCKET)
    {
        struct sockaddr_in serv_addr = {0};
        struct timeval tv = {0};
        
        tv.tv_usec = 100;

        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*) &tv, sizeof(struct timeval));

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(port);

        result = bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == 0;
    }

    return result;
}

int wait_connection(int sockfd)
{
    int new_sockfd = INVALID_SOCKET;

    if (sockfd != INVALID_SOCKET)
    {
        int clilen;
        struct sockaddr_in cli_addr;

        listen(sockfd, MAX_SOCKETS);
        clilen = sizeof(cli_addr);

        new_sockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t*) &clilen);

        if (new_sockfd != INVALID_SOCKET)
        {
            struct timeval tv = {0};

            tv.tv_usec = 100;

            setsockopt(new_sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*) &tv, sizeof(struct timeval));

            printf("New connection incoming: %s:%d\n", inet_ntoa(cli_addr.sin_addr),
                ntohs(cli_addr.sin_port));
        }
    }

    return new_sockfd;
}

int do_processing(int sockfd)
{
    int result = 0;

    if (sockfd != INVALID_SOCKET)
    {
        int read;
        char buffer[256] = {0};

        if ((read = recv_socket(sockfd, buffer, 255)) >= 0)
        {
            send_socket(sockfd, buffer, read);
            result = 1;
        }
    }

    return result;
}

void disconnect_socket(int sockfd)
{
    if (sockfd != INVALID_SOCKET)
    {
        close(sockfd);
    }
}

void destroy_socket(int sockfd)
{
    /* Empty */
}

int send_socket(int sockfd, char* buffer, int to_send)
{
    int sent = 0;

    if ((sockfd != INVALID_SOCKET) && buffer && (to_send > 0))
    {
        sent = send(sockfd, buffer, to_send, 0); //write
    }

    return sent;
}

int recv_socket(int sockfd, char* buffer, int to_read)
{
    int read = 0;

    if ((sockfd != INVALID_SOCKET) && buffer && (to_read > 0))
    {
        read = recv(sockfd, buffer, to_read, 0); //read
    }

    return read;
}
