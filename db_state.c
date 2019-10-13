/* netkv - Copyright (C) 2019 Ethan McTague
 * Licensed under the MIT license. */

#include "main.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

void db_state_init(db_state *state, GDBM_FILE db) {
    state->db = db;
    assert(!pthread_mutex_init(&state->lock, NULL));

    state->password = gdbm_fetch(state->db, dtm("~password")).dptr;

    char *lock_setting = gdbm_fetch(state->db, dtm("~lock")).dptr;
    state->is_locked = lock_setting && !strcmp(lock_setting, "yes");
    free(lock_setting);
}

void db_state_deinit(db_state *state) {
    free(state->password);
    gdbm_close(state->db);
    pthread_mutex_destroy(&state->lock);
}
