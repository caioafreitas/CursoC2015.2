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

int connect_socket(SOCKET* sock, char* ip, int port, int timeout_recv)
{
    int result = 0;

    if (sock && ip)
    {
        struct sockaddr_in serv_addr = {0};
        struct timeval tv = {0};

        tv.tv_sec = timeout_recv;
        setsockopt(*sock, SOL_SOCKET, SO_RCVTIMEO, (char*) &tv, sizeof(struct timeval));

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(ip);
        serv_addr.sin_port = htons(port);

        result = (connect(*sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) >= 0);
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
