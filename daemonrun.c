/* daemonize and run 'cmd', returning pid of the daemonized process */
int daemonrun(char *incmd, char *pidfile) {
  int pid, sid, i, status, rc;
  char **argv=NULL, *cmd;
  
  if (!incmd) {
    return(1);
  }
  
  pid = fork();
  if (pid < 0) {
    return(1);
  }
  if (!pid) {
    char *tok=NULL, *ptr=NULL;
    int idx, rc;
    struct sigaction newsigact = { 0 };

    newsigact.sa_handler = SIG_DFL;
    newsigact.sa_flags = 0;
    sigemptyset(&newsigact.sa_mask);
    sigprocmask(SIG_SETMASK, &newsigact.sa_mask, NULL);
    sigaction(SIGTERM, &newsigact, NULL);
    
    rc = daemon(0,0);
    // become parent of session
    sid = setsid();
    
    // construct argv
    cmd = strdup(incmd);
    idx=0;
    argv = realloc(NULL, sizeof(char *));
    tok = strtok_r(cmd, " ", &ptr);
    while(tok) {
      fflush(stdout);
      argv[idx] = strdup(tok);
      idx++;
      tok = strtok_r(NULL, " ", &ptr);
      argv = realloc(argv, sizeof(char *) * (idx+1));
    }
    argv[idx] = NULL;
    free(cmd);
    
    // close all fds
    for (i=0; i<sysconf(_SC_OPEN_MAX); i++) {
      close(i);
    }

    if (pidfile) {
      char pidstr[32];
      snprintf(pidstr, 32, "%d", getpid());
      rc = write2file(pidfile, pidstr);
    }
    // run
    exit(execvp(*argv, argv));
  } else {
    rc = waitpid(pid, &status, 0);
  }
  
  return(0);
}
