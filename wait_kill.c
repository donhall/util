#include <stdio.h>
#include <sys/statfs.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t timewait (pid_t pid, int *status, int timeout_sec) 
{    
    if (timeout_sec < 0) // do not allow negative timeouts
        timeout_sec = 0;
    
    *status = 1; // TODO: remove this once we know that no callers rely on status to detect timeout
    
    int rc = waitpid (pid, status, WNOHANG);
    time_t elapsed_usec = 0;
    while (rc == 0 && elapsed_usec < (timeout_sec * 1000000)) {
        usleep (10000);
        elapsed_usec += 10000;
        rc = waitpid(pid, status, WNOHANG);
    }
    if (rc == 0) {
       printf("waitpid() timed out: pid=%d\n", pid);
    }
    return (rc);
}

int main(int argc, char *argv[])
{
	pid_t pid;
	int st, ret;
	pid=fork();
	if(pid == 0) {
		struct statfs fs;
		if (statfs (argv[1], &fs) == -1) {
			perror("statfs failed:");
			exit(-1);
    		}
		exit(0);
	} else {
		ret=timewait(pid, &st, 2);
	}
	if(ret == 0) {
		kill(pid, SIGTERM);
		printf("ret=0, pid=%d\n", pid);
	} else {
		printf("pid=%d, ret=%d\n", pid, ret);
	}
	return 1;
}