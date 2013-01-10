/* search for variable #name# in file #path# and return whatever is after
 * = in value (which will need to be freed).
 *
 * Example of what we are able to parse:
 * TEST="test"
 * TEST=test
 *     TEST   = test
 *
 * Return 1 on success, 0 on variable not found and -1 on error (parse or
 * file not found)
 */
int
get_conf_var(	const char *path, 
		const char *name,
		char **value) {
	FILE *f;
	char *buf, *ptr, *ret;
	int len;

	/* sanity check */
	if (path == NULL || path[0] == '\0' || name == NULL  
			|| name[0] == '\0'|| value == NULL) {
		return -1;
	}
	*value = NULL;

	f = fopen(path, "r");
	if (f == NULL) {
		return -1;
	}

	len = strlen(name);
	buf = malloc(sizeof(char) * 32768);
	while (fgets(buf, 32768, f)) {
		/* the process here is fairly simple: spaces are not
		 * considered (unless between "") so we remove them
		 * before every step. We look for the variable *name*
		 * first, then for an = then for the value */
		for (ptr = buf; (*ptr != '\0') && isspace((int)*ptr); ptr++)
			; 
		if (strncmp(ptr, name, len) != 0) {
			continue;
		}
		for (ptr += len; (*ptr != '\0') && isspace((int)*ptr); ptr++)
			;
		if (*ptr != '=') {
			continue;
		}
		/* we are in business */
		for (ptr++; (*ptr != '\0') && isspace((int)*ptr); ptr++)
			;
		if (*ptr == '"') {
			/* we have a quote, we need the companion */
			ret = ++ptr;
			while ((*ptr != '"')) {
				if (*ptr == '\0') {
					/* something wrong happened */
				  fclose(f);
				  free(buf);
				  return -1;
				}
				ptr++;
			}
		} else {
			/* well we get the single word right after the = */
			ret = ptr;
			while (!isspace((int)*ptr) && *ptr != '#' 
					&& *ptr != '\0') {
				ptr++;
			}
		}
		*ptr = '\0';
		*value = strdup(ret);
		if (*value == NULL) {
		  fclose(f);
		  free(buf);
		  return -1;
		}
		fclose(f);
		free(buf);
		return 1;
	}
	fclose(f);
	free(buf);
	return 0;
}
