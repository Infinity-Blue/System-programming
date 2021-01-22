#include "bankingServer.h"

pthread_mutex_t mutex_lock;
sem_t bin_sem;
Thread_List	thread_list;
Socket_List	socket_list;
int tcount;
int scount;
Account_Info **account_info;
int	account_cnt;
int semaphore_value;
void *thrfunc(void *data);
void catch_interrupt_function();

void updateSocket(int socket_id, int mode);
void updateThread(pthread_t thread_id, int mode);

void (*old_fun)(int);
void displayAllaccount();
void *itimer_interrupt_event_thread();

int main(argc, argv)
int	argc;
char *argv[];
{
   	int	server_socket;
   	int client_socket;
   	int client_addr_size;

   	struct sockaddr_in   server_addr;
   	struct sockaddr_in   client_addr;

   	int	port_no, status; 
   	pthread_t	thread_id;
   	
   	sigset_t	sigset;
       tcount=0;
	scount=0;   
/*
Argument Checking
*/
   	if (argc != 2)
   	{
		fprintf(stderr, "Should be bankingServer Port_no\n");
		return (-1);   		
   	}
/*
Port_no Checking
*/
   	port_no = atoi(argv[1]);
   	if (port_no == 0)
   	{
		fprintf(stderr, "the Port_no error\n");
		return (-1);    		
   	}

/*
Semaphore Initialization 
*/
	status = sem_init(&bin_sem, 0, 0);
	if (status != 0)
    {
       	fprintf(stderr, "sem_init error: %s\n", strerror(status));
       	return(-1);
    }	
/*
Timer 
*/
	signal(SIGINT, SIG_IGN);
	signal(SIGALRM, SIG_IGN);
		
	sigemptyset(&sigset);							// Delete (make empty) all variables of Signal Group
//	sigaddset(&sigset, SIGINT);					// Add SIGINT into the Signal Group
	sigaddset(&sigset, SIGALRM);				// Add SIGALRM into the Signal Group
	
//	sigprocmask(SIG_BLOCK, &sigset, NULL);					// Block the Signals Set (SIGALRM & SIGINT) of the sigset
	status = pthread_sigmask(SIG_BLOCK, &sigset, NULL);		// Block the Signals Set (SIGALRM & SIGINT) of the sigset from this main thread
	if (status != 0)
    {
       	fprintf(stderr, "thread sigmask block error: %s\n", strerror(status));
       	return(-1);
    }	    
/*	
	status = pthread_create(&thread_id, NULL, itimer_interrupt_event_thread, NULL);
	if (status != 0)
    {
       	fprintf(stderr, "itimer_int_event_thread create error: %s\n", strerror(status));
       	return(-1);
    }	           
    
	updateThread(thread_id, 1);
*/
/*
Signal SIGINT Handler Setting 
*/
	old_fun = signal(SIGINT, catch_interrupt_function);
	if (old_fun == SIG_ERR)
	{
		fprintf(stderr, "An error occurred while settinh a signal handler: %s\n", strerror(errno));
		return (-1);
	}

/*
Socket 
*/
   	server_socket  = socket(PF_INET, SOCK_STREAM, 0);	// PF_INET ("IPv4" Internet Protocol), SOCK_STREAM (TCP/IP protocol)
   	if(server_socket == -1)
   	{
		fprintf(stderr, "creat the server socket error: %s\n", strerror(errno));
		return (-1);
   	}	
/*
Socket information setting
*/
   	memset(&server_addr, 0, sizeof( server_addr));
   	server_addr.sin_family     = AF_INET;
   	server_addr.sin_port       = htons(port_no);
   	server_addr.sin_addr.s_addr= htonl(INADDR_ANY);
/*
Binding with the Socket Information
*/
   	if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
   	{
		fprintf(stderr, "bind the server socket: %s\n", strerror(errno));
		return (-1);
   	}
   	
/*
Thread Mutex Initialization
*/  	
	if(pthread_mutex_init(&mutex_lock, NULL) != 0)										// Initialize the Mutex
	{
		fprintf(stderr, "pthread mutex init error: %s\n", strerror(errno));
		return (-1);
	}

/*
Initialization
*/  	
	int i;
	memset((char *)&thread_list, 0, sizeof (Thread_List));
	memset((char *)&socket_list, 0, sizeof (Socket_List));
	thread_list.thread_cnt = 0;
	socket_list.socket_cnt = 0;
	for (i = 0; i < MAX_SESSION; i++)
	{
		thread_list.flag[i] = 0;
		socket_list.flag[i] = 0;
	}
	account_cnt = 0;

	status = pthread_create(&thread_id, NULL, itimer_interrupt_event_thread, NULL);
        if (status != 0)
    {
     	fprintf(stderr, "itimer_int_event_thread create error: %s\n", strerror(status));
        return(-1);
    }

     	updateThread(thread_id, 1);
	
   	while(1)
   	{
/*
Listen
*/ 
    	if(listen(server_socket, LISTENQ) == -1)
    	{
			fprintf(stderr, "listen Error: %s\n", strerror(errno));
			return (-1);
      	}
/*
Accept
*/ 
      	client_addr_size  = sizeof(client_addr);
      	client_socket     = accept(server_socket, (struct sockaddr*)&client_addr, (socklen_t*)&client_addr_size);
      	if (client_socket == -1)
      	{
 			fprintf(stderr, "accept error: %s\n", strerror(errno));
         	return(-1);
      	}

		updateSocket(client_socket, 1);
      	
        /*≈¨∂Û¿Ãæ∆Æ ∏∂¥Ÿ ªı∑ŒøÓ æ≤∑πµÂ∏¶ ª˝º∫«—¥Ÿ.
          «ÿ¥Á ∫Œ∫–ø°º≠¥¬ ªı∑ŒøÓ æ≤∑πµÂ∏¶ ª˝º∫«œ¥¬ ƒ⁄µÂ∏∏ ¿€º∫«œ∞Ì,  ¡æ∑· ƒ⁄µÂ∏¶ ¿˚øÎ«œ¡ˆ æ ¥¬¥Ÿ.
          ∏∏æ‡ ±‚¡∏ øπ¡¶ ƒ⁄µÂø° ¿÷¥¯ æ≤∑πµÂ ¡æ∑· ¥Î±‚ƒ⁄µÂ∏¶ ¿€º∫«œ∞‘ µ«∏È
          1∏Ì¿« ªÁøÎ¿⁄ æ≤∑πµÂ∞° ¡æ∑·µ«±‚ ¿¸±Ó¡ˆ¥¬ ªı∑ŒøÓ ªÁøÎ¿⁄∞° æ≤∑πµÂ∑Œ ¡¢±Ÿ«œø© 
          ¿€æ˜¿ª ¡¯«‡ «“ ºˆ æ¯¥Ÿ. ±◊∑∏±‚ø° self æ≤∑πµÂ ¡æ∑·∏¶ ªÁøÎ «‘ */
/*
Thread Create
*/          
		status = pthread_create(&thread_id, NULL, thrfunc, (void*)&client_socket);
		if (status != 0)
    	{
        	fprintf(stderr, "thread create error: %s\n", strerror(status));
        	return(-1);
    	}	           
    	
		updateThread(thread_id, 1);
   
   }
}

