#include "headers.h"


char *get_username() {
    struct passwd *pw = getpwuid(getuid());
    if (pw != NULL) {
        return pw->pw_name;
    }
    return NULL;
}

char *get_system_name() {
    struct utsname uname_data;
    if (uname(&uname_data) == 0) {
        return strdup(uname_data.nodename);
    }
    return NULL;
}

char *get_relative_path(char *cwd, const char initial_dir[1024]) {
    if (strcmp(cwd, initial_dir) == 0) {
        return strdup("~");
    } else if (strstr(cwd, initial_dir) != NULL) {
        size_t len = strlen(initial_dir);
        char *relative_path = strdup(cwd + len);
        return relative_path;
    } else {
        return strdup(cwd);
    }
}
