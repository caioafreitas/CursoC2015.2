#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define IP_ADDR "127.0.0.1"
#define PORT_NO 8080

#define INVALID_SOCKET -1

int create_socket(void);
int connect_socket(int sockfd, char* ip, int port, int timeout_recv);
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
        if (connect_socket(sockfd, IP_ADDR, PORT_NO, 10))
        {
            char buffer[256] = {0};
            int n;

            keyboard_read("Please enter the message: ", buffer, 255);

            n = send_socket(sockfd, buffer, strlen(buffer));

            if (n >= 0)
            {
                memset((void*) buffer, 0, 256);
                n = recv_socket(sockfd, buffer, 256);

                if (n >= 0)
                {
                    printf("%s\n", buffer);
                }
                else
                {
                    perror("ERROR reading from socket");
                }
            }
            else
            {
                perror("ERROR writing to socket");
            }
        }
        else
        {
            perror("ERROR connecting");
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

int connect_socket(int sockfd, char* ip, int port, int timeout_recv)
{
    int result = 0;

    if ((sockfd != INVALID_SOCKET) && ip)
    {
        struct sockaddr_in serv_addr = {0};
        struct timeval tv = {0};

        tv.tv_sec = 10;

        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*) &tv, sizeof(struct timeval));

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(ip);
        serv_addr.sin_port = htons(port);

        result = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) >= 0;
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

int keyboard_read(char* message, char* buffer, int to_read)
{
    int read = 0;

    if (message && buffer && (to_read > 0))
    {
        printf(message);

        if (fgets(buffer, to_read, stdin))
        {
            read = to_read;
        }
    }

    return read;
}
