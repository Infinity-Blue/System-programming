CC	=	gcc

CFLAGS = -Wall -Werror



all : bankingServer bankingClient



bankingServer : bankingServer.c bankingServer.h

	$(CC) $(CFLAGS) bankingServer.c -o bankingServer -lm -lpthread  
	
bankingClient : bankingClient.c bankingClient.h

	$(CC) $(CFLAGS) bankingClient.c -o bankingClient -lm -lpthread  


clean :
	
	rm -f bankingServer bankingClient