/*
Timer and SIGINT event thread
*/
void *itimer_interrupt_event_thread()
{
	sigset_t	sigset;
	struct itimerval	itimer;
	
	int	sig_no = 0;
	int	status;
//	pthread_t thread_id;
#ifdef	DEBUG
//printf("itimer_int_event_thread start\n");
#endif

//	pthread_detach(pthread_self());
	
/*
Block alarm & interrupt signals, will be waited on explicitly
*/
	sigemptyset(&sigset);							// Delete (make empty) all variables of Signal Group
	sigaddset(&sigset, SIGINT);					// Add SIGINT into the Signal Group
	sigaddset(&sigset, SIGALRM);				// Add SIGALRM into the Signal Group
	
/*
Un block
*/
//	sigprocmask(SIG_BLOCK, &sigset, NULL);					// Block the Signals Set (SIGALRM & SIGINT) of the sigset
	status = pthread_sigmask(SIG_UNBLOCK, &sigset, NULL);		// Un_Block the Signals Set (SIGALRM & SIGINT) of the sigset from this main thread
	if (status != 0)
    {
	fprintf(stderr, "thread sigmask unblock error: %s\n", strerror(status));
       	return 0;
    }	   

/*
Set up Periodic Interrupt Timer
*/		
	itimer.it_value.tv_sec = 20;
	itimer.it_value.tv_usec = 0;		// 15 sec
	itimer.it_interval = itimer.it_value;	
//	itimer.it_interval.tv_sec = 0;
//        itimer.it_interval.tv_usec=15000000;
	if(setitimer(ITIMER_REAL, &itimer, NULL) != 0)
	{
		perror("setitimer error: ");
        return 0;
	}
	
/*
Wait signal forever
*/
	while (1)
	{
		if (sigwait(&sigset, &sig_no) != 0)
		{
			fprintf(stderr, "sigwait error: %s\n", strerror(status));
		}
		
		switch(sig_no)
		{
			case SIGALRM:
#ifdef	DEBUG
//printf("SIGALRM\n");
#endif
            displayAllaccount();
/*            status = pthread_create(&thread_id, NULL, displayAllaccount, NULL);
            if (status != 0)
             {
                 fprintf(stderr, "thread create error: %s\n", strerror(status));
          //       return(-1);
             }
*/   
            break;
/*	    case SIGINT:
#ifdef	DEBUG
//printf("SIGINT\n");
#endif
				catch_interrupt_function();
				break;
*/
			default:
				fprintf(stderr, "Unexpected signal (Signal No. is %d)\n", sig_no);	
		}
	}
}


