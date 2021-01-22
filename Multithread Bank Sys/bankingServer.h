/*****
*
******/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>

#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <semaphore.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define	DEBUG					0
#define	D_Create					"create"
#define	D_Serve						"serve"
#define	D_Deposit					"deposit"
#define	D_Withdraw					"withdraw"
#define	D_Query						"query"
#define	D_End						"end"
#define	D_Quit						"quit"
#define LISTENQ					100
#define	M_Accept					"Connection to server is successful"
#define	M_Client_In_Service			"Client is in a service session"
#define	M_Client_Not_In_Service		"Client is not in a service session"
#define	M_Server_Disconnect			"This client is disconnected from the server"
#define	M_Account_Exist				"The Account already exists"
#define	M_Account_No_Exist			"The Account does not exist"
#define	M_Account_In_Service		"The Account is already in service"
#define	M_Account_Not_Defined		"The Account name must be entered"
#define	M_Amount_Not_Defined		"The Amount of the money to deposit or withdraw must be entered"
#define	M_Account_Created			"The Account is created"
#define	M_Account_Served			"The Account is in serviced"
#define	M_Account_Deposited			"The Amount of the money is deposited sucessfully"
#define	M_Account_Withdrawed		"The Amount of the money is withdrawed sucessfully"
#define	M_Account_Ended				"The Service session is ended"
#define	M_Account_Withdraw_Too_Much	"The requested amount exceeds the current balance"
#define	M_Memory_Allocation_Error	"Memory Allocation Error"
#define	M_Shutdown					"Server shutdowns by SIGINT"
#define M_Commend_Error            "Invalid command"
#define M_In_Diagnostic            "Diagnostic Program is Running. Please wait a little bit..."
#define M_Quit_Error		   "Must end session before quit"
#define	BUF_SIZE		512
#define	MAX_SESSION		1024

#define	NOT_SERVICE		0
#define	IN_SERVICE		1

#define	INVALID			0
#define	VALID			1

typedef struct
{
	char	name[255];
	double	balance;
	int		flag;
} Account_Info;

typedef struct
{   
	int     thread_cnt;
    pthread_t	p_thread[MAX_SESSION];
    int		flag[MAX_SESSION];
} Thread_List;
typedef struct
{   
	int     socket_cnt;
    int		id_socket[MAX_SESSION];
    int		flag[MAX_SESSION];
} Socket_List;

