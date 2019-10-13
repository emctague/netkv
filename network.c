/* netkv - Copyright (C) 2019 Ethan McTague
 * Licensed under the MIT license. */

#include "main.h"
#include <netinet/in.h>
#include <string.h>

int bind_socket(int port) {
    int sock;
    int optval = 1;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void*)&optval, sizeof(int));
    bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    listen(sock, 1024);

    return sock;
}
