#include "traceroute.h"

boolean check_checksum(struct icmphdr *);
boolean check_header(struct icmphdr *, int);

boolean check_response(char *buffer, int seq) {
    struct iphdr *ip = (struct iphdr *)buffer;
    struct icmphdr *icmp = (struct icmphdr *)(buffer + ip->ihl * 4);
    if (!check_header(icmp, seq)) {
        return false;
    }
    if (!check_checksum(icmp)) {
        return false;
    }
    return true;
}

boolean check_checksum(struct icmphdr *header) {
    u_int16_t checksum = header->checksum;
    header->checksum = 0;
    u_int16_t expected_checksum = calculate_checksum(header);
    if (checksum != expected_checksum) {
        printf("Error: checksum does not match %d !+ %d\n", checksum,
               expected_checksum);
    }
    return checksum == expected_checksum;
}

boolean check_header(struct icmphdr *header, int seq) {
    int pid = getpid();
    return (header->type == 0 && header->code == 0 &&
            header->un.echo.id == pid && header->un.echo.sequence == seq);
}

uint16_t calculate_checksum(void *packet) {
    uint16_t *buffer = packet;
    uint16_t sum = 0;
    int max = sizeof(Packet);

    for (int i = 0; i < max; i += 2)
        sum += *buffer++;
    if (max % 2 != 0) {
        sum += *(unsigned char *)buffer;
    }

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);

    return ~sum;
}
