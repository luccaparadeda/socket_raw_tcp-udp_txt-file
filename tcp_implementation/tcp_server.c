/*
	Simple TCP server to receive a file and print its contents
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
 
#define BUFLEN 1024   // Max length of buffer
#define PORT    8080  // The port on which to listen for incoming data

void die(const char *s)
{
    perror(s);
    exit(1);
}
 
int main(void)
{
    struct sockaddr_in si_me, si_other;
    int s, slen = sizeof(si_other), recv_len, conn;
    char buf[BUFLEN];
    FILE *file;

    /* Create a TCP socket */
    if ((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
        die("socket");
    
    /* Zero out the structure */
    memset((char *) &si_me, 0, sizeof(si_me));
     
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
     
    /* Bind socket to port */
    if (bind(s, (struct sockaddr*)&si_me, sizeof(si_me)) == -1)
        die("bind");
    
    /* Allow 5 requests to queue up */ 
    if (listen(s, 5) == -1)
        die("listen");
     
    /* Keep listening for data */
    while (1) {
        memset(buf, 0, sizeof(buf));
        printf("Waiting for a connection...\n");
        fflush(stdout);
        
        conn = accept(s, (struct sockaddr *) &si_other, (socklen_t *)&slen);
        if (conn < 0)
            die("accept");

        printf("Client connected: %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));

        /* Open a file to write the received data */
        file = fopen("received_file.txt", "wb");
        if (file == NULL) {
            die("fopen");
        }

        /* Receive data in chunks and write to the file */
        while ((recv_len = read(conn, buf, BUFLEN)) > 0) {
            fwrite(buf, 1, recv_len, file);
            memset(buf, 0, BUFLEN);
        }

        if (recv_len < 0) {
            die("read");
        }

        fclose(file);

        printf("File received and saved as received_file.txt\n");

        /* Close the connection */
        close(conn);
    }

    close(s);
    return 0;
}
