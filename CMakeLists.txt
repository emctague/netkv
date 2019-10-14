cmake_minimum_required(VERSION 3.15)
project(netkv C)

set(CMAKE_C_STANDARD 99)


add_executable(netkv main.c main.h session_state.c db_state.c exit_handler.c network.c session.c hash.c)
target_link_directories(netkv PUBLIC /usr/local/lib /usr/local/opt/openssl/lib)
target_include_directories(netkv PUBLIC /usr/local/include /usr/local/opt/openssl/include)
target_link_libraries(netkv gdbm crypto)
