/* netkv - Copyright (C) 2019 Ethan McTague
 * Licensed under the MIT license. */

#ifndef NETKV_MAIN_H
#define NETKV_MAIN_H

#include <pthread.h>
#include "/usr/local/include/gdbm.h"


/** Datum shorthand. */
datum dtm(const char *text);


/** State of the database as a whole. */
typedef struct {
    pthread_mutex_t lock; /** Must be acquired to use state struct. */
    GDBM_FILE db; /** Database handle. */

    char *password; /** Database password, SHA512 hashed. */
    int is_locked; /** Indicates if the database is 'locked'. */
} db_state;

void db_state_init(db_state *state, GDBM_FILE db);
void db_state_deinit(db_state *state);


/** State of an individual session. */
typedef struct {
    db_state *db_state; /** Global database state pointer. */
    int active; /** Session is still active. */
    int sock_fd; /** Session socket file descriptor. Should be closed after use. */
    FILE *sock; /** Session socket FILE* handle. Should be closed after use. */

    int authorized; /** Indicates if this session has logged in yet. */
} session_state;

session_state *session_state_new(db_state *db_state, int sock_fd);
void session_state_destroy(session_state *state);


/** Handle a session. */
void handle_session(session_state *state);


/** Create an internet socket listening on the given port. */
int bind_socket(int port);


/** Exit handler. */
void on_exit(int);

/** Database state pointer for exit handler. */
extern db_state *exit_handler_db_state;

/** Bind exit handler! */
void bind_exit_handler(db_state *state);


/** Database assignment handler. */
char *handle_assignment(session_state *state, const char *key, const char *value);

/** Database retrieval handler. */
char *handle_retrieval(session_state *state, const char *key);


#endif //NETKV_MAIN_H
