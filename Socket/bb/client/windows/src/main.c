#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "socket.h"

#define IP_ADDR "192.168.150.28"
#define PORT_NO 666

int main(void)
{
    SOCKET sockfd = create_socket();

    if (sockfd != INVALID_SOCKET)
    {
        if (connect_socket(&sockfd, IP_ADDR, PORT_NO, 10))
        {
			char buffer[256];
			int n, op;
 
			printf ("\n\n1-Receber texto\n2-Receber CAPS\n3-Consultar LEDS\n4-Modificar LEDS\n");
			printf ("Opcao: ");
			scanf  ("%d", &op);
			if (op == 1)
			{
				char header[2] = {0x01, 00};
				printf ("Please enter the message: ");
				memset ((void*) buffer, 0, 256);
				scanf  ("%s", buffer);

				header[1] = strlen(buffer)-1;

				send_socket (&sockfd, header, 2);
				n = send_socket (&sockfd, buffer, strlen(buffer));
			}
			else if (op == 2)
			{
				char header[2] = {0x02, 00};
				printf ("Please enter the message: ");
				memset ((void*) buffer, 0, 256);
				scanf  ("%s", buffer);

				header[1] = strlen(buffer)-1;

				send_socket (&sockfd, header, 2);
				n = send_socket (&sockfd, buffer, strlen(buffer));
			}
			else if (op == 3)
			{
				char header[2] = {0x03, 00};
				memset ((void*) buffer, 0, 256);

				header[1] = strlen(buffer)-1;

				send_socket (&sockfd, header, 2);
				n = send_socket (&sockfd, buffer, strlen(buffer));
			}
			else if (op == 4)
			{
				int leds;
				char header[2] = {0x04, 00};
				printf ("Entre com o decimal referente a operacao: ");
				scanf  ("%d", &leds);
				memset ((void*) buffer, 0, 256);

				header[1] = leds;

				send_socket (&sockfd, header, 2);
				n = send_socket (&sockfd, buffer, strlen(buffer));
			}
			else printf ("Opcao Invalida!");
			
			if (n >= 0)
			{
				memset((void*) buffer, 0, 256);
				n = recv_socket(&sockfd, buffer, 255);

				if (n >= 0)
				{
					printf("%s\n", buffer);
				}
				else
				{
					printf("ERROR reading from socket");
				}
			}
			else
			{
				printf("ERROR writing to socket");
			}
		}
		else
		{
			printf("ERROR connecting");
		}

		disconnect_socket(&sockfd);
	}
    else
    {
        printf("ERROR opening socket");
    }

    destroy_socket(&sockfd);

    return 0;
}
