/*
    Simple UDP client to send a file
    usage: udpclient <server ip> <port> <filename>
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFLEN 512  // Max length of buffer

void die(const char *s)
{
    perror(s);
    exit(1);
}

int main(int argc, char **argv)
{
    struct sockaddr_in si_other;
    int s, slen = sizeof(si_other);
    char buf[BUFLEN];
    FILE *file;
    size_t nread;

    if (argc != 4) {
        fprintf(stderr, "Usage: %s <server ip> <port> <filename>\n", argv[0]);
        return -1;
    }

    // Create a UDP socket
    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        die("socket");

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(atoi(argv[2]));

    if (inet_aton(argv[1], &si_other.sin_addr) == 0)
        die("inet_aton");

    // Open the file
    file = fopen(argv[3], "rb");
    if (file == NULL)
        die("fopen");

    // Send the file in chunks
    while ((nread = fread(buf, 1, BUFLEN, file)) > 0) {
        if (sendto(s, buf, nread, 0, (struct sockaddr *) &si_other, slen) == -1)
            die("sendto()");
    }

    if (ferror(file)) {
        perror("fread");
        fclose(file);
        close(s);
        exit(1);
    }

    fclose(file);
    printf("File sent successfully.\n");

    close(s);
    return 0;
}
