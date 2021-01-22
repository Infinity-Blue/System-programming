
# Multi-threaded Bank System
## Decription

### bankingClient
Given port number and host name, bankingClient connects to bankingServer. By creating separate threads for reading and sending data, bankingClient continuously communicates with Server until client or server shuts down.
### bankingServer
Given port number, bankingServer listens and accepts incoming client sockets. If the user inputs SIGINT, signal handler stops timer, locks the account, deallocates memory, sends all clients shutdown message, closes clients, joins all threads, destroys semaphore, and closes all sockets. bankingServer creates three main threads: one for dealing with SIGALRM and one for displaying accounts, and the other for reading and sending data to clients.
