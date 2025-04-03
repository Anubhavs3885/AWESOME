#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345

// Function to send SYN packet
void send_syn(int sock, struct sockaddr_in *server_addr) {
    char packet[sizeof(struct iphdr) + sizeof(struct tcphdr)];
    memset(packet, 0, sizeof(packet));

    struct iphdr *ip = (struct iphdr *)packet;
    struct tcphdr *tcp = (struct tcphdr *)(packet + sizeof(struct iphdr));

    // Fill IP header
    ip->ihl = 5;
    ip->version = 4;
    ip->tos = 0;
    ip->tot_len = htons(sizeof(packet));
    ip->id = htons(54321);
    ip->frag_off = 0;
    ip->ttl = 64;
    ip->protocol = IPPROTO_TCP;
    ip->saddr = inet_addr(SERVER_IP);
    ip->daddr = server_addr->sin_addr.s_addr;

    // Fill TCP header
    tcp->source = htons(12346);
    tcp->dest = htons(SERVER_PORT);
    tcp->seq = htonl(200); // Initial sequence number
    tcp->syn = 1;
    tcp->doff = 5;
    tcp->window = htons(8192);
    tcp->check = 0;

    // Send packet
    if (sendto(sock, packet, sizeof(packet), 0, (struct sockaddr *)server_addr, sizeof(*server_addr)) < 0) {
        perror("sendto() failed");
    } else {
        std::cout << "[+] Sent SYN" << std::endl;
    }
}

// Function to receive SYN-ACK and send ACK
void receive_syn_ack_send_ack(int sock) {
    char buffer[65536];
    struct sockaddr_in source_addr;
    socklen_t addr_len = sizeof(source_addr);

    while (true) {
        int data_size = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&source_addr, &addr_len);
        if (data_size < 0) {
            perror("recvfrom() failed");
            continue;
        }

        struct iphdr *ip = (struct iphdr *)buffer;
        struct tcphdr *tcp = (struct tcphdr *)(buffer + (ip->ihl * 4));

        if (tcp->syn == 1 && tcp->ack == 1 && ntohl(tcp->ack_seq) == 201) {
            std::cout << "[+] Received SYN-ACK" << std::endl;
            
            // Construct ACK packet
            char ack_packet[sizeof(struct iphdr) + sizeof(struct tcphdr)];
            memset(ack_packet, 0, sizeof(ack_packet));

            struct iphdr *ip_ack = (struct iphdr *)ack_packet;
            struct tcphdr *tcp_ack = (struct tcphdr *)(ack_packet + sizeof(struct iphdr));

            // Fill IP header
            ip_ack->ihl = 5;
            ip_ack->version = 4;
            ip_ack->tot_len = htons(sizeof(ack_packet));
            ip_ack->protocol = IPPROTO_TCP;
            ip_ack->saddr = inet_addr(SERVER_IP);
            ip_ack->daddr = source_addr.sin_addr.s_addr;

            // Fill TCP header
            tcp_ack->source = htons(12346);
            tcp_ack->dest = htons(SERVER_PORT);
            tcp_ack->seq = htonl(600);
            tcp_ack->ack_seq = htonl(ntohl(tcp->seq) + 1);
            tcp_ack->ack = 1;
            tcp_ack->doff = 5;

            // Send ACK
            if (sendto(sock, ack_packet, sizeof(ack_packet), 0, (struct sockaddr *)&source_addr, sizeof(source_addr)) < 0) {
                perror("sendto() ACK failed");
            } else {
                std::cout << "[+] Sent ACK, handshake complete." << std::endl;
                break;
            }
        }
    }
}

int main() {
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    send_syn(sock, &server_addr);
    receive_syn_ack_send_ack(sock);

    close(sock);
    return 0;
}
