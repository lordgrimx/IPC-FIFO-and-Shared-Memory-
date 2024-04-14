#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <vector>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 1234
#define SHM_SIZE 1024
#define FIFO_NAME "/tmp/myfifo"

using namespace std;

// Structure to represent a program
struct program {
    int program_id;         // Program ID
    int program_version;    // Program version
    string program_name;    // Program name
};

// Function to send update messages to shared memory
void sendUpdate(int shmid, const program& server_program) {
    // Check if shared memory segment is attached properly
    if (shmid == -1) {
        perror("Error attaching shared memory segment");
        return;
    }

    // Print update information
    cout << "Sending update for Program Name: " << server_program.program_name 
         << ", Version: " << server_program.program_version << endl;
    
    // Construct update message and download link
    string updateMessage = "Update available for " + server_program.program_name + ". ";
    string downloadLink = "Download link: http://example.com/download/" + server_program.program_name;

    // Attach to shared memory
    char *shared_memory = (char*)shmat(shmid, NULL, 0);
    if (shared_memory == (char*)-1) {
        perror("Error attaching shared memory segment");
        return;
    }

    // Write update message to shared memory
    strcpy(shared_memory, updateMessage.c_str());

    // Write download link to shared memory after the update message
    strcpy(shared_memory + updateMessage.size() + 1, downloadLink.c_str());

    // Detach shared memory
    if (shmdt(shared_memory) == -1) {
        perror("Error detaching shared memory segment");
    }
}

// Function to check if shared memory is empty
bool isSharedMemoryEmpty(int shmid) {
    // Attach to shared memory
    char *shared_memory = (char*)shmat(shmid, NULL, 0);
    if (shared_memory == (char*)-1) {
        perror("Error attaching shared memory segment");
        return false;
    }

    // Check if shared memory is empty
    bool isEmpty = true;
    for (int i = 0; i < SHM_SIZE; i++) {
        if (shared_memory[i] != '\0') {
            isEmpty = false;
            break;
        }
    }

    // Detach shared memory
    shmdt(shared_memory);
    return isEmpty;
}

// Function to clean up shared memory
void cleanupSharedMemory() {
    // Try to attach to the existing shared memory segment
    int shmid = shmget(SHM_KEY, SHM_SIZE, 0666);
    if (shmid == -1) {
        // If there's no existing shared memory segment, no need to clean up
        return;
    }

    // Try to remove the shared memory segment
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("Error deleting shared memory segment");
        exit(EXIT_FAILURE);
    }

    cout << "Previous shared memory segment removed successfully." << endl;
}

// Server function
void serverFunction() {
    // Clean up previous shared memory segment if it exists
    cleanupSharedMemory();

    // Create a new shared memory segment
    int shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | IPC_EXCL | 0666);
    if (shmid == -1) {
        perror("Error creating shared memory segment");
        exit(EXIT_FAILURE);
    }

    // Define server's program list
    vector<program> server_program_list = {
        {18, 3, "CinsCalculator"},
        {22, 1, "CinsBrowser"},
        {25, 2, "CinsAntivirus"}
    };

    // Create named pipe
    mkfifo(FIFO_NAME, 0666);
    int fifo_fd = open(FIFO_NAME, O_RDONLY);
    if (fifo_fd == -1) {
        perror("Error opening named pipe");
        exit(EXIT_FAILURE);
    }

    // Define a vector to hold client's program list
    vector<program> client_program_list(3);

    // Main loop to handle communication with client
    while (true) {
        ssize_t n = read(fifo_fd, client_program_list.data(), client_program_list.size() * sizeof(program));
        if (n <= 0) {
            if (n < 0) {
                perror("Error reading from named pipe");
            }
            usleep(100000); // Sleep for 100ms if no data is available
            continue;
        }

        cout << "****************************************" << endl;
        cout << "Received " << n << " bytes from client" << endl;

        int num_programs = n / sizeof(program);
        if (n % sizeof(program) != 0) {
            cerr << "Error: Invalid data size received from client" << endl;
            continue;
        }

        // Check for updates in the received program list
        for (int i = 0; i < num_programs; i++) {
            for (int j = 0; j < server_program_list.size(); j++) {
                if (server_program_list[j].program_id == client_program_list[i].program_id &&
                    server_program_list[j].program_version != client_program_list[i].program_version) {
                    // Send update to shared memory
                    sendUpdate(shmid, server_program_list[j]);
                    usleep(100000); // Sleep for 100ms
                    while (true)
                    {
                        // Wait until shared memory is empty
                        if (isSharedMemoryEmpty(shmid)) {
                            cout << "Shared memory is empty. Waiting for next update..." << endl;
                            break;
                        }else{
                            continue;
                        }
                    }
                }
            }
        }
        break;
    }

    // Close and unlink named pipe
    close(fifo_fd);
    unlink(FIFO_NAME);

    // Remove shared memory segment
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("Error deleting shared memory segment");
    }

    // Recursive call to restart serverFunction()
    serverFunction();
}

// Main function
int main() {
    cout << "Server started. Waiting for clients..." << endl;
    serverFunction();
    return 0;
}

