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
        logprintfl(EUCAERROR, "waitpid() timed out: pid=%d\n", pid);
    }
    return (rc);
}