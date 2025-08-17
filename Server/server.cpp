/*
🔧 Purpose:
This file demonstrates how to implement a basic server in C++ using QNX's native
Interprocess Communication (IPC) system. It uses a named channel (name_attach)
to allow clients to send synchronous messages and receive replies.

🧠 Key Concepts Demonstrated:

Creating a message-receiving channel via ChannelCreate()

Attaching a name to the channel using name_attach() so clients can locate it

Receiving client messages using MsgReceive()

Sending synchronous replies using MsgReply()

Clean termination using "shutdown" message

Using QNX-specific headers and system calls for IPC

🗣️ Interaction Flow:

The server creates a channel and registers a name ("qnx_server").
It enters a loop, waiting to receive messages from any client.
Upon receiving a message:

If the message is "shutdown", it sends a "terminate" reply and shuts down.
Otherwise, it replies with "ack" and continues serving.
On shutdown, it detaches the named channel and destroys the channel.
*/
#include <iostream>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <sys/dispatch.h>
#include <unistd.h>
#include <string.h>

#define SERVER_NAME "qnx_server"

struct Message {
    int type;
    char data[256];
};

int main() {
    int chid = ChannelCreate(0);
    if (chid == -1) {
        perror("ChannelCreate");
        return 1;
    }

    std::cout << "[Server] CHID: " << chid << ", PID: " << getpid() << std::endl;

    // Option 2: Register name
    name_attach_t* attach = name_attach(NULL, SERVER_NAME, 0);
    if (!attach) {
        perror("name_attach");
        return 1;
    }

    while (true) {
        Message msg;
        int rcvid = MsgReceive(chid, &msg, sizeof(msg), NULL);
        if (rcvid == -1) {
            perror("MsgReceive");
            break;
        }

        std::cout << "[Server] Received: " << msg.data << std::endl;

        if (strcmp(msg.data, "shutdown") == 0) {
            strcpy(msg.data, "terminate");
            MsgReply(rcvid, 0, &msg, sizeof(msg));
            std::cout << "[Server] Shutdown signal received. Destroying channel.\n";
            break;
        } else {
            strcpy(msg.data, "ack");
            MsgReply(rcvid, 0, &msg, sizeof(msg));
        }
    }

    name_detach(attach, 0);  // For option 2
    ChannelDestroy(chid);    // Clean up
    return 0;
}
