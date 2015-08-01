#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT_NO 9090

#define INVALID_SOCKET -1
#define MAX_SOCKETS 10

int create_socket(void);
int bind_socket(int sockfd, int port, int timeout_recv);
int wait_connection(int sockfd);
void do_processing(int sockfd);
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
            while (1)
            {
                int new_sockfd = wait_connection(sockfd);

                if (new_sockfd != INVALID_SOCKET)
                {
                    int pid = fork();

                    if (pid >= 0)
                    {
                        if (pid == 0)
                        {
                            disconnect_socket(sockfd);
                            do_processing(new_sockfd);
                            disconnect_socket(new_sockfd);
                        }
                    }
                    else
                    {
                        perror("ERROR forking");
                    }
                }
                else
                {
                    perror("ERROR waiting");
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
            printf("New connection incoming: %s:%d\n", inet_ntoa(cli_addr.sin_addr),
                ntohs(cli_addr.sin_port));
        }
    }

    return new_sockfd;
}

void do_processing(int sockfd)
{
    if (sockfd != INVALID_SOCKET)
    {
        int read;
        char buffer[256] = {0};

        while (1)
        {
            if ((read = recv_socket(sockfd, buffer, 255)) >= 0)
            {
                send_socket(sockfd, buffer, read);
            }
            else
            {
                break;
            }
        }
    }
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
