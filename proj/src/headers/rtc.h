#ifndef _RTC_H_
#define _RTC_H_

/** @defgroup RTC RTC
 * @{
 * Contains functions to set up the RTC device.
 * Contains macros for the RTC device.
 */

/**
 * @brief RTC address register
 * 
 */
#define RTC_ADDR_REG 0x70

/**
 * @brief RTC data register
 * 
 */
#define RTC_DATA_REG 0x71

/**
 * @brief RTC seconds register
 * 
 */
#define SECONDS 0

/**
 * @brief RTC seconds alarm register
 * 
 */
#define SECONDS_ALARM 1

/**
 * @brief RTC minutes register
 * 
 */
#define MINUTES 2

/**
 * @brief RTC minutes alarm register
 * 
 */
#define MINUTES_ALARM 3

/**
 * @brief RTC hours register
 * 
 */
#define HOURS 4

/**
 * @brief RTC hours alarm register
 * 
 */
#define HOURS_ALARM 5

/**
 * @brief RTC days of the week register
 * 
 */
#define DAYS_OF_THE_WEEK 6

/**
 * @brief RTC day of the week register
 * 
 */
#define DAY 7

/**
 * @brief RTC month of the year register
 * 
 */
#define MONTH 8

/**
 * @brief RTC year register
 * 
 */
#define YEAR 9

/**
 * @brief RTC register A
 * 
 */
#define RTC_REG_A 10

/**
 * @brief RTC register B
 * 
 */
#define RTC_REG_B 11

/**
 * @brief RTC register C
 * 
 */
#define RTC_REG_C 12

/**
 * @brief RTC register D
 * 
 */
#define RTC_REG_D 13

//RECORD
/**
 * @brief RTC register reseverd for the record time in the first level
 * 
 */
#define RECORD 14

/**
 * @brief RTC register reserved for the record time in the second level
 * 
 */
#define RECORD2 15

/**
 * @brief RTC register reserved for the record time in the third level
 * 
 */
#define RECORD3 16

//REGISTER C
/**
 * @brief RTC register C PF
 * 
 */
#define RTC_PF BIT(6)

/**
 * @brief RTC register C AF
 * 
 */
#define RTC_AF BIT(5)

/**
 * @brief RTC register C UF
 * 
 */
#define RTC_UF BIT(4)

//REGISTER A
/**
 * @brief RTC register A UIP
 * 
 */
#define RTC_UIP BIT(7)

//REGISTER B
/**
 * @brief RTC register B SET
 * 
 */
#define SET BIT(7)

/**
 * @brief RTC register B PIE
 * 
 */
#define PIE BIT(6)

/**
 * @brief RTC register B AIE
 * 
 */
#define AIE BIT(5)

/**
 * @brief RTC register B UIE
 * 
 */
#define UIE BIT(4)

/**
 * @brief RTC register B DM
 * 
 */
#define DM BIT(2)

/**
 * @brief RTC IRQ line
 * 
 */
#define RTC_IRQ 8

/**
 * @brief RTC interruput handler
 * 
 */
void rtc_ih();

/**
 * @brief Waits for valid values received from the RTC
 * 
 */
void wait_valid_rtc();

/**
 * @brief RTC interrupt subscription
 * 
 * @param bit_no 
 * @return int 
 */
int rtc_subscribe_int(uint8_t * bit_no);

/**
 * @brief RTC interrupt unsubscription
 * 
 * @return int 
 */
int rtc_unsubscribe_int();

/**
 * @brief Inhibits RTC updates by setting SET of register B
 * 
 */
void inhibit_updates_true();

/**
 * @brief Unsets SET of register B, enabling updates
 * 
 */
void inhibit_updates_false();

/**
 * @brief Get the hour of the RTC
 * 
 * @return uint32_t - hour
 */
uint32_t get_hour();

/**
 * @brief Set the hour of the RTC
 * 
 * @param hour - hour to be set
 */
void set_hour(uint32_t hour);

/**
 * @brief Get the minutes of the RTC
 * 
 * @return uint32_t - minutes
 */
uint32_t get_minutes();

/**
 * @brief Set the minutes of the RTC
 * 
 * @param minutes - minutes to bet set
 */
void set_minutes(uint32_t minutes);

/**
 * @brief Get the seconds of the RTC
 * 
 * @return uint32_t - seconds
 */
uint32_t get_seconds();

/**
 * @brief Handles updates during the RTC interrupt handler
 * 
 * @return int 
 */
int handle_update_int();

/**
 * @brief Updates the record contained in the port that represensts a game level
 * 
 * @param time - time to be updated as record
 * @param port - port which contains previous record to the corresponding level
 */
void update_record(uint32_t time, int port);

/**
 * @brief Get the record from RTC for level1
 * 
 * @return uint32_t - record
 */
uint32_t get_record1();

/**
 * @brief Get the record from RTC for level2
 * 
 * @return uint32_t - record
 */
uint32_t get_record2();

/**
 * @brief Get the record from RTC for level3
 * 
 * @return uint32_t - record
 */
uint32_t get_record3();

/**
 * @brief Draws background color of the game accordingly to the time of the day
 * 
 */
void draw_background_color();

/**@} end of RTC*/

#endif
