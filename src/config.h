/* 
 *  path to the battery directory
 *  in this directory there must atleast be a file named "capacity"
 *  for the current battery capacity as a percentage
 *  and a file called "status" that shows the current status like
 *  "Charging", "Discharging", etc..
 */
#ifdef DEBUG
#define BAT_PATH "./testbat"
#else
#define BAT_PATH "/sys/class/power_supply/BAT0"
#endif

/*
 *  the amount of time in seconds to sleep before checking the battery percentage
 */
#ifdef DEBUG
#define SLEEP_TIME 1
#else
#define SLEEP_TIME 30
#endif

/*
 *  battery levels in percentage
 */
#define BAT_LEVEL_FULL 95
#define BAT_LEVEL_LOW 20
#define BAT_LEVEL_CRITICAL 10

/*
 *  log directory path
 */
#define LOG_FILE_PATH "/var/log/battery-notification/"
