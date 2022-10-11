#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include <libnotify/notify.h>
#include <glib.h>

#ifdef DEBUG
#include ".config-debug.h"
#else
#include "config.h"
#endif
#include "battery_states.h"

#define VERSION "1.0.0-release"

// TODO add logfile
#define LOG(str...) \
    fprintf(stderr, str);

NotifyNotification *BatteryCriticalNotification = NULL;
NotifyNotification *BatteryLowNotification = NULL;
NotifyNotification *BatteryFullNotification = NULL;
NotifyNotification *BatteryChargingNotification = NULL;
NotifyNotification *BatteryStopChargingNotification = NULL;

int get_battery_level(void) {
    // open file
    FILE *capacity_file = fopen(BAT_PATH "/capacity", "r");
    if (capacity_file == NULL) {
        LOG("Error: Could not open '" BAT_PATH "/capacity'!\n");
        return -1;
    }
    // reading file
    char line[4] = {0};
    fread(line, 3, 1, capacity_file);
    line[3] = '\0'; // NULL terminate
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

int is_charging(void) {
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

void init(void) {
    //TODO add logfile entry
    // init libnotify
    if (!notify_init("Battery Notification")) {
        LOG("Error: Could not initiate libnotify!\n");
        exit(-1);
    }
    // create notifications
    BatteryCriticalNotification = notify_notification_new("Battery Crtical", "Battery level is critically low!", "battery");
    BatteryLowNotification = notify_notification_new("Battery Low", "Battery level is low", "battery");
    BatteryFullNotification = notify_notification_new("Battery Full", "Battery level is full", "battery");
    BatteryChargingNotification = notify_notification_new("Battery Charging", "Battery is now charging", "battery");
    BatteryStopChargingNotification = notify_notification_new("Battery Stopped Charging!", "Battery stopped charging", "battery");
    // set notification urgency
    notify_notification_set_urgency(BatteryCriticalNotification, NOTIFY_URGENCY_CRITICAL);
    notify_notification_set_urgency(BatteryLowNotification, NOTIFY_URGENCY_CRITICAL);
    notify_notification_set_urgency(BatteryFullNotification, NOTIFY_URGENCY_NORMAL);
    notify_notification_set_urgency(BatteryChargingNotification, NOTIFY_URGENCY_NORMAL);
    notify_notification_set_urgency(BatteryStopChargingNotification, NOTIFY_URGENCY_NORMAL);
}

void uninit(void) {
    // uninit libnotify
    if (notify_is_initted())
        notify_uninit();
}

void sig_handler(int sig) {
    // TODO add logfile entry
    // uninit
    uninit();
    // exit
    exit(0);
}

    // show
    //notify_notification_show(notif, NULL);
int main(int argc, char **argv) {
    // handle arguments
    for(int i = 1; i < argc; i++) {
        char *str = argv[i];
        for(int i2 = 0; i2 < strlen(str); i2++) {
            char c = str[i2];
            
        }
    }

    // init
    init();
    // add ctrl+c signal handler
    signal(SIGINT, sig_handler);
    // main battery check loop
    int last_state = BAT_STATE_UNDEFINED;
    GError *g_err = NULL;
    while (1) {
        if (g_err != NULL)
            g_error_free(g_err);
        // sleep
        sleep(SLEEP_TIME);
        // get battery percentage
        int level = get_battery_level();    
        if (level == -1) {
            LOG("Error: Could not get battery capacity: %s\n", g_err->message);
            exit(-1);
        }
        // check if charging
        if (is_charging()) {
            if (last_state != BAT_STATE_CHARGING) {
#ifdef DEBUG
                puts("CHARGING NOTIFICATION");
#endif
                if (!notify_notification_show(BatteryChargingNotification, &g_err)) {
                    // TODO add log entry
                    LOG("Error: Could now show Charging notification: %s\n", g_err->message);
                }
                last_state = BAT_STATE_CHARGING;
            }
            continue;
        } else if (last_state == BAT_STATE_CHARGING) {
#ifdef DEBUG
            puts("STOP CHARGING NOTIFICATION");
#endif
            if (!notify_notification_show(BatteryStopChargingNotification, &g_err)) {
                // TODO add log entry
                LOG("Error: Could now show Stop Charging notification: %s\n", g_err->message);
            }
            last_state = BAT_STATE_UNDEFINED;
            continue;
        }
        // check battery percentage
        if (level < BAT_LEVEL_CRITICAL) {
            if (last_state != BAT_STATE_CRITICAL) {
#ifdef DEBUG
                puts("CRITICAL NOTIFICATION");
#endif
                if (!notify_notification_show(BatteryCriticalNotification, &g_err)) {
                    // TODO add log entry
                    LOG("Error: Could now show Critical notification: %s\n", g_err->message);
                }
                last_state = BAT_STATE_CRITICAL;
            }
            continue;
        }
        else if (level < BAT_LEVEL_LOW) {
            if (last_state != BAT_STATE_LOW) {
#ifdef DEBUG
                puts("LOW NOTIFICATION");
#endif
                if (!notify_notification_show(BatteryLowNotification, &g_err)) {
                    // TODO add log entry
                    LOG("Error: Could now show Low notification: %s\n", g_err->message);
                }
                last_state = BAT_STATE_LOW;
            }
            continue;
        }
        else if (level >= BAT_LEVEL_FULL) {
            if (last_state != BAT_STATE_FULL) {
#ifdef DEBUG
                puts("FULL NOTIFICATION");
#endif
                if (!notify_notification_show(BatteryFullNotification, &g_err)) {
                    // TODO add log entry
                    LOG("Error: Could now show Full notification: %s\n", g_err->message);
                }
                last_state = BAT_STATE_FULL;
            }
            continue;
        }
        // TODO add logfile entry
    }
}
