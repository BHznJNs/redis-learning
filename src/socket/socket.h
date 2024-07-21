#ifndef __NET_SOCKET_H__
#define __NET_SOCKET_H__

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define BIND_ADDR "127.0.0.1"
#define BIND_PORT 8000

int create_socket();
void panic(const char *fmt, ...);

#endif
