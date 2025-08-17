/*
🔧 Purpose:
This file demonstrates how to implement a client application in C++ that communicates 
with a QNX server using native message passing IPC mechanisms. It connects to the server 
either via PID/Channel ID or by a registered channel name.

🧠 Key Concepts Demonstrated:
- Attaching to a server channel using `ConnectAttach()` with PID and channel ID
- Attaching via server name using `name_open()` (recommended for flexibility)
- Sending synchronous messages to the server with `MsgSend()`
- Receiving and handling replies from the server
- Handling server shutdown signals and clean client termination

🗣️ Interaction Flow:
1. The client attempts to connect to the server:
   - If provided, it uses PID and channel ID (command line arguments).
   - Otherwise, it uses the registered name `"qnx_server"` to connect.
2. It enters a loop prompting the user to enter messages.
3. Each message is sent to the server synchronously with `MsgSend()`.
4. The client waits for the server’s reply and prints it.
5. If the reply is `"terminate"`, the client exits gracefully.
6. On exit, the client detaches from the server channel.
*/
#include <iostream>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/dispatch.h>

#define SERVER_NAME "qnx_server"

struct Message {
    int type;
    char data[256];
};

int main(int argc, char* argv[]) {
    int coid;
    if (argc == 3) {
        // Option 1: Connect via PID + CHID
        int server_pid = atoi(argv[1]);
        int server_chid = atoi(argv[2]);
        coid = ConnectAttach(ND_LOCAL_NODE, server_pid, server_chid, 0, 0);
        if (coid == -1) {
            perror("ConnectAttach");
            return 1;
        }
        std::cout << "[Client] Connected via PID/CHID\n";
    } else {
        // Option 2: Connect via name
        coid = name_open(SERVER_NAME, 0);
        if (coid == -1) {
            perror("name_open");
            return 1;
        }
        std::cout << "[Client] Connected via name_open\n";
    }

    Message msg;
    Message reply;
    while (true) {
        std::cout << "[Client] Enter message (or 'shutdown' to stop server): ";
        std::cin.getline(msg.data, sizeof(msg.data));

        if (MsgSend(coid, &msg, sizeof(msg), &reply, sizeof(reply)) == -1) {
            perror("MsgSend");
            break;
        }

        std::cout << "[Client] Reply: " << reply.data << std::endl;

        if (strcmp(reply.data, "terminate") == 0) {
            std::cout << "[Client] Server requested termination.\n";
            break;
        }
    }

    ConnectDetach(coid);
    return 0;
}
