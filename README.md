# Network-LiveChat
Establishing sockets code from Nicholas Day. Remaining code is original.

## Currently present:
 - Server/client connection when both applications are running.
 - Client can send messages to server, which are then sent back.
     - This is done with threads, in preparation for multiple clients but is not yet equipped to handle multiple clients.
 - If client enters "end", the connection is terminated.
 - Connection error checking and feedback.
 
## Todo:
 - Re-structure server code (run connection as static)
 - Test multiple clients.
 - Try elimination of client code OR add GUI.
 - Testing.
