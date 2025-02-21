# Network-LiveChat
Console application allowing for users to communicate over TCP connection.

Establishing sockets code from Nicholas Day. Remaining code is original.

## Currently present:
 - Establishinng server/client(s) connection.
 - Each connecting server/client sets up a new user. Host is both a user and server.
 - User(s) can send messages to server.
 - User(s) can recieve messages from other users (through server).
 - Messages have an upper cap of 2 bytes.
 - Commands:
	- /end: Severs the user's connection.
	- /config: Enters configuration mode.
 - Config mode:
	- Name: User can change their name.
	- Server setting:
		- Log mode: Currently inaffective. See: todo.
	- Users will not be able to send or view messages sent while in config mode.
 - Connection error checking and feedback.
 - Input checking:
	- Invalid connect option
	- Invalid IP format
 - Connection handling:
	- Client
		- Winsoc, socket and IP/port connection handling
		- Handles server connection loss
	- Server
		- Winsoc, socket and IP/port connection handling
		- Handles expected user disconnect (ie, recieves "end")
		- Handles unexpected user disconnect
 
## Todo:
 - config
	- Log modes: How/if the chat is stored during a session.
 - Server
	- Handling (and rejecting) connections not via the application
	- (Deppendant on feesability) run chat sepperate from connection log
 - Testing + cleanup
 - GUI?
