# Network-LiveChat
Establishing sockets code from Nicholas Day. Remaining code is original.

## Currently present:
 - Establishes server/client(s) connection.
 - All server/client connections sets up a new user.
 - User(s) can send messages to server.
 - User(s) can recieve messages from other users (through server).
 - Messages do not have an upper cap.
 - User can enter "end" to end their connection to the sever (same as closing application)
 - Connection error checking and feedback.
 - Input checking:
	- Invalid connect option
	- Invalid IP format
 - Connection handling:
	Client:
		- Winsoc, socket and IP/port connection handling
		- Handles server connection loss
	Server:
		- Winsoc, socket and IP/port connection handling
		- Handles expected user disconnect (ie, recieves "end")
		- Handles unexpected user disconnect
 
## Todo:
 - config (to change name)
	- Either client or host keeps record of messages so that they return after being in config
 Server:
	- Handling (and rejecting) connections not via the application
	- (Deppendant on feesability) run chat sepperate from connection log
 - Testing + cleanup