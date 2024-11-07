# Network-LiveChat
Establishing sockets code from Nicholas Day. Remaining code is original.

## Currently present:
 - Establishes server/client(s) connection.
 - All server/client connections sets up a new user.
 - User(s) can send messages to server.
 - User(s) can recieve messages from other users (through server).
 - Messages do not have an upper cap.
 - User can enter "end" to end their connection to the sever (same as closing application).
 - Connection error checking and feedback.
 - User can enter config mode:
	- User can change their name
	- User can exit config mode with "/quit"
	- User will not be able to view messages while in this mode.
 - User can change their name.
 - Input checking:
	- Invalid connect option
	- Invalid IP format
 - Connection handling:
	Client
		- Winsoc, socket and IP/port connection handling
		- Handles server connection loss
	Server
		- Winsoc, socket and IP/port connection handling
		- Handles expected user disconnect (ie, recieves "end")
		- Handles unexpected user disconnect
 
## Todo:
 - config
	- Log mode (both client and server)
 Server
	- Handling (and rejecting) connections not via the application
	- (Deppendant on feesability) run chat sepperate from connection log
 - Testing + cleanup