/* CSci4061 S2016 Assignment 2
* section: 1
* section: 5
* date: 03/11/16
* name: Yuanhao Lu, Phoebe Zhang, Peter Nguyen (for partner(s))
* id: luxx0267, zhan2937, nguy1435 */

Purpose: 
        - The purpose of this program is to implement a local chat application by using IPC.

Required Files:
        - server.c
	- shell.h
	- util.c
	- util.h

Compile Instructions:
	- Unzip tar file if needed
	- run UNIX system command make from shell	

Usage:
	- run UNIX system command ./server to start the server.
	- use \add <username> in server shell to add a new user
	- use \list in server or user shell to print the list of user names.
	- use \kick <username> in server shell to kick a specific user out.
	- use \p2p <username> <message> in user shell to send p2p message to a specificc user.
	- use \exit in server shell to cleanup all the users and exit, or use it in a user shell to exit the user.
	- use \seg in user shell to create a segmentation fault in the user shell.
	- any other text in server shell will be broadcast message, and any other text in user shell will be sent to other users and printed out on their shells.
	- any invalid commands for either the server or user will print an error message to the server shell.

Assumptions:
	- max number of users will not exceed 10 not including the server.
	- message size less than 1024 bytes including the command string.
	- users will not have the same usernames.
		- will still work, but commands will be applied to the first registered user with that username.
	- server and user should not be using the command \child_pid to set the child_pid.
	- after using \p2p <username>, the message will be followed by a space, if not the server will not reconized who to send the message to.
	

Strategies for error handling:
	- prints an error message for any failed system calls.
	- exit program on failures for fork and opening, reading and writing to pipes
	- prints an error message for any illegal commands.
	
Other Instructions/ Notes:
	- commands will work as long as the command is entered correctly from start of command string. 
		(ie: \adddd <username> will work but \aadd <username> will not work)
	- wrongly typed commands will be treated as messages to be broadcast.
	- closing the server terminal, via "x" button will not close out the user's xterm windows.
