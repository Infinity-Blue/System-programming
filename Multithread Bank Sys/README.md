
# Multi-threaded Bank System
## Decription

### bankingClient
Given port number and host name, bankingClient connects to bankingServer. By creating separate threads for reading and sending data, bankingClient continuously communicates with Server until client or server shuts down.
### bankingServer
Given port number, bankingServer listens and accepts incoming client sockets. If the user inputs SIGINT, signal handler stops timer, locks the account, deallocates memory, sends all clients shutdown message, closes clients, joins all threads, destroys semaphore, and closes all sockets. bankingServer creates three main threads: one for dealing with SIGALRM and one for displaying accounts, and the other for reading and sending data to clients.

## How to start
```
./bankingServer <port number>
./bankingClient user@<ip address> <port number>
```
## How to run 
The **command syntax** allows the user to; create accounts, start sessions to serve specific accounts, and to exit the client process altogether:
```
create <accountname (char)> 
serve <accountname (char)> 
deposit <amount (double)>  
withdraw <amount (double)> 
query
end
quit
```
The client process will send commands to the bank, and the bank will send responses back to the client. Every messages the client sends to the server should result in a response from the server; either data, an error or a confirmation message.

## Command syntax details
### Create:
The create command creates a new account for the bank. It is an error if the bank already has an account with the specified name or can not create the account for some reason. A client in a service session (see below) cannot create new accounts, but another client who is not in a customer session can create new accounts. The name specified uniquely identifies the account. 
### Serve:
The serve command starts a service session for a specific account. The deposit, withdraw,
query and end commands are only valid in a service session. It is not possible to start more
than one service session in any single client window, although there can be concurrent service
sessions for different accounts in different client windows.
### Deposit/Withdraw:
The deposit and withdraw commands add and subtract amounts from an account balance.
Amounts are specified as floating-point numbers. Both commands should result in an error if the client is not in a service session. 
### Query:
The query command simply returns the current account balance.
### End:
The end command ends the current service session. Once the service session is ended, it is possible to create new accounts or start a new service session.
### Quit:
The quit command disconnects the client from the server and ends the client process. The server process should continue execution.
