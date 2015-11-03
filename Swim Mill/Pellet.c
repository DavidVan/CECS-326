#define _XOPEN_SOURCE 700
#include <stdlib.h> // Standard library
#include <time.h> // For seeding rand
#include <unistd.h> // For fork
#include <signal.h> // For signal
#include <stdbool.h> // For bool
#include <stdio.h> // Standard I/O
#include <sys/types.h> // For types (such as pid_t)
#include <sys/ipc.h> // For interprocess communication
#include <sys/shm.h> // For shared memory
#include <sys/wait.h> // For wait

void onInterrupt();
void onTermination();

bool eaten = false;	

int main(int argc, char *argv[]) {

	signal(SIGINT, onInterrupt);	
	signal(SIGTERM, onTermination);

	int sMemID; // Shared memory ID
	key_t keyValue = 1337;
	char (*dataPtr)[10][10]; // Pellet 2D Array
	
	// Create shared memory segment. Exit on error.
	if ((sMemID = shmget(keyValue, sizeof(char[10][10]), 0666)) < 0) {
		perror("Could not create memory segment!\n");
		exit(1);
	}
	
	// Attach the shared memory segment. Exit on error.
	if ((dataPtr = (char(*)[10][10])shmat(sMemID, NULL, 0)) == (char(*)[10][10]) -1) {
		perror("Could not attach shared memory segment!\n");
		exit(1);
	}
	
	srand(time(NULL));	
	int row = rand() % 10;
	int col = rand() % 10;
	while (*dataPtr[row][col] != '~') {
		row = rand() % 10;
		col = rand() % 10;	
	}
	*dataPtr[row][col] = 0x50; // My terminal doesn't support extended ASCII, so we won't be using 0x80.

	while(row < 9) {
		*dataPtr[row][col] = '~';
		row++;
		if (*dataPtr[row][col] == 'F') {
			*dataPtr[row][col] |= 0x50;
			eaten = true;
			sleep(1);
			break;
		}
		*dataPtr[row][col] = 0x50;
		sleep(1);
	}
	if (!eaten) {
		*dataPtr[row][col] = '~';
	}
	else if (*dataPtr[row][col] == 0x50) {
		*dataPtr[row][col] = 'F';
	}
	sleep(1);
	printf("Pellet PID: %d, X: %d, Y: %d - eaten: %s\n", getpid(), row, col, eaten ? "true" : "false");
	exit(0);
}

void onInterrupt() {
	fprintf(stderr, "\n(Pellet) PID: %d - Died due to interrupt! - Eaten: %s\n", getpid(), eaten ? "true" : "false");
	exit(0);
}

void onTermination() {
	fprintf(stderr, "\n(Fish) PID: %d - Died due to termination! - Eaten: %s\n", getpid(), eaten ? "true" : "false");
	exit(0);

}
