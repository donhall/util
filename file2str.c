/* read file 'path' into a new string */
char * file2str (const char * path)
{
    char * content = NULL;
    int file_size;

    struct stat mystat;
    if (stat (path, &mystat) < 0) {
        logprintfl (EUCAERROR, "error: file2str() could not stat file %s\n", path);
        return content;
    }
    file_size = mystat.st_size;

    if ( (content = malloc (file_size+1)) == NULL ) {
        logprintfl (EUCAERROR, "error: file2str() out of memory reading file %s\n", path);
        return content;
    }

    int fp;
    if ( ( fp = open (path, O_RDONLY) ) < 0 ) {
        logprintfl (EUCAERROR, "error: file2str() failed to open file %s\n", path);
        free (content);
        content = NULL;
        return content;
    }

    int bytes;
    int bytes_total = 0;
    int to_read = ((SSIZE_MAX)<file_size)?(SSIZE_MAX):file_size;
    char * p = content;
    while ( (bytes = read (fp, p, to_read)) > 0) {
        bytes_total += bytes;
        p += bytes;
        if (to_read > (file_size-bytes_total)) {
            to_read = file_size-bytes_total;
        }
    }
    close(fp);

    if ( bytes < 0 ) {
        logprintfl (EUCAERROR, "error: file2str() failed to read file %s\n", path);
        free (content);
        content = NULL;
        return content;
    }

    * p = '\0';
    return content;
}
