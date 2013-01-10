// copies contents of src to dst, possibly overwriting whatever is in dst
int copy_file (const char * src, const char * dst)
{
	struct stat mystat;

	if (stat (src, &mystat) < 0) {
		logprintfl (EUCAERROR, "error: cannot stat '%s'\n", src);
		return ERROR;
	}

	int ifp = open (src, O_RDONLY);
	if (ifp<0) {
		logprintfl (EUCAERROR, "failed to open the input file '%s'\n", src);
		return ERROR;
	}

	int ofp = open (dst, O_WRONLY | O_CREAT, 0600);
	if (ofp<0) {
		logprintfl (EUCAERROR, "failed to create the ouput file '%s'\n", dst);
		close (ifp);
		return ERROR;
	}

#   define _BUFSIZE 16384
	char buf [_BUFSIZE];
	ssize_t bytes;
	int ret = OK;

	while ((bytes = read (ifp, buf, _BUFSIZE)) > 0) {
		if (write (ofp, buf, bytes) < 1) {
			logprintfl (EUCAERROR, "failed while writing to '%s'\n", dst);
			ret = ERROR;
			break;
		}
	}
	if (bytes<0) {
		logprintfl (EUCAERROR, "failed while writing to '%s'\n", dst);
		ret = ERROR;
	}

	close (ifp);
	close (ofp);

	return ret;
}
