# Network-LiveChat
Establishing sockets code from Nicholas Day. Remaining code is original.
##Recent commit: Handling messages of varied lengths (transfered message clips at 200).

## Currently present:
 - Establishes server/client(s) connection.
 - Client(s) can send messages to server.
 - Client(s) can recieve messages from other clients (through server).
 - If client enters "end", the connection is terminated for all clients.
 - Connection error checking and feedback.
 
## Todo:
 - Fix issue: Client recieved message has charatcers that shoulden't be present (at beginning of message).
 - Remove message clipping.
 - Try elimination of client code OR add GUI.
 - Testing.
