#include <stdio.h>
#include <string.h>

#define PACKET_SIZE 10
#define MAX_INPUT_SIZE (PACKET_SIZE + 1) // Including null terminator

int main() {
    char packet[MAX_INPUT_SIZE];

    // Continuous loop until EOF or error
    while (1) {
        // Read one packet of input
        if (fgets(packet, MAX_INPUT_SIZE, stdin) == NULL) {
            if (feof(stdin)) {
                // End of input stream
                printf("End of input stream reached.\n");
            } else {
                // Error reading input
                printf("Error reading input.\n");
            }
            break;
        }

        // Process the packet (replace this with your logic)
        printf("Packet: %s", packet);

        // Check if the packet is smaller than PACKET_SIZE
        if (strlen(packet) < PACKET_SIZE) {
            // End of input reached
            printf("End of input reached.\n");
            break;
        }
    }

    return 0;
}
