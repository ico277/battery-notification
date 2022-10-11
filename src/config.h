/* 
 *  path to the battery directory
 *  in this directory there must atleast be a file named "capacity"
 *  for the current battery capacity as a percentage
 *  and a file called "status" that shows the current status like
 *  "Charging", "Discharging", etc..
 */
#define BAT_PATH "/sys/class/power_supply/BAT0"

/*
 *  the amount of time in seconds to sleep before checking the battery percentage
 */
#define SLEEP_TIME 30

/*
 *  battery levels in percentage
 */
#define BAT_LEVEL_FULL 99
#define BAT_LEVEL_LOW 20
#define BAT_LEVEL_CRITICAL 10
