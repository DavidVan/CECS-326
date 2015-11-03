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
void onAlarm();
void onTermination();

int main(int argc, char *argv[]) {

	signal(SIGINT, onInterrupt);
	signal(SIGALRM, onAlarm);
	//signal(SIGTERM, onTermination);
	alarm(30);

	int sMemID; // Shared memory ID
	key_t keyValue = 1337;
	char (*dataPtr)[10][10]; // Pellet 2D Array

	// Create shared memory segment. Exit on error.
	if ((sMemID = shmget(keyValue, sizeof(char[10][10]), IPC_CREAT | 0666)) < 0) {
		perror("Could not create memory segment!\n");
		exit(1);
	}
	
	// Attach the shared memory segment. Exit on error.
	if ((dataPtr = (char(*)[10][10])shmat(sMemID, NULL, 0)) == (char(*)[10][10]) -1) {
		perror("Could not attach shared memory segment!\n");
		exit(1);
	}
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			*dataPtr[i][j] = '~';
		}
	}

	pid_t pIDFish = fork();
	if (pIDFish == 0) {
		execv("Fish", argv);
	}
	else {
		sleep(1);
		pid_t pIDPrint = fork();
		if (pIDPrint == 0) {
			while(1) {
				// Print the shared memory.
				printf("@@@@@@@@@@");
				for(int i = 0; i < 10; i++) {
					printf("\n");
					for (int j = 0; j < 10; j++) {
						printf("%c", *dataPtr[i][j]);
					}
				}
				printf("\n");
				wait(NULL);
				sleep(1);
			}
		}
		while(1) {
			pid_t pIDPellet = fork();
			if (pIDPellet == 0) {
				execv("Pellet", argv);
			}
			srand(time(NULL));
			int sec = ((rand() % 10) + 2);
			sleep(sec);
		}
	}
}

void onInterrupt() {
	printf("\n(Coordinator) PID: %d - Died due to interrupt!\n", getpid());
	int sMemID; // Shared memory ID
	key_t keyValue = 1337;
	char (*dataPtr)[10][10]; // Pellet 2D Array
	// Create shared memory segment. Exit on error.
	if ((sMemID = shmget(keyValue, sizeof(char[10][10]), IPC_CREAT | 0666)) < 0) {
		perror("Could not create memory segment!\n");
		exit(1);
	}
	
	// Attach the shared memory segment. Exit on error.
	if ((dataPtr = (char(*)[10][10])shmat(sMemID, NULL, 0)) == (char(*)[10][10]) -1) {
		perror("Could not attach shared memory segment!\n");
		exit(1);
	}
	shmdt(dataPtr);
	shmctl(sMemID, IPC_RMID, NULL);
	while (wait(NULL) > 0) {
		printf("Child dead\n");
	}
	exit(0);
}

void onAlarm() {
	raise(SIGINT);
}

void onTermination() {
	printf("\n(Coordinator) PID: %d - Died due to termination!\n", getpid());
	int sMemID; // Shared memory ID
	key_t keyValue = 1337;
	char (*dataPtr)[10][10]; // Pellet 2D Array
	// Create shared memory segment. Exit on error.
	if ((sMemID = shmget(keyValue, sizeof(char[10][10]), IPC_CREAT | 0666)) < 0) {
		perror("Could not create memory segment!\n");
		exit(1);
	}
	
	// Attach the shared memory segment. Exit on error.
	if ((dataPtr = (char(*)[10][10])shmat(sMemID, NULL, 0)) == (char(*)[10][10]) -1) {
		perror("Could not attach shared memory segment!\n");
		exit(1);
	}
	shmdt(dataPtr);
	shmctl(sMemID, IPC_RMID, NULL);
	while (wait(NULL) > 0) {
		printf("Child dead\n");
	}
	exit(0);
}
