/* netkv - Copyright (C) 2019 Ethan McTague
 * Licensed under the MIT license. */

#include "main.h"
#include <string.h>
#include <stdlib.h>

void handle_session(session_state *state) {
    size_t lsize = 0;
    char *line = NULL;

    if (!state->authorized) {
        fprintf(state->sock, "?Needs Auth\n");
        fflush(state->sock);
    }

    while (state->active && getline(&line, &lsize, state->sock) != EOF) {
        line[strlen(line) - 1] = '\0';

        char *value = line;
        char *key = strsep(&value, "=");
        char *response = NULL;

        if (value) response = handle_assignment(state, key, value);
        else response = handle_retrieval(state, key);

        if (response) fprintf(state->sock, "%s\n", response);
        fflush(state->sock);
        free(response);
    }

    free(line);
    session_state_destroy(state);
}


char *handle_assignment(session_state *state, const char *key, char *value) {
    if (!state->authorized) {
        if (!strcmp(key, "~password") && hash_equals(state->db_state->password, value)) {
            state->authorized = 1;
            return strdup("?Authorized");
        } else {
            state->active = 0;
            return strdup("!Not Authorized");
        }
    } else {
        if (state->db_state->is_locked && key[0] == '~') return strdup("!Locked");
        else {
            if (!strcmp(key, "~password")) {
                char *salt = make_salt();
                value = hash(value, salt);
                free(salt);
                free(state->db_state->password);
                state->db_state->password = value;
            } else if (!strcmp(key, "~locked") && !strcmp(value, "yes")) {
                state->db_state->is_locked = 1;
            }

            pthread_mutex_lock(&state->db_state->lock);
            int result = gdbm_store(state->db_state->db, dtm(key), dtm(value), GDBM_REPLACE);
            pthread_mutex_unlock(&state->db_state->lock);

            if (result)
                return strdup("!Not Set");
            else {
                char *result;
                asprintf(&result, "=%s", value);
                return result;
            }
        }
    }
}


char *handle_retrieval(session_state *state, const char *key) {
    if (!state->authorized) {
        state->active = 0;
        return strdup("!Not Authorized");
    } else {
        if (state->db_state->is_locked && key[0] == '~') return strdup("!Locked");
        else {
            pthread_mutex_lock(&state->db_state->lock);
            char *value = gdbm_fetch(state->db_state->db, dtm(key)).dptr;
            pthread_mutex_unlock(&state->db_state->lock);

            if (value) {
                char *result;
                asprintf(&result, "=%s", value);
                free(value);
                return result;
            } else {
                return strdup("!Not Gotten");
            }
        }
    }
}