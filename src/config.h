/* 
 *  path to the battery directory
 *  in this directory there must atleast be a file named "capacity"
 *  for the current battery capacity as a percentage
 */
#define BATTERY_PATH "/sys/class/power_supply/BAT0"

/*
 *  the amount of time in seconds to sleep before checking the battery percentage
 */
#define SLEEP_TIME 60

/*
 *  battery levels in percentage
 */
#define BATTERY_LEVEL_LOW 15
#define BATTERY_LEVEL_CRITICAL 5

/*
 *  log directory path
 *  to disable logs comment this out
 */
#define LOG_FILE_PATH "/var/log/battery-notification/"
