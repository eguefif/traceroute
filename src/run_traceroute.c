#include "traceroute.h"
#define RECV_TIMEOUT 1
#define BUFF_SIZE 1000

boolean running = true;

Packet init_packet(int seq);
int init_socket();
void print_buffer(char *buffer, int n);
void send_ping(int sockfd, struct sockaddr *addr, int seq);
boolean handle_response(int sockfd, int seq);
void signalHandler();

void run_traceroute(Params params) {
    int sockfd = init_socket();

    signal(SIGINT, signalHandler);

    while (running) {
    }
}

void signalHandler() { running = false; }

int init_socket() {
    struct timeval timeout;

    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        fprintf(stderr, "Error: socket not created\n");
        exit(EXIT_FAILURE);
    }

    int ttl = TTL;
    if (setsockopt(sockfd, SOL_IP, IP_TTL, &ttl, sizeof(ttl)) != 0) {
        fprintf(stderr, "Error: impossible to set ttl for socket\n");
        exit(EXIT_FAILURE);
    }

    timeout.tv_sec = RECV_TIMEOUT;
    timeout.tv_usec = 0;

    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (void *)&timeout,
                   sizeof(struct timeval)) != 0) {

        fprintf(stderr, "Error: impossible to set timeout for socket\n");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}

void send_ping(int sockfd, struct sockaddr *addr, int seq) {
    Packet packet;
    packet = init_packet(seq);
    if (sendto(sockfd, &packet, sizeof(Packet), 0, addr,
               sizeof(struct sockaddr_in)) == -1) {
        fprintf(stderr, "Error: failed to send packet\n");
        exit(EXIT_FAILURE);
    }
}

Packet init_packet(int seq) {
    Packet packet;

    bzero(&packet, sizeof(Packet));
    packet.header.type = ICMP_ECHO;
    packet.header.un.echo.id = getpid();
    packet.header.un.echo.sequence = seq;

    for (int i = 0; i < 10; i++) {
        packet.message[i] = (char)i + 33;
    }
    packet.header.checksum = calculate_checksum(&packet);

    return packet;
}

boolean handle_response(int sockfd, int seq) {
    char buffer[BUFF_SIZE];

    int n = recv(sockfd, buffer, BUFF_SIZE, 0);
    if (n > 1) {
        return check_response(buffer, seq) ? true : false;
    } else {
        fprintf(stderr, "Probe timeout\n");
    }
    return false;
}
