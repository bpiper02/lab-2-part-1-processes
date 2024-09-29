#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define MAX_ITERATIONS 30
#define MAX_SLEEP_TIME 10

void childProcess(void);  // Prototype for child process function
void parentProcess(pid_t childPid1, pid_t childPid2);  // Prototype for parent process function

int main(void)
{
    pid_t pid1, pid2;

    // Create the first child process
    pid1 = fork();
    if (pid1 < 0) {
        perror("Fork failed for child 1");
        exit(1);
    } else if (pid1 == 0) {
        childProcess();  // First child process
        exit(0);
    }

    // Create the second child process
    pid2 = fork();
    if (pid2 < 0) {
        perror("Fork failed for child 2");
        exit(1);
    } else if (pid2 == 0) {
        childProcess();  // Second child process
        exit(0);
    }

    // Parent process waits for both child processes to finish
    parentProcess(pid1, pid2);

    return 0;
}

void childProcess(void)
{
    int iterations = random() % MAX_ITERATIONS + 1;  // Random iterations, max 30
    srandom(time(NULL) ^ getpid());  // Seed the random number generator with process-specific randomness

    for (int i = 0; i < iterations; i++) {
        printf("Child Pid: %d is going to sleep!\n", getpid());

        int sleepTime = random() % MAX_SLEEP_TIME + 1;  // Random sleep time, max 10 seconds
        sleep(sleepTime);

        printf("Child Pid: %d is awake!\nWhere is my Parent: %d?\n", getpid(), getppid());
    }

    printf("Child Pid: %d has finished looping.\n", getpid());
}

void parentProcess(pid_t childPid1, pid_t childPid2)
{
    int status;

    // Wait for the first child process to finish
    waitpid(childPid1, &status, 0);
    printf("Child Pid: %d has completed\n", childPid1);

    // Wait for the second child process to finish
    waitpid(childPid2, &status, 0);
    printf("Child Pid: %d has completed\n", childPid2);
}
