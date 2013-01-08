char * pruntf (int log_error, char *format, ...)
{
    va_list ap;
    FILE *IF=NULL;
    char cmd[1024], *ptr;
    size_t bytes=0;
    int outsize=1025, rc;
    char *output=NULL;

    va_start(ap, format);
    vsnprintf(cmd, 1024, format, ap);

    strncat(cmd, " 2>&1", 1024 - 1);
    output = NULL;

    IF=popen(cmd, "r");
    if (!IF) {
      va_end(ap);
      return(NULL);
    }

    output = malloc(sizeof(char) * outsize);
    if(output) {
        output[0]='\0'; // make sure we return an empty string if there is no output
    }

    while(output != NULL && (bytes = fread(output+(outsize-1025), 1, 1024, IF)) > 0) {
        output[(outsize-1025)+bytes] = '\0';
        outsize += 1024;
        output = realloc(output, outsize);
    }

    if (output == NULL) {
        va_end(ap);
        pclose(IF);
        return(NULL);
    }

    rc = pclose(IF);
    if (rc) {
        // TODO: improve this hacky special case: failure to find or detach non-existing loop device is not a failure
        if (strstr (cmd, "losetup") && strstr (output, ": No such device or address")) {
            rc = 0;
        } else {
            if (log_error) {
                printf ("error:%s\n", output);
            }
            if (output) free (output);
            output = NULL;
        }
    }
    va_end(ap);

    return (output);
}