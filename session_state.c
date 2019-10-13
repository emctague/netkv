/* netkv - Copyright (C) 2019 Ethan McTague
 * Licensed under the MIT license. */

#include "main.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

session_state *session_state_new(db_state *db_state, int sock_fd) {
    session_state *state = malloc(sizeof(session_state));
    state->db_state = db_state;
    state->sock_fd = sock_fd;
    state->sock = fdopen(sock_fd, "r+");
    state->active = 1;
    state->authorized = (db_state->password == NULL);
    return state;
}

void session_state_destroy(session_state *state) {
    fclose(state->sock);
    close(state->sock_fd);
    free(state);
}
