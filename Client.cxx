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

int main() {
    // Create or get the shared memory segment
    int shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("Error creating shared memory segment");
        exit(EXIT_FAILURE);
    }

    // Attach to the shared memory segment
    char *shared_memory = (char*)shmat(shmid, NULL, 0);
    if (shared_memory == (char*)-1) {
        perror("Error attaching shared memory segment");
        exit(EXIT_FAILURE);
    }

    // Create a named pipe (FIFO)
    mkfifo(FIFO_NAME, 0666);

    // Inform that the client has started and created a FIFO
    cout << "Client started. FIFO created: " << FIFO_NAME << endl;

    // Open the named pipe for writing
    int fifo_fd = open(FIFO_NAME, O_WRONLY);
    if (fifo_fd == -1) {
        perror("Error opening named pipe");
        exit(EXIT_FAILURE);
    }

    // Define the list of programs the client has
    vector<program> client_program_list = {
        {18, 32, "CinsCalculator"},
        {22, 1, "CinsBrowser"},
        {25, 2, "CinsAntivirus"}
    };

    // Write the client's program list to the named pipe
    write(fifo_fd, client_program_list.data(), client_program_list.size() * sizeof(program));

    // Close the named pipe
    close(fifo_fd);

    string updateMessage;
    int counter = 0;
    while (true) {
        // Read update message from shared memory
        updateMessage = shared_memory;
        if (!updateMessage.empty()) {
            cout << "*********************************************************" << endl;
            cout << "Update Message: " << updateMessage << endl;

            // Find the position of the null terminator separating update message and download link
            size_t nullTerminatorPos = updateMessage.size();

            // Read download link from shared memory
            string downloadLink = shared_memory + nullTerminatorPos + 1;
            cout << "Download Link: " << downloadLink << endl;

            // Clear shared memory after reading
            memset(shared_memory, 0, SHM_SIZE);
            counter = 0;
        }
        counter++;
        if (counter == 5) {
            break; // Exit the loop after 5 iterations (for demonstration)
        }

        // Sleep for a while before checking again (to avoid busy waiting)
        usleep(100000); // 100ms
    }

    // Detach shared memory
    if (shmdt(shared_memory) == -1) {
        perror("Error detaching shared memory segment");
    }

    // The shared memory segment removal is handled by the server, so no need to remove it here.

    return 0;
}

