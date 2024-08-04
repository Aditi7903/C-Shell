#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/utsname.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libgen.h>
#include <dirent.h>
#include <grp.h> 
#include <curl/curl.h>
#include <time.h>
#include <termios.h>
#include <ctype.h>
#include <signal.h>
#include <netdb.h>
#include <arpa/inet.h> 
#define path_length 256
#define line_length 256
#define MAX_BG_PROCESSES 20
#define MAX_COMMAND_LENGTH 100
#define MAX_COMMANDS 15
#define MAX_ACTIVE_PROCESSES 20
#define MAX_INPUT_LENGTH 1024
#include "globals.h"
#include "pastevents.h"
#include "warp.h"
#include "peek.h"
#include "seek.h"
#include "proclore.h"
#include "system.h"
#include "prompt.h"
#include "input.h"
#include "iMan.h"
#include "neonate.h"
#include "redirection.h"
#include "ping.h"
#include "activities.h"
#include "ctrl.h"
#include "globals.h"

#endif
