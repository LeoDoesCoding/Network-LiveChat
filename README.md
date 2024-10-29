# Network-LiveChat
Establishing sockets code from Nicholas Day. Remaining code is original.

## Currently present:
 - Establishes server/client(s) connection.
 - All server/client connections sets up a new user.
 - User(s) can send messages to server.
 - User(s) can recieve messages from other users (through server).
 - Messages do not have an upper cap.
 - If user enters "end", the connection is terminated for all users.
 - Connection error checking and feedback.
 
## Todo:
 - config (to change name)
	- Either client or host keeps record of messages so that they return after beign in config
 - Testing + cleanup