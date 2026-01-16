# Dev Log:

This document must be updated daily every time you finish a work session.

## Brian Zheng

### 2026-01-06 - Initial Stuff
Update devlog file names, edit the README. (5 min)
Learned and experimented with ncurses in main.c (50 min)

### 2026-01-07 - Researching
Update the PROPOSAL with using select (5 min)
Spent time understanding the select sample code and how it works (15 min)
More ncurses researching (40 min)

### 2026-01-08 - Start Server Logic
Write the makefile (5 min)
Write server_setup and server and client handshake functions in networking.c (15 min)
Start the select logic in server.c (20 min)
Create the message struct and setup using shared memory for messages (15 min)

### 2026-01-09 - Continue Server Logic
Scrapped using shared memory, use select to connect to a client if listen_socket has something to read. If a client has something to read, read that message and send it to all other clients. (30 min)

Start select logic for the client, if stdin has something to read, read that message and write it to the server. (20 min)

### 2026-01-10 - Finish client select logic
Finish client select logic for reading from server (15 min)
Add the username and the current local time to the message sent to the server (15 min)
Test client (doesn't work) and try to debug (25 min)

### 2026-01-11 - MVP Works
Stop using struct messages, got the basic chat working (40 min)

### 2026-01-12 - Connecting from other computers
Added connecting to the server from a client from a different computer (20 min)
Fix warnings with message size (15 min)

### 2026-01-13 - Clean client exiting, debugging
Cleanly have client close and exit when server close or ctrlC on client (20 min)
Debug server trying to read from first client when nothing to read (45 min)

### 2026-01-14 - Finish debugging, sighandler
Use sighandler for better exiting on control C (15 min)
Fix previous bug (30 min)

### 2026-01-15 - Redraw windows, arrow keys
Redraw the input, user, and chat windows after getch(), otherwise some are not displayed (20 min)
Try to implement using arrow keys, couldn't get it working (30 min)
