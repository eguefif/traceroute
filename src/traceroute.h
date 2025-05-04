#ifndef PING_H
#include <arpa/inet.h>
#include <errno.h>
#include <math.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#define true 1
#define false 0
#define EXIT_FAILURE 1
#define TTL 64
#define PING_SIZE 64

typedef int boolean;

typedef struct {
    struct icmphdr header;
    char message[PING_SIZE - sizeof(struct icmphdr)];
} Packet;

typedef struct {
    char *target;
    boolean verbose;
    char *host;
    char *ip;
    struct sockaddr_in addr;
} Params;

void parse_params(Params *params, char **argv, int argc);
void resolve_dns(Params *params);
void run_traceroute(Params params);
char *reverse_resolve_dns(struct sockaddr *addr, char *ip);

boolean check_response(char *buffer, int seq);
uint16_t calculate_checksum(void *packet);
#endif
