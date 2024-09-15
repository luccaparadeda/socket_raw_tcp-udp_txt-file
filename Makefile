CC      = gcc
CFLAGS  = -O2 -Wall

all:
	$(CC) $(CFLAGS) ./tcp_implementation/tcp_server.c -o ./tcp_implementation/simple_tcp_server
	$(CC) $(CFLAGS) ./tcp_implementation/tcp_client.c -o ./tcp_implementation/simple_tcp_client
	$(CC) $(CFLAGS) ./udp_implementation/udp_server.c -o ./udp_implementation/simple_udp_server
	$(CC) $(CFLAGS) ./udp_implementation/udp_client.c -o ./udp_implementation/simple_udp_client
udp:
	$(CC) $(CFLAGS) ./udp_implementation/udp_server.c -o ./udp_implementation/simple_udp_server
	$(CC) $(CFLAGS) ./udp_implementation/udp_client.c -o ./udp_implementation/simple_udp_client
tcp:
	$(CC) $(CFLAGS) ./tcp_implementation/tcp_server.c -o ./tcp_implementation/simple_tcp_server
	$(CC) $(CFLAGS) ./tcp_implementation/tcp_client.c -o ./tcp_implementation/simple_tcp_client
clean:
	-rm -f *.o *~ ./tcp_implementation/simple_tcp_server ./tcp_implementation/simple_tcp_client ./udp_implementation/simple_udp_server ./udp_implementation/simple_udp_client
