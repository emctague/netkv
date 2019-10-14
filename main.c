/* netkv - Copyright (C) 2019 Ethan McTague
 * Licensed under the MIT license. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include "main.h"

int main(int argc, char **argv) {
    GDBM_FILE db;
    int port;

    struct sockaddr_in cliaddr;
    socklen_t clilen;
    int sock;

    db_state state;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file> <port>\n", argv[0]);
        return 1;
    }

    port = atoi(argv[2]);
    if (!port) {
        fprintf(stderr, "Port isn't a valid port number.\n");
        return 2;
    }

    db = gdbm_open(argv[1], 1024, GDBM_WRCREAT, 0660, NULL);
    if (!db) {
        fprintf(stderr, "Unable to open database file.\n");
        return 3;
    }

    db_state_init(&state, db);
    sock = bind_socket(port);
    clilen = sizeof(cliaddr);
    bind_exit_handler(&state);

    for (;;) {
        int session = accept(sock, (struct sockaddr*)&cliaddr, &clilen);

        session_state *sess_state = session_state_new(&state, session);
        pthread_t thread;
        pthread_create(&thread, 0, (void(*)(void*))handle_session, (void*)sess_state);
        pthread_detach(thread);
    }
}

datum dtm(const char *text) {
    return (datum){ (void*)text, (int)strlen(text) + 1 };
}

