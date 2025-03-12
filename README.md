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
	- Log mode: User can change whether/how a log is kept of the chat. These include:
		- No log: No log is being kept of the chat. Leaving the chat room or entering opotions mode erases the chat for the user.
		- Temporary log: A local log of chat is kept as a variable. Quitting the chat will erase the chat session.
		- Text-file log: A local log of chat is kept as a text file.
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
	- Exchange data when switching between temporary <--> persistant.
	- Config -> log mode -> server: (client only) requests server for the log when exiting options mode.
 - Server
	- Handling (and rejecting) connections not via the application
	- (Deppendant on feesability) run chat sepperate from connection log
 - Testing + cleanup
 - GUI?