void *thrfunc(void *data)
{//       int ret;
	int accp_sock = (int) *((int*)data);
	
	char buf_rcv[BUF_SIZE];
	char buf_snd[BUF_SIZE];
        int semaphore_cnt;
	int	i;
	char *ptr;
	double	balance;
	int	in_service_session;
	
//	pthread_detach(pthread_self());
	printf("connection to client is successful\n");
	sprintf(buf_snd, M_Accept);
	write(accp_sock, buf_snd, strlen(buf_snd)+1); 
		
	in_service_session = -1;
	
	while(1)
	{
		memset(buf_rcv, 0, BUF_SIZE);
		memset(buf_snd, 0, BUF_SIZE);
                semaphore_cnt = 0;
/*
Read text from a client
*/
int n_red;
 n_red = read (accp_sock, buf_rcv, BUF_SIZE);
       if (n_red > 0)
       {

//  if(read (accp_sock, buf_rcv, BUF_SIZE)>0)

     
		ptr = strtok(buf_rcv, " ");

/*
Create Account
*/		
		if (!strcmp(ptr, D_Create))			
		{
			if (in_service_session != -1)			// In case the client is in a service session
			{       
			//	sprintf(buf_snd, M_Client_In_Service);
				sprintf(buf_snd, "error: create command is not valid in account session");
				write(accp_sock, buf_snd, strlen(buf_snd)+1); 
			}
			else									// In case the client is NOT in a service session, it is possible to create new account
			{
				ptr = strtok(NULL, " ");			// Read the account_name
				if (ptr != NULL)
				{
					
					while(1)						// is not able to creat an account until the semaphore 
					{
						sem_getvalue(&bin_sem, &semaphore_value);
#ifdef DEBUG
//                                       printf("sem_getvalue in thr_fuc is %d\n", semaphore_value);
#endif

						if (semaphore_value == 0)
							break;
                                        else
			                  {
                    		 if (semaphore_cnt == 0)
                    		 {
                      			  sprintf(buf_snd, M_In_Diagnostic);
                       			  write(accp_sock, buf_snd, strlen(buf_snd)+1); 
                      			  memset(buf_snd, 0, BUF_SIZE);
                    		 }
                  			   semaphore_cnt++;
                 		         }

					}
					
	    			pthread_mutex_lock(&mutex_lock);
	    			for (i = 0; i < account_cnt; i++)
	    			{
	    				if (!strcmp(ptr, account_info[i]->name))
	    				{
							break;
						}
					}
					if (i == account_cnt)			// there are not accounts with the name inputted
					{					
						if (account_cnt == 0)	
							account_info = (Account_Info **) malloc (sizeof(Account_Info));
						else
						{
							account_info = (Account_Info **) realloc ((void *)account_info, sizeof(Account_Info) * (account_cnt+1));
						}
						
						if (account_info == NULL)	// in case of memory allocation error
	   					{	
	    					sprintf(buf_snd, M_Memory_Allocation_Error);
							write(accp_sock, buf_snd, strlen(buf_snd)+1); 
	   					}	
						else
						{
	   						account_info[account_cnt] = (Account_Info *) malloc (sizeof(Account_Info));
	   						if (account_info[account_cnt] == NULL)	// in case of memory allocation error
	   						{
	    						sprintf(buf_snd, M_Memory_Allocation_Error);
								write(accp_sock, buf_snd, strlen(buf_snd)+1); 
							}
							else					// create account
							{
								memset(account_info[account_cnt], 0, sizeof(Account_Info));
								strcpy(account_info[account_cnt]->name, ptr);
								account_info[account_cnt]->name[sizeof(ptr)] = 0;
								account_info[account_cnt]->balance = 0;
								account_info[account_cnt]->flag = NOT_SERVICE;
								account_cnt++;
								
	    						sprintf(buf_snd, M_Account_Created);
								write(accp_sock, buf_snd, strlen(buf_snd)+1); 
							}					
						}
					}
					else							// in case there is already the account with the name inputted
					{
	    				sprintf(buf_snd, M_Account_Exist);
						write(accp_sock, buf_snd, strlen(buf_snd)+1); 
					}
	    			pthread_mutex_unlock(&mutex_lock);
				}
				else								// In case the account name is not inputted
				{
	    			sprintf(buf_snd, M_Account_Not_Defined);
					write(accp_sock, buf_snd, strlen(buf_snd)+1); 
				}
			}
		}
/*
Make the account in_service
*/	
		else if (!strcmp(ptr, D_Serve))
		{
			if (in_service_session != -1)			// In case the client is in a service session
			{
			//	sprintf(buf_snd, M_Client_In_Service);
				sprintf(buf_snd, "error: serve command is not valid in account session");
				write(accp_sock, buf_snd, strlen(buf_snd)+1); 
			}
			else									// In case the client is NOT in a service session, it is possible to make the client in_service
			{    
				ptr = strtok(NULL, " ");			// Read the account_name
//                             printf("Sever serves account: %s\n", ptr);
				if (ptr != NULL)
				{
	    			pthread_mutex_lock(&mutex_lock);
	    			for (i = 0; i < account_cnt; i++)
	    			{
	    				if (!strcmp(ptr, account_info[i]->name))
	    				{
							break;
						}
					}
					if (i == account_cnt)			// the account is not exist
					{				
	    				sprintf(buf_snd, M_Account_No_Exist);
						write(accp_sock, buf_snd, strlen(buf_snd)+1); 	
					}
					else							// the account is exist
					{
						if (account_info[i]->flag == NOT_SERVICE)		// Make the account in_service
						{
							account_info[i]->flag = IN_SERVICE;
							in_service_session = i;
							
	    				//	sprintf(buf_snd, M_Account_Served);
						printf("Sever serves account: %s\n", ptr);
						sprintf(buf_snd,"Server starts a service session for account: %s", ptr);
							write(accp_sock, buf_snd, strlen(buf_snd)+1); 						
						}
						else						// the account is already in_service
						{
	    					sprintf(buf_snd, M_Account_In_Service);
							write(accp_sock, buf_snd, strlen(buf_snd)+1); 
						}
					}
	    			pthread_mutex_unlock(&mutex_lock);
				}
				else								// In case the account name is not inputted
				{
	    			sprintf(buf_snd, M_Account_Not_Defined);
					write(accp_sock, buf_snd, strlen(buf_snd)+1); 
				}
			}	
		}
/*
Deposit into the account
*/	
		else if (!strcmp(ptr, D_Deposit))
		{
			if (in_service_session == -1)			// The accunt must be in_servie to deposit. In case the account is NOT in_service
			{
			//	sprintf(buf_snd, M_Client_Not_In_Service);
				sprintf(buf_snd, "error: deposit command is only valid in account session");
				write(accp_sock, buf_snd, strlen(buf_snd)+1); 
			}
			else									// In case the account is in_service
			{
				ptr = strtok(NULL, " ");			// Read the amount of money to deposit
				if (ptr != NULL)
				{
	    			pthread_mutex_lock(&mutex_lock);
					balance = atof(ptr);
					if (balance > 0)				// Deposit
					{
						account_info[in_service_session]->balance += balance;
						
	    				sprintf(buf_snd, M_Account_Deposited);
						write(accp_sock, buf_snd, strlen(buf_snd)+1); 	
					}
	    			pthread_mutex_unlock(&mutex_lock);
				}
				else								// In case the aomunt of the money is not inputted
				{
	    			sprintf(buf_snd, M_Amount_Not_Defined);
					write(accp_sock, buf_snd, strlen(buf_snd)+1); 
				}
			}
		}
/*
Withdraw from the account
*/	
		else if (!strcmp(ptr, D_Withdraw))
		{
			if (in_service_session == -1)			// The accunt must be in_servie to withdraw. In case the account is NOT in_service
			{
			//	sprintf(buf_snd, M_Client_Not_In_Service);
				sprintf(buf_snd, "error: withdraw command is only valid in account session");
				write(accp_sock, buf_snd, strlen(buf_snd)+1); 
			}
			else									// In case the account is in_service
			{
				ptr = strtok(NULL, " ");			// Read the amount of money to deposit
				if (ptr != NULL)
				{
	    			pthread_mutex_lock(&mutex_lock);
					balance = atof(ptr);
					if (balance > 0)
					{
						if (account_info[in_service_session]->balance >= balance)	// Withdraw
						{
							account_info[in_service_session]->balance -= balance;
							
		    				sprintf(buf_snd, M_Account_Withdrawed);
							write(accp_sock, buf_snd, strlen(buf_snd)+1);
						}
						else						// In case the requested amount exceeds the current balance
						{
		    				sprintf(buf_snd, M_Account_Withdraw_Too_Much);
							write(accp_sock, buf_snd, strlen(buf_snd)+1);
						}
					}
	    			pthread_mutex_unlock(&mutex_lock);
				}
				else								// In case the aomunt of the money is not inputted
				{
	    			sprintf(buf_snd, M_Amount_Not_Defined);
					write(accp_sock, buf_snd, strlen(buf_snd)+1); 
				}
			}
		}
/*
Display the balance of the account
*/
		else if (!strcmp(ptr, D_Query))	
		{
			if (in_service_session == -1)			// The accunt must be in_servie to query. In case the account is NOT in_service
			{
			//	sprintf(buf_snd, M_Client_Not_In_Service);
				sprintf(buf_snd, "error: query command is only valid in account session");
				write(accp_sock, buf_snd, strlen(buf_snd)+1); 
			}
			else
			{
				pthread_mutex_lock(&mutex_lock);
				
	    		sprintf(buf_snd, "The current account balace is %f", account_info[in_service_session]->balance);
				write(accp_sock, buf_snd, strlen(buf_snd)+1); 
				
				pthread_mutex_unlock(&mutex_lock);
			}
		}
/*
Make the account not_in_service
*/
		else if (!strcmp(ptr, D_End))
		{
			if (in_service_session == -1)			// The accunt must be in_servie. In case the account is NOT in_service
			{
			 //	sprintf(buf_snd, M_Client_Not_In_Service);
                                sprintf(buf_snd, "error: end command is only valid in account session");
				write(accp_sock, buf_snd, strlen(buf_snd)+1); 
			}
			else
			{
				pthread_mutex_lock(&mutex_lock);
				
				account_info[in_service_session]->flag = NOT_SERVICE;
				in_service_session = -1;
	    		sprintf(buf_snd, M_Account_Ended);
				write(accp_sock, buf_snd, strlen(buf_snd)+1); 
				
				pthread_mutex_unlock(&mutex_lock);
			}	
		}
/*
Disconnect
*/
		else if (!strcmp(ptr, D_Quit))
		{       
			if (in_service_session != -1)
			{
				sprintf(buf_snd, M_Quit_Error);
				write(accp_sock, buf_snd, strlen(buf_snd)+1);
			}			
			else 
			{
			sprintf(buf_snd, M_Server_Disconnect);
			write(accp_sock, buf_snd, strlen(buf_snd)+1);
			 
			close(accp_sock);	
			
			updateSocket(accp_sock, 0);
			updateThread(pthread_self(), 0);
			 pthread_detach(pthread_self());
			return ((void*)NULL);
			}
		}
/*
Commend Error
*/
      else
      {
         sprintf(buf_snd, M_Commend_Error);
         write(accp_sock, buf_snd, strlen(buf_snd)+1);
      }
}
	}
//exit(0);
}

