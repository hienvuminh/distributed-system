// Include necessary headers
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>

// Define constants
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int main() {
    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return -1;
    }

    // Set up server details
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &(serverAddress.sin_addr)) <= 0) {
        std::cerr << "Invalid address/Address not supported." << std::endl;
        return -1;
    }

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Connection failed." << std::endl;
        return -1;
    }

    // Prompt the user for the file name
    std::string fileName;
    std::cout << "Enter the name of the file to transfer: ";
    std::cin >> fileName;

    // Open the file
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file." << std::endl;
        return -1;
    }

    // Read and send the file contents
    char buffer[BUFFER_SIZE];
    while (!file.eof()) {
        file.read(buffer, BUFFER_SIZE);
        send(clientSocket, buffer, file.gcount(), 0);
    }

    // Close the file and socket
    file.close();
    close(clientSocket);

    std::cout << "File transfer complete." << std::endl;

    return 0;
}