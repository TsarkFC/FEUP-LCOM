#ifndef _MACRO_KEYBOARD_H_
#define _MACRO_KEYBOARD_H_

/** @defgroup Macro_Keyboard Macro_Keyboard
 * @{
 * Contains macros related to the keyboard device.
 */

/**
 * @brief STATUS REGISTER
 * 
 */
#define STAT_REG    0x64

/**
 * @brief Keyboar command register
 * 
 */
#define KBC_CMD_REG 0x64

/**
 * @brief Input buffer
 * 
 */
#define IN_BUF 0x60

/**
 * @brief output buffer
 * 
 */
#define OUT_BUF 0x60

/**
 * @brief ESC key code
 * 
 */
#define ESQ_KEY 0x81

/**
 * @brief fisrt byte of key which have 2 byte break/make codes
 * 
 */
#define TWO_BYTE 0xE0 //first byte of a two byte scancode

/**
 * @brief Macro that allows to check if we have a break or make code
 * 
 */
#define BREAK_MAKE_CODE 0x80 //tests if break or make code when used

/**
 * @brief Waiting time during operations
 * 
 */
#define WAIT_KBC 20000 

/**
 * @brief Checks parity errors
 * 
 */
#define PAR_ERR 0x80

/**
 * @brief Checks time out errors
 * 
 */
#define TO_ERR 0x40

/**
 * @brief Read information command
 * 
 */
#define READ_COMMAND 0x20

/**
 * @brief Write information command
 * 
 */
#define WRITE_COMMAND 0X60

/**
 * @brief Allows to enable interrupts after polling operation
 * 
 */
#define EN_INT_OBF BIT(0)|BIT(1) //enable interrupts on output buffer from mouse and keyboard

/**
 * @brief OBF, allows to check if output buffer is full
 * 
 */
#define OBF BIT(0) //bit 0 to 1, checks if output buffer is full

/**
 * @brief IBF, allow to check if input buffer is full
 * 
 */
#define IBF BIT(1) //bit 1 to 1, checks if input buffer is full

/**
 * @brief keyboard IRQ line
 * 
 */
#define KBD_IRQ 1  //keyboard irq number

/**
 * @brief up key break code
 * 
 */
#define UP_KEY_BREAK 0xc8 //break code of up arrow key

/**
 * @brief down key break code
 * 
 */
#define DOWN_KEY_BREAK 0xd0 //break code of down arrow key

/**
 * @brief enter key break code
 * 
 */
#define ENTER_KEY_BREAK 0x9c //break code of enter key

/**
 * @brief right arrow key break code
 * 
 */
#define RIGHT_ARROW_BREAK 0xcd //break code of right arrow key

/**
 * @brief left arrow key break code
 * 
 */
#define LEFT_ARROW_BREAK 0xcb //break code of left arrow key

/**
 * @brief space key break code
 * 
 */
#define SPACE_BREAK 0xb9

/**
 * @brief numeric key 1 break code
 * 
 */
#define key_1 0x82

/**
 * @brief numeric key 2 break code
 * 
 */
#define key_2 0x83

/**
 * @brief numeric key 3 break code
 * 
 */
#define key_3 0x84

/**
 * @brief numeric key 4 break code
 * 
 */
#define key_4 0x85

/**
 * @brief numeric key 5 break code
 * 
 */
#define key_5 0x86

/**
 * @brief numeric key 6 break code
 * 
 */
#define key_6 0x87

/**
 * @brief numeric key 7 break code
 * 
 */
#define key_7 0x88

/**
 * @brief numeric key 8 break code
 * 
 */
#define key_8 0x89

/**
 * @brief numeric key 9 break code
 * 
 */
#define key_9 0x8a

/**
 * @brief numeric key 0 break code
 * 
 */
#define key_0 0x8b

/**@} end of Macro_Keyboard*/

#endif
