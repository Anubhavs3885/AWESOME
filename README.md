# Assignment 3: Raw Socket TCP Handshake

## Overview
This project implements a simplified TCP handshake using raw sockets in C++. The server listens for SYN packets, responds with SYN-ACK, and waits for an ACK from the client to complete the handshake. The client initiates the handshake and sends the required packets in response.

## Files Included
- **`server.cpp`**: Implements the server that listens for incoming SYN packets and completes the handshake.
- **`client.cpp`**: Implements the client that sends a SYN packet, receives the SYN-ACK response, and sends the final ACK.
- **`README.md`**: This file, providing instructions on compilation and execution.

## Prerequisites
Before running the programs, ensure you have:
- A Linux environment (raw sockets require administrative privileges).
- A C++ compiler (e.g., `g++`).
- Superuser (`sudo`) access to run raw socket programs.

## Compilation Instructions
Use the following commands to compile both server and client programs:
```sh
sudo g++ server.cpp -o server
sudo g++ client.cpp -o client
```

## Execution Instructions
Follow these steps to run the server and client:

### Step 1: Run the Server
Open a terminal and execute:
```sh
sudo ./server
```

### Step 2: Run the Client
Open another terminal and execute:
```sh
sudo ./client
```

## Expected Output
### Server Output
```sh
[+] Server listening on port 12345...
[+] Received SYN from 127.0.0.1
[+] Sent SYN-ACK
[+] Received ACK, handshake complete.
```

### Client Output
```sh
[+] Sent SYN
[+] Received SYN-ACK
[+] Sent ACK, handshake complete.
```

## Notes
- The client follows the correct sequence numbers (`200`, `400`, `600`) as per the assignment.
- Running raw sockets requires `sudo` privileges.
- The client and server should be run on the same machine (`127.0.0.1`).
- Ensure `IP_HDRINCL` is enabled for proper packet transmission.

## Troubleshooting
- If the packets are not being received, verify that your firewall or security settings are not blocking raw sockets.
- Use `tcpdump` or `Wireshark` to monitor raw packets being sent and received.
- Check that you have the correct permissions (`sudo`) when running the programs.

---

This concludes the setup and execution guide for the TCP handshake using raw sockets. Happy coding! 🚀

