int check_process(pid_t pid, char *search) {
  char file[MAX_PATH], buf[1024];
  FILE *FH=NULL;
  int rc, ret=0;

  snprintf(file, MAX_PATH, "/proc/%d/cmdline", pid);
  rc = check_file(file);
  if (!rc) {
    // cmdline exists
    ret = 1;
    if (search) {
      // check if cmdline contains 'search'
      FH = fopen(file, "r");
      if (FH) {
     bzero(buf, 1024);
     while(fgets(buf, 1024, FH)) {
       char *p;
       while((p = memchr(buf, '\0', 1024))) {
         *p = 'X';
       }
       buf[1023] = '\0';
       if (strstr(buf, search)) {
         ret = 0;
       }
     }
     fclose(FH);
      }
    } else {
      ret = 0;
    }
  } else {
    ret = 1;
  }
  return(ret);
}