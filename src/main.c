#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

#include "config.h"

// TODO add logfile
#define LOG(str...) \
    fprintf(stderr, str);

int get_battery_level(void) {
    // open file
    FILE *capacity_file = fopen(BATTERY_PATH "/capacity", "r");
    if (capacity_file == NULL) {
        LOG("Error: Could not open '" BATTERY_PATH "/capacity'!\n");
        return -1;
    }
    // reading file
    char line[1024] = {0};
    fread(line, 1023, 1, capacity_file);
    line[1023] = '\0'; // NULL terminate
    if (ferror(capacity_file)) {
        LOG("Error: Reading '" BATTERY_PATH " /capacity' " "failed!\n");
        return -1;
    }
    fclose(capacity_file);
    // converting to int
    errno = 0;
    int level = (int) strtol(line, NULL, 10);
    if (errno != 0) {
        LOG("errno: %d\n", errno);
        return -1;
    }
    return level;
}

void sig_handler(int sig) {
    // TODO add logfile entry
    exit(0);
}

int main(int argc, char **argv) {
    // add ctrl+c signal handler
    signal(SIGINT, sig_handler);
    // main battery check loop
    int last_state = -1;
    while (1) {
        // sleep
        sleep(SLEEP_TIME);
        // get battery percentage
        int level = get_battery_level();    
        if (level == -1) {
            LOG("Error: Could not get battery capacity!\n");
            exit(-1);
        }
        // check battery percentage
        if (level < BATTERY_LEVEL_CRITICAL) {
            if (last_state != 0) {
                puts("CRITICAL NOTIFICATION");
                last_state = 0;
            }
            continue;
        }
        else if (level < BATTERY_LEVEL_LOW) {
            if (last_state != 1) {
                puts("LOW NOTIFICATION");
                last_state = 1;
            }
            continue;
        }
        else if (level >= BATTERY_LEVEL_FULL) {
            if (last_state != 2) {
                puts("FULL NOTIFICATION");
                last_state = 2;
            }
            continue;
        }
        // TODO add logfile entry
    }
}