void catch_interrupt_function()
{
	struct itimerval	itimer;
	char buf_snd[BUF_SIZE];
	int i; 
        int status;
        int ret;	
	pthread_mutex_lock(&mutex_lock);
/*
Stop timer
*/
	itimer.it_value.tv_sec = 0;
	itimer.it_value.tv_usec = 0;
	itimer.it_interval = itimer.it_value;
	
//	fprintf(stdout, "\nStop the timer\n");
	if(setitimer(ITIMER_REAL, &itimer, NULL) != 0)
	{
//		fprintf(stderr, "setitimer stop error: %s\n", strerror(status));
          perror("setitimer stop error: "); 
          return;
	}
/*
Lock all the account
*/
  //    fprintf(stdout, "Lock all the account\n");
	for (i = 0; i < account_cnt; i++)
	{
		account_info[i]->flag = NOT_SERVICE;
	}
//fprintf(stdout, "Deallocate the memory\n");
/*
Deallocate the memory
*/	
	for (i = 0; i < account_cnt; i++)
	{
		free(account_info[i]);
	}
	free(account_info);

//fprintf(stdout, "Send all clients a shutdown message and close the client\n");	
/*
Send all client a shutdown message and close the client
*/
	for (i = 0; i < socket_list.socket_cnt; i++)
	{ 
		if (socket_list.flag[i] == VALID)
		{       
			sprintf(buf_snd, M_Shutdown);
			write(socket_list.id_socket[i], buf_snd, strlen(buf_snd)+1);
		}
	}
/*
Join all the threads
*/
  // fprintf(stdout, "Join all the threads\n");
   for (i = 0; i < thread_list.thread_cnt && i < MAX_SESSION; i++)
   {
      if (thread_list.flag[i] == VALID)
      {

         pthread_cancel(thread_list.p_thread[i]);                  // thermination of thread

         ret = pthread_join(thread_list.p_thread[i], (void **)&status);   // deallocation of resoureces which the thread used
         if (ret != 0)
          {
              fprintf(stderr, "thread join error : %s\n", strerror(errno));
          }
     }
   }

/*
Close all sockets
*/
  // fprintf(stdout, "Close all sockets\n");

	for (i = 0; i < socket_list.socket_cnt; i++)
	{
		if (socket_list.flag[i] == VALID)
		{
			close(socket_list.id_socket[i]);
		}
	}

	pthread_mutex_unlock(&mutex_lock);
//	signal(SIGINT, old_fun);

/*
Semaphore Destory 
*/
//fprintf(stdout, "Semaphore Destory \n");
	status = sem_destroy(&bin_sem);
	if (status != 0)
    {
       	fprintf(stderr, "sem_destory error: %s\n", strerror(status));
 	return;
    }
fprintf(stdout, "Shutdown Completed \n");
	exit(0);
}

