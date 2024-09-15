/*
    Simple UDP server to receive and save a file
    usage: udp_server <port>
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>

#define BUFLEN 512  // Max length of buffer

int s;  // Socket descriptor
FILE *file;  // File pointer

void die(const char *s)
{
    perror(s);
    exit(1);
}

void cleanup(int signum) {
    if (file) {
        fclose(file);
        printf("File closed cleanly.\n");
    }
    if (s) {
        close(s);
        printf("Socket closed cleanly.\n");
    }
    exit(0);
}

int main(int argc, char **argv)
{
    struct sockaddr_in si_me, si_other;
    int slen = sizeof(si_other), recv_len;
    char buf[BUFLEN];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return -1;
    }

    // Set up signal handler for clean termination
    signal(SIGINT, cleanup);
    signal(SIGTERM, cleanup);

    // Create a UDP socket
    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        die("socket");

    // Zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(atoi(argv[1]));
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind socket to port
    if (bind(s, (struct sockaddr*)&si_me, sizeof(si_me)) == -1)
        die("bind");

    // Open a file to write the received data
    file = fopen("received_file.txt", "wb");
    if (file == NULL)
        die("fopen");

    // Keep listening for data
    while (1) {
        memset(buf, 0, sizeof(buf));
        printf("Waiting for data...\n");
        fflush(stdout);

        // Try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, (socklen_t *)&slen)) == -1)
            die("recvfrom()");

        printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));

        // Write the received data to the file
        size_t written = fwrite(buf, 1, recv_len, file);
        if (written < recv_len) {
            perror("fwrite");
            cleanup(0);
        }

        // Flush the file buffer to ensure the data is written to disk
        fflush(file);
        printf("Data written to file.\n");
    }

    // Close the file and socket (though this will never be reached due to the infinite loop)
    fclose(file);
    close(s);
    return 0;
}
