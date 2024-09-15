/*
    Simple TCP client to send a file
    usage: tcpclient <hostname> <port> <filename>
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFSIZE 1024

void die(const char *s)
{
    perror(s);
    exit(1);
}

int main(int argc, char **argv) {
    int sockfd, portno, n;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;
    char buf[BUFSIZE];
    FILE *file;
    size_t nread;

    /* check command line arguments */
    if (argc != 4) {
        fprintf(stderr, "usage: %s <hostname> <port> <filename>\n", argv[0]);
        exit(0);
    }
    hostname = argv[1];
    portno = atoi(argv[2]);
    const char *filename = argv[3];

    /* socket: create the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        die("socket");

    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL)
        die("gethostbyname");

    /* build the server's Internet address */
    memset((char *) &serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(portno);

    if (inet_aton(hostname, &serveraddr.sin_addr) == 0)
        die("inet_aton");

    /* connect: create a connection with the server */
    if (connect(sockfd, (const struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) 
        die("connect");

    /* Open the file */
    file = fopen(filename, "rb");
    if (file == NULL)
        die("fopen");

    /* Send the file in chunks */
    while ((nread = fread(buf, 1, BUFSIZE, file)) > 0) {
        if (write(sockfd, buf, nread) < 0)
            die("write");
    }

    if (ferror(file)) {
        perror("fread");
        fclose(file);
        close(sockfd);
        exit(1);
    }

    fclose(file);

    printf("File sent successfully.\n");

    close(sockfd);
    return 0;
}
