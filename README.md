# QNX-Native-IPC-Message-Passing
This project demonstrates the fundamental concepts of **native interprocess communication (IPC)** in the QNX Neutrino Real-Time Operating System (RTOS) using **synchronous message passing**. It consists of two C++ programs:

- **Server (`server.cpp`)**: Creates a named channel, waits for client messages, replies synchronously, and handles shutdown.
- **Client (`client.cpp`)**: Connects to the server either via PID & channel ID or by registered name, sends messages, and processes replies.

The project is an educational example ideal for embedded systems developers or engineers new to QNX IPC.

---

## Key Concepts

- **QNX Message Passing:** Core IPC mechanism in QNX for synchronous communication.
- **Channels & Connections:** Server creates channels to receive messages; clients attach connections to channels.
- **Named Channels:** Using `name_attach()` and `name_open()` allows clients to connect by name rather than numeric IDs.
- **Synchronous Messaging:** Clients send requests and wait for server replies in blocking calls.
- **Graceful Shutdown:** Server listens for a special `"shutdown"` message to terminate cleanly.

---

## Files

### `server.cpp`

- Creates a communication channel.
- Registers a name `"qnx_server"` for clients to find the channel.
- Receives and prints client messages.
- Replies with `"ack"` for normal messages.
- Shuts down cleanly upon receiving `"shutdown"`, replying `"terminate"`.

### `client.cpp`

- Connects to the server via either:
  - Command-line arguments: PID and Channel ID.
  - Named connection: `"qnx_server"`.
- Prompts user to input messages.
- Sends messages synchronously to the server.
- Displays server replies.
- Exits gracefully if server replies with `"terminate"`.

---

## Environment

- Tested on QNX Neutrino RTOS 8.x
- Developed in C++17 standard
- Requires QNX SDP 8.0 BSP for Generic x86 64 (64-bit).
- qemu-system-x86_64

## How to Build

Compile with QNX compiler (example):

```sh
qcc -o server server.cpp
qcc -o client client.cpp
```

## How to Run

```sh
Server: ./server
Client:  ./client or ./client <server_pid> <server_chid>
```

##Learning Outcomes

-Understand QNX native IPC primitives and system calls.
-Gain hands-on experience with synchronous message passing.
-Learn how to create and use named channels for flexible communication.
-Handle IPC errors and implement clean shutdown protocols.

##License

This project is for educational purposes. Feel free to use and modify the code.

Contact

For questions or suggestions, please open an issue or contact [ahmedlotfy271993@gmail.com].
