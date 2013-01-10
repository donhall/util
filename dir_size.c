/* sums up sizes of files in the directory, as well as the size of the
 * directory itself; no subdirectories are allowed - if there are any, this
 * returns -1 */
long long dir_size (const char * path)
{
    struct stat mystat;
    DIR * dir;
    long long size = 0;

    if ((dir=opendir(path))==NULL) {
        logprintfl (EUCAWARN, "warning: unopeneable directory %s\n", path);
        return -1;
    }
    if (stat (path, &mystat) < 0) {
        logprintfl (EUCAWARN, "warning: could not stat %s\n", path);
	closedir(dir);
        return -1;
    }
    size += (long long)mystat.st_size;

    struct dirent * dir_entry;
    while ((dir_entry=readdir(dir))!=NULL) {
        char * name = dir_entry->d_name;
        unsigned char type = dir_entry->d_type;

        if (!strcmp(".", name) ||
            !strcmp("..", name))
            continue;

        if (DT_REG!=type) {
            logprintfl (EUCAWARN, "warning: non-regular (type=%d) file %s/%s\n", type, path, name);
            size = -1;
            break;
        }
        
        char filepath [MAX_PATH];
        snprintf (filepath, MAX_PATH, "%s/%s", path, name);
        if (stat (filepath, &mystat) < 0 ) {
            logprintfl (EUCAWARN, "warning: could not stat file %s\n", filepath);
            size = -1;
            break;
        }
        
        size += (long long)mystat.st_size;
    }

    closedir (dir);
    return size;
}
