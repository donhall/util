char * replace_string (char ** stringp, char * source, char * destination )
{
    char *start=NULL, *substart=NULL, *tok=NULL, * new_string=NULL;
    if (source==NULL || destination==NULL) return NULL;
    char * buf;
    int maxlen = 32768*2;
   
    buf = malloc(sizeof(char) * maxlen);
    new_string = malloc(sizeof(char) * maxlen); /* TODO: this has to be dynamic */
    if (!buf || !new_string) {
        fprintf(stderr, "replace_string: out of memory\n");
     if (buf) free(buf);
     if (new_string) free(new_string);
     return NULL;
    }
    bzero(new_string, maxlen);
   
    start = * stringp;
    substart = start;
    tok = strstr(start, source);
    while(tok != NULL) {
        *tok = '\0';
        snprintf (buf, maxlen, "%s%s%s", new_string, substart, destination);
        strncpy (new_string, buf, maxlen);
        tok+=strlen(source);
        substart = tok;
        tok = strstr(substart, source);
    }
    snprintf (buf, maxlen, "%s%s", new_string, substart);
    strncpy (new_string, buf, maxlen);
    if (buf) free(buf);

    free (* stringp);
    * stringp = new_string;

   
    return new_string;
}

