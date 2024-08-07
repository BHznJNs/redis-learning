#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "socket.h"

static struct sockaddr_in* create_addr(char *addr, int port);
static int client(char *addr, int port);

// --- --- --- --- --- ---

static struct sockaddr_in* create_addr(char *addr, int port) {
    struct sockaddr_in* socket_addr = malloc(sizeof(struct sockaddr_in));
    memset(socket_addr, 0, sizeof(socket_addr));
    socket_addr->sin_family = AF_INET; // IPv4
    socket_addr->sin_port = htons(port); // set port

    int status_code = inet_aton(addr, &socket_addr->sin_addr);
    if (status_code != 0) {
        return socket_addr;
    }

    // addr is invalid, maybe a hostname like `localhost`
    struct hostent* he;
    he = gethostbyname(addr);

    if (he == NULL) {
        panic("can't resolve: %s\n", addr);
    }
    memcpy(&socket_addr->sin_addr, he->h_addr, sizeof(struct in_addr));
    return socket_addr;
}

static int client(char *addr, int port) {
    int socket_fd = create_socket();
    struct sockaddr_in* socket_addr = create_addr(addr, port);
    try_connect(socket_fd, socket_addr, sizeof(socket_addr));
    return socket_fd;
}

int main() {
    int client_fd = client(BIND_ADDR, BIND_PORT);

    char msg[] = "Test content";
    write(client_fd, msg, sizeof(msg));

    char buf[256];
    read(client_fd, buf, sizeof(buf));
    printf("[Server] %s\n", buf);

    close(client_fd);
    return 0;
}