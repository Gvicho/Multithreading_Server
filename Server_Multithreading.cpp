#include <iostream>
#include <winsock2.h> // Don't forget to link the ws2_32.lib library tou the project this code is in...
#include <thread> // Include this header for multithreading

// Function to handle client connections
void HandleClient(SOCKET clientSocket) {
    char buffer[1024];
    int bytesReceived;

    // Step 6: Receive data from the client
    bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0'; // Null-terminate received data

        // Step 6: Process received data (e.g., send a response)
        std::cout << "Received from client: " << buffer << std::endl;
        const char* responseMessage = "Hello from server!";
        std::cout << "Message was Sent\n";
        send(clientSocket, responseMessage, strlen(responseMessage), 0);
    }

    // Step 7: Close the client socket
    closesocket(clientSocket);
}

int main() {
    // Step 1: Initialize WSA
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return 1;
    }
    std::cout << "WSA Initialized!\n";


    // Step 2: Create a socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket." << std::endl;
        WSACleanup();
        return 1;
    }
    std::cout << "Socket was created!\n";

    // Step 3: Bind the socket
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Failed to bind socket." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "Socket was binded!\n";

    // Step 4: Listen for client connections
    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        std::cerr << "Failed to listen on the socket." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "Listening for the client connections...\n";

    // Main loop to accept and handle multiple client connections
    while (true) {
        // Step 5: Accept a client connection
        sockaddr_in clientAddress;
        int clientAddressSize = sizeof(clientAddress);
        SOCKET clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressSize);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Failed to accept client connection." << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        std::cout << "Connection was created, prepearing thread for the client...\n";

        // Create a thread to handle the client
        std::thread clientThread(HandleClient, clientSocket);
        clientThread.detach(); // Detach the thread so it can run independently
    }

    // Clean up (this code will never be reached due to the infinite loop)
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
