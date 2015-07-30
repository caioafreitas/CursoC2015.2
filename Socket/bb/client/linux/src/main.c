#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define IP_ADDR "192.168.1.2"
#define PORT_NO 666

int main(void)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd >= 0)
    {
        struct sockaddr_in serv_addr = {0};
        struct timeval tv = {0};

        tv.tv_sec = 10;

        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*) &tv, sizeof(struct timeval));

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(IP_ADDR);
        serv_addr.sin_port = htons(PORT_NO);

        if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) >= 0)
        {
            char header[2] = {0x01, 00};
            char buffer[256];
            int n;
 
            printf("Please enter the message: ");
            memset((void*) buffer, 0, 256);
            fgets(buffer, 255, stdin);

            header[1] = strlen(buffer)-1;

            write(sockfd, header, 2);
            n = write(sockfd, buffer, strlen(buffer));

            if (n >= 0)
            {
                memset((void*) buffer, 0, 256);
                n = read(sockfd, buffer, 255);

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

        close(sockfd);
    }
    else
    {
        perror("ERROR opening socket");
    }

    return 0;
}
