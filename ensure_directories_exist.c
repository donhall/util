// given path=A/B/C and only A existing, create A/B and, unless
// is_file_path==1, also create A/B/C directory
// returns: 0 = path already existed, 1 = created OK, -1 = error
int ensure_directories_exist (const char * path, int is_file_path, const char * user, const char * group, mode_t mode)
{
    int len = strlen (path);
    char * path_copy = NULL;
    int ret = 0;
    int i;

    if (len>0)
        path_copy = strdup (path);

    if (path_copy==NULL)
        return -1;

    for (i=0; i<len; i++) {
        struct stat buf;
        int try_dir = 0;

        if (path[i]=='/' && i>0) { // dir path, not root
            path_copy[i] = '\0';
            try_dir = 1;

        } else if (path[i]!='/' && i+1==len) { // last one
            if (!is_file_path)
                try_dir = 1;
        }

        if ( try_dir ) {
            if ( stat (path_copy, &buf) == -1 ) {
                logprintfl (EUCAINFO, "{%u} creating path %s\n", (unsigned int)pthread_self(), path_copy);

                if ( mkdir (path_copy, mode) == -1) {
                    logprintfl (EUCAERROR, "error: failed to create path %s: %s\n", path_copy, strerror (errno));

                    free (path_copy);
                    return -1;
                }
                ret = 1; // we created a directory

                if(diskutil_ch(path_copy, user, group, mode) != OK) {
                    logprintfl (EUCAERROR, "error: failed to change perms on path %s\n", path_copy);
                    free (path_copy);
                    return -1;
                }
                //                chmod (path_copy, mode); // ensure perms are right despite mask
            }
            path_copy[i] = '/'; // restore the slash
        }
    }

    free (path_copy);
    return ret;
}
