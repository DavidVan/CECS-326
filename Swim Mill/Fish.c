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

	*dataPtr[9][5] = 'F';
	
	while(1) {
		// Get location of the fish
		int fishCurrentCol;
		for (int i = 0; i < 10; i++) {
			if (*dataPtr[9][i] == 'F') {
				fishCurrentCol = i;
				//printf("Fish location: %d\n", fishCurrentCol);
			}
		}
		// Check closest pellet row based on location of the fish
		int closestPelletRow = 10;
		for (int i = 9; i >= 0; i--) {
			for (int j = 9; j >= 0; j--) {
				if (*dataPtr[i][j] == 0x50 && abs(closestPelletRow - 9) < abs(i - 9)) {
					// Pellet found!
					closestPelletRow = i;
					printf("Closest row: %d\n", closestPelletRow);
				}
			}
		}
		// Check closest pellet column based on location of the fish
		int closestPelletCol = 10;	
		for (int i = 9; i >= 0; i--) {
			for (int j = 9; j >= 0; j--) {
				if (*dataPtr[i][j] == 0x50 && abs(closestPelletCol - fishCurrentCol) > abs(j - fishCurrentCol)) {
					// Pellet found!
					closestPelletCol = j;
					printf("Closest column: %d\n", closestPelletCol);
				}
			}
		}
		if (closestPelletCol != 10) {
			// Now move the fish!
			// Calculate distance
			int distanceCol = closestPelletCol - fishCurrentCol;
			while (abs(distanceCol)) {
				bool breakTestRow = false;
				// Check closest pellet row based on location of the fish
				int closestPelletRow = 10;
				for (int i = 9; i >= 0; i--) {
					for (int j = 9; j >= 0; j--) {
						if (*dataPtr[i][j] == 0x50 && abs(closestPelletRow - 9) < abs(i - 9)) {
							break;
							breakTestRow = true;
						}
						if (breakTestRow) {
							break;
						}
					}
				}
				bool breakTestCol = false;
				// Check closest pellet column based on location of the fish
				int closestPelletCol = 10;	
				for (int i = 9; i >= 0; i--) {
					for (int j = 9; j >= 0; j--) {
						if (*dataPtr[i][j] == 0x50 && abs(closestPelletCol - fishCurrentCol) > abs(j - fishCurrentCol)) {
							break;
							breakTestCol = true;
						}
						if (breakTestCol) {
							break;
						}
					}
				}
				*dataPtr[9][fishCurrentCol] = '~';
				if (distanceCol > 0) {
					fishCurrentCol++;
				}
				else {
					fishCurrentCol--;
				}
				*dataPtr[9][fishCurrentCol] = 'F';
				if (distanceCol > 0) {
					distanceCol--;
				}
				else {
					distanceCol++;
				}
				struct timespec s;
				s.tv_sec = 0;
				s.tv_nsec = 500000000;
				//nanosleep(&s, NULL);
				sleep(1);
			}
		}
		else {
			// Move the fish back to the middle!
			// Calculate distance
			int distanceCol = fishCurrentCol - 5;
			while (abs(distanceCol)) {
				bool breakTestRow = false;
				// Check closest pellet row based on location of the fish
				int closestPelletRow = 10;
				for (int i = 9; i >= 0; i--) {
					for (int j = 9; j >= 0; j--) {
						if (*dataPtr[i][j] == 0x50 && abs(closestPelletRow - 9) < abs(i - 9)) {
							break;
							breakTestRow = true;
						}
						if (breakTestRow) {
							break;
						}
					}
				}
				bool breakTestCol = false;
				// Check closest pellet column based on location of the fish
				int closestPelletCol = 10;	
				for (int i = 9; i >= 0; i--) {
					for (int j = 9; j >= 0; j--) {
						if (*dataPtr[i][j] == 0x50 && abs(closestPelletCol - fishCurrentCol) > abs(j - fishCurrentCol)) {
							break;
							breakTestCol = true;
						}
						if (breakTestCol) {
							break;
						}
					}
				}
				*dataPtr[9][fishCurrentCol] = '~';
				if (distanceCol > 0) {
					fishCurrentCol--;
				}
				else {
					fishCurrentCol++;
				}
				*dataPtr[9][fishCurrentCol] = 'F';
				if (distanceCol > 0) {
					distanceCol--;
				}
				else {
					distanceCol++;
				}
				struct timespec s;
				s.tv_sec = 0;
				s.tv_nsec = 500000000;
				//nanosleep(&s, NULL);
				sleep(1);
			}
		}
		sleep(1);
	}
	exit(0);
}

void onInterrupt() {
	fprintf(stderr, "\n(Fish) PID: %d - Died due to interrupt!\n", getpid());
	exit(0);
}

void onTermination() {
	fprintf(stderr, "\n(Fish) PID: %d - Died due to termination!\n", getpid());
	exit(0);

}
