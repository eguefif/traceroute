#include "traceroute.h"

struct addrinfo *get_addr_info(char *host);

void resolve_dns(Params *params) {
    struct addrinfo *result;

    result = get_addr_info(params->host);
    memcpy(&params->addr, result->ai_addr, sizeof(struct sockaddr));
    freeaddrinfo(result);
}

struct addrinfo *get_addr_info(char *host) {
    struct addrinfo hints;
    struct addrinfo *result;
    int error;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;
    hints.ai_flags = 0;
    if ((error = getaddrinfo(host, NULL, &hints, &result)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error));
        exit(EXIT_FAILURE);
    }

    if (result == NULL) {
        fprintf(stdout, "DNS resolution failed: no result\n");
        exit(EXIT_FAILURE);
    }

    return result;
}

char *reverse_resolve_dns(struct sockaddr *addr, char *ip) {
    char *host = malloc(200);
    if (getnameinfo(addr, sizeof(struct sockaddr), host, 200, NULL, 0, 0) !=
        0) {
        fprintf(stderr, "Error: impossible to reverse resolve dsn ip %s\n", ip);
        exit(EXIT_FAILURE);
    }
    return host;
}
