#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

static void *child(void *ignored) {
	while(1) { // Infinite loop.
		printf("Child will start sleeping for 5 seconds.\n");
		sleep(5);
		printf("Child is done sleeping 5 seconds.\n");
	}
	return NULL;
}

int main(int argc, char *argv[]) {

	pthread_t child_thread;
	int code;
	code = pthread_create(&child_thread, NULL, child, NULL); // Starts the child thread.

	if (code) {
		fprintf(stderr, "pthread_create failed with code %d\n", code);
	}

	char c;

	while(1) {
		// Get user input.
		c = getchar();
		// Since we only need the enter key, this should work.
		if (c == '\n') {
			break;
		}
	}

	pthread_cancel(child_thread); // This kills the thread. ;-;

	printf("Child thread killed.\n");
	return 0;
}
