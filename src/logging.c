#include "logging.h"

const char *get_filename(const char *path)
{
	if (path == NULL) {
		errout("path == NULL");
		return NULL;
	}
	const char *last_slash = path;
	while (*path != '\0')
	{
		if (*path == '\\' || *path == '/') last_slash = path+1;
		path++;
	}
	return last_slash;
}
