#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "config.h"
#include "battery_states.h"

// TODO add logfile
#define LOG(str...) \
    fprintf(stderr, str);

int get_battery_level(void) {
    // open file
    FILE *capacity_file = fopen(BAT_PATH "/capacity", "r");
    if (capacity_file == NULL) {
        LOG("Error: Could not open '" BAT_PATH "/capacity'!\n");
        return -1;
    }
    // reading file
    char line[128] = {0};
    fread(line, 127, 1, capacity_file);
    line[127] = '\0'; // NULL terminate
    if (ferror(capacity_file)) {
        LOG("Error: Reading '" BAT_PATH " /capacity' " "failed!\n");
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

int is_charging() {
    // open file
    FILE *status_file = fopen(BAT_PATH "/status", "r");
    if (status_file == NULL) {
        LOG("Error: Could not open '" BAT_PATH "/status'!\n");
        return -1;
    }
    // reading file
    char line[9] = {0};
    fread(line, 8, 1, status_file);
    line[8] = '\0'; // NULL terminate
    if (ferror(status_file)) {
        LOG("Error: Reading '" BAT_PATH " /status' " "failed!\n");
        return -1;
    }
    fclose(status_file);
    // check if charging by comparing strings
    if (strcmp("Charging", line) == 0) {
        return 1;
    }
    return 0;
}

void sig_handler(int sig) {
    // TODO add logfile entry
    exit(0);
}

int main(int argc, char **argv) {
    // add ctrl+c signal handler
    signal(SIGINT, sig_handler);
    // main battery check loop
    int last_state = BAT_STATE_UNDEFINED;
    while (1) {
        // sleep
        sleep(SLEEP_TIME);
        // get battery percentage
        int level = get_battery_level();    
        if (level == -1) {
            LOG("Error: Could not get battery capacity!\n");
            exit(-1);
        }
        // check if charging
        if (is_charging()) {
            if (last_state != BAT_STATE_CHARGING) {
                puts("CHARGING NOTIFICATION");
                last_state = BAT_STATE_CHARGING;
            }
            continue;
        } else if (last_state == BAT_STATE_CHARGING) {
            puts("STOP CHARGING NOTIFICATION");
            last_state = BAT_STATE_UNDEFINED;
            continue;
        }
        // check battery percentage
        if (level < BAT_LEVEL_CRITICAL) {
            if (last_state != BAT_STATE_CRITICAL) {
                puts("CRITICAL NOTIFICATION");
                last_state = BAT_STATE_CRITICAL;
            }
            continue;
        }
        else if (level < BAT_LEVEL_LOW) {
            if (last_state != BAT_STATE_LOW) {
                puts("LOW NOTIFICATION");
                last_state = BAT_STATE_LOW;
            }
            continue;
        }
        else if (level >= BAT_LEVEL_FULL) {
            if (last_state != BAT_STATE_FULL) {
                puts("FULL NOTIFICATION");
                last_state = BAT_STATE_FULL;
            }
            continue;
        }
        // TODO add logfile entry
    }
}
