#include "bankingClient.h"

pthread_mutex_t mutex_lock;

void * thread_snd(void *data);
void * thread_rcv(void *data);
// int     server_socket;
int main(argc, argv)
int	argc;
char *argv[];
{
//	char *host_name;
   	int	port_no;
   	
	int	server_socket;
   	struct sockaddr_in   server_addr;
   	
   	pthread_t	thread_id1, thread_id2;
   	int status; 
   	
   	struct hostent *he;
   	
/*
Argument Checking
*/
   	if (argc != 3)
   	{
		fprintf(stderr, "Should be bankingClinet Name_of_the_Machine Port_no\n");
		return (-1);   		
   	}
/*
Port_no Checking
*/
   	port_no = atoi(argv[2]);
   	if (port_no == 0)
   	{
		fprintf(stderr, "the Port_no error\n");
		return (-1);    		
   	}

/*
Find out the Server
*/
    if ((he=gethostbyname(argv[1])) == NULL) {  /* get the host info */
		fprintf(stderr, "the host name error\n");
		return (-1);    
    }
/*
Socket 
*/
   	server_socket  = socket(PF_INET, SOCK_STREAM, 0);	// PF_INET ("IPv4" Internet Protocol), SOCK_STREAM (TCP/IP protocol)
   	if(server_socket == -1)
   	{
		fprintf(stderr, "create the server socket error: %s\n", strerror(errno));
		return (-1);
   	}	
/*
Socket information setting
*/
   	memset(&server_addr, 0, sizeof(server_addr));
   	server_addr.sin_family     = AF_INET;
   	server_addr.sin_port       = htons(port_no);
//   	server_addr.sin_addr.s_addr= htonl(INADDR_ANY);
    server_addr.sin_addr = *((struct in_addr *)he->h_addr);
    bzero(&(server_addr.sin_zero), 8);     /* zero the rest of the struct */
//	inet_pton(AF_INET, serverIP, &servaddr.sin_addr);

/*
Connect with the Socket Information
*/
	while(1)
	{
   		if(connect(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
   		{
			fprintf(stderr, "Connect error: %s\n", strerror(errno));
			sleep(3);
   		}
   		else 
   			break;
	}
   	
/*
Thread Mutex Initialization
  	
	if(pthread_mutex_init(&mutex_lock, NULL) != 0)										// Initialize the Mutex
	{
		fprintf(stderr, "pthread mutex init error: %s\n", strerror(errno));
		return (-1);
	}
*/
/*
Initialization
*/  	
	
//   	while(1)
  // 	{
/*
Thread Create
*/          
		status = pthread_create(&thread_id1, NULL, thread_snd, (void*)&server_socket);
		if (status != 0)
    	{
        	perror("thread_snd create error: ");
//fprintf(stderr, "thread_snd create error: %s\n", strerror(status));
        	return (-1);
    	}  
    //	 pthread_join(thread_id1, NULL);
		status = pthread_create(&thread_id2, NULL, thread_rcv, (void*)&server_socket);
		if (status != 0)
    	{
        	fprintf(stderr, "thread_rcv create error: %s\n", strerror(status));
        	return(-1);
    	}            
          pthread_join(thread_id1, NULL);
//	}
}

void * thread_rcv(void *data)
{
	char buf_rcv[BUF_SIZE];
	int accp_sock = (int) *((int*)data);
		
	pthread_detach(pthread_self());

/*
Read text from the server
*/
	while(1)
	{
		memset(buf_rcv, 0, BUF_SIZE);
    	read (accp_sock, buf_rcv, BUF_SIZE);
		printf("%s\n", buf_rcv);
           if (!strcmp(buf_rcv,"Server shutdowns by SIGINT") || !strcmp(buf_rcv,"This client is disconnected from the server"))
		{
	      printf("client closes\n"); 
              close(accp_sock);
             exit(0);
		
		}

	}
}

void * thread_snd(void *data)
{  
	char buf_snd[BUF_SIZE];
	int accp_sock = (int) *((int*)data);
	
//	pthread_detach(pthread_self());

/*
Read commend and send it to the server
*/
	while(1)
	{          
		memset(buf_snd, 0, BUF_SIZE);
	       fgets(buf_snd, BUF_SIZE, stdin);
               buf_snd[strcspn(buf_snd, "\n")]=0;
   	       write (accp_sock, buf_snd, strlen(buf_snd)+1);
                sleep(2);       

#ifdef	DEBUF
//printf("%s\n", buf_snd);
#endif
          
	}
//fflush(stdin);
 // sleep(2000);	

}