/*
Update Socket_id list
*/	
void updateSocket(int socket_id, int mode)
{
	int i;
	
	pthread_mutex_lock(&mutex_lock);
	scount++;
	if (mode == 1)		// add the socket id
	{
		for (i = 0; (i < MAX_SESSION) && (i < scount); i++)
		{
			if (socket_list.flag[i] == INVALID)		// invalid 
			{
				socket_list.id_socket[i] = socket_id;
				socket_list.flag[i] = VALID;		// valid
				socket_list.socket_cnt++;
			}
		}	
	}
	else				// delete the socket id
	{ 
		for (i = 0; i < MAX_SESSION; i++)
		{
			if (socket_list.id_socket[i] == socket_id && socket_list.flag[i] == VALID)
			{
				socket_list.flag[i] = INVALID;		// invalid
				socket_list.socket_cnt--;
				if (socket_list.socket_cnt < 0)
					socket_list.socket_cnt = 0;
			}
		}	
	}
	
	pthread_mutex_unlock(&mutex_lock);
}

/*
Update Thread_id list
*/
void updateThread(pthread_t thread_id, int mode)
{
	int i;
	pthread_mutex_lock(&mutex_lock);
	tcount++;
	if (mode == 1)		// add the thread id
	{
		for (i = 0; (i < MAX_SESSION) && (i<tcount); i++)
		{  
#ifdef DEBUG
// printf("thread list id of index %d: %lu\n",i,(unsigned long)thread_list.p_thread[i]);
#endif
			if (thread_list.flag[i] == INVALID)		// invalid 
			{        
				thread_list.p_thread[i] = thread_id;
			//	printf("thread id in updateThread of index %d: %lu\n",i,(unsigned long)thread_list.p_thread[i]);  
				thread_list.flag[i] = VALID;		// valid
#ifdef DEBUG
//				printf("thread flag of index %d :%d\n", i, thread_list.flag[i]);
#endif
				thread_list.thread_cnt++;
			}
		}	
	}
	else				// delete the thread id
	{  
		for (i = 0; i < MAX_SESSION; i++)
		{
			if (thread_list.p_thread[i] == thread_id && thread_list.flag[i] == VALID)
			{
				thread_list.flag[i] = INVALID;		// invalid
				thread_list.thread_cnt--;
				if (thread_list.thread_cnt < 0)
					thread_list.thread_cnt = 0;
			}
		}	
	}
	
	pthread_mutex_unlock(&mutex_lock);
}

/*
Display all account information
*/
void displayAllaccount()
{
	int i;
//	int retval=0;
//	pthread_mutex_lock(&mutex_lock);
	sem_post(&bin_sem);
        sleep(5);	
	for(i = 0; i < account_cnt; i++)
	{
		if (account_info[i]->flag == IN_SERVICE)
			printf("%s\t%f\tIN SERVICE\n", account_info[i]->name, account_info[i]->balance);
		else
			printf("%s\t%f\n", account_info[i]->name, account_info[i]->balance);
	}
	
//	pthread_mutex_unlock(&mutex_lock);
	sem_wait(&bin_sem);
}
