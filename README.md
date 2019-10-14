# netkv

A **simple**, **networked** key-value store based on GDBM.

[![asciicast](https://asciinema.org/a/8OdgJ65i424Et8t3wjVf4Zijn.svg)](https://asciinema.org/a/8OdgJ65i424Et8t3wjVf4Zijn)

## The basics

Netkv maps string keys to string values. Launch the server like this>

    netkv <file> <port>

Terminate with SIGTERM (^C) for safe shutdown. Other means of killing the server should also be safe.

## Protocol

### Commands

Commands are sent on individual lines, separated by \\n.
Commands take the form:

    <key>[=<value>]

(e.g. to assign "name" the value "hello", send `name=hello`,
 and to retrieve the value in "name", send `name`.)
 
Disconnection is performed by transmitting EOF (^D in nc/telnet).

### Responses

Responses take on the form:

    <indicator><value>

`<indicator>` is one of the following:

| Indicator | Meaning |
| --------- | ------- |
| =         | **Success**, the value of the key set/retrieved follows. |
| !         | **Error**, The error message follows. |
| ?         | **Information**, an informational message follows. |


### Special Cases and Errors

The error message `!Not Set` will result from failure to assign a
key. The message `!Not Gotten` will result from failure to get a
value, which usually appears because the given key does not exist.

If **password protection** is enabled, the message `?Needs Auth` is
sent upon connecting. In this state, attempting to get or set
values will result in `!Not Authorized` and disconnection. The
special value `~password` should be set to the server's password
in order to authenticate, after which `?Authorized` will be sent.

If locking is enabled, attempts to access special values (prefixed
with `~`) will result in the error message `!Locked`. This excludes
authorization via `~password=...`, which is a special case and does not actually perform assignment


## Server Behavior

### Special Values

Special values, beginning with "~", are used to configure the
database. The following keys exist:

#### `~password`

Empty by default, may be set to a password. The Password is stored
with a SHA512 hash, prefixed by a 32-character salt.
Once set, new connections will require authentication using the
correct password (not hashed.)

#### `~locked`

Empty by default, when set to `yes` locking is enabled. Once
enabled, special values may not be set or retrieved, **INCLUDING**
`~locked` itself. If you wish to disable locking, a third-party
tool such as gdbmtool should be used. User-specified values
beginning with `~` will also be locked.


## Compilation Notes

In its current state, netkv includes the gdbm header via an absolute
path, and forces the compiler to include a system include directory.
This is a temporary fix for odd behavior on MacOS and should be fixed
soon to be platform-independent.


## Copyright and Licensing

netkv source code, documentation, binaries, and other related content
is Copyright (C) 2019 Ethan McTague.

netkv source code is distributed under the MIT license. See the included
LICENSE file for more information.
