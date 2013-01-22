// turn a string into a boolean (returned as a char)
char parse_boolean (const char * s)
{
    char * lc = strduplc (s);
    char val = 0;

    if (strcmp (lc, "y")==0 ||
        strcmp (lc, "yes")==0 ||
        strcmp (lc, "t")==0 ||
        strcmp (lc, "true")==0) { val = 1; }
    else if (strcmp (lc, "n")==0 ||
             strcmp (lc, "no")==0 ||
             strcmp (lc, "f")==0 ||
             strcmp (lc, "false")==0) { val = 0; }
    else
        logprintfl (EUCAERROR, "error: failed to parse value '%s' as boolean", lc);
    free (lc);

    return val;
}
