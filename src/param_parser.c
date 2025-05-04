#include "traceroute.h"

void parse_target(char *target, Params *params);
struct sockaddr_in get_sockaddrin_from_str(char *ip);
boolean is_ip_addr(char *target);

void parse_params(Params *params, char **argv, int argc) {
    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "-v", 2) == 0) {
            params->verbose = true;
        } else {
            params->target = argv[i];
            parse_target(argv[i], params);
        }
    }
}

void parse_target(char *target, Params *params) {
    char *check = malloc(sizeof(char) * strlen(target));
    strncpy(check, target, strlen(target));
    if (is_ip_addr(check)) {
        params->ip = target;
        params->addr = get_sockaddrin_from_str(target);
        params->host =
            reverse_resolve_dns((struct sockaddr *)&params->addr, target);
    } else {
        params->host = target;
        resolve_dns(params);
        params->ip = inet_ntoa(params->addr.sin_addr);
    }
    free(check);
}

boolean is_ip_addr(char *target) {
    char *next = strtok(target, ".");
    int counter = 0;
    while (next != NULL) {
        int chunk = atoi(next);
        if (chunk < 0 || chunk > 255) {
            return false;
        }
        next = strtok(NULL, ".");
        counter++;
    }
    if (counter != 4) {
        return false;
    }
    return true;
}

struct sockaddr_in get_sockaddrin_from_str(char *ip) {
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(0);

    if (inet_pton(AF_INET, ip, &(addr.sin_addr)) != 1) {
        fprintf(stderr, "Error: ip address wrong format %s", ip);
        exit(EXIT_FAILURE);
    }

    return addr;
}
