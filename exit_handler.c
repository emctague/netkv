/* netkv - Copyright (C) 2019 Ethan McTague
 * Licensed under the MIT license. */

#include "main.h"
#include <signal.h>
#include <stdlib.h>

db_state *exit_handler_db_state = NULL;

void on_exit(int _) {
    db_state_deinit(exit_handler_db_state);
    exit(0);
}

void bind_exit_handler(db_state *state) {
    exit_handler_db_state = state;
    signal(SIGHUP, &on_exit);
    signal(SIGINT, &on_exit);
    signal(SIGABRT, &on_exit);
    signal(SIGQUIT, &on_exit);
    signal(SIGKILL, &on_exit);
    signal(SIGTSTP, &on_exit);
    signal(SIGTERM, &on_exit);
}