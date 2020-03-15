#ifndef _SERIAL_H_
#define _SERIAL_H_

/** @defgroup Serial Serial
 * @{
 * Contains functions to set up the Serial port device.
 * Contains macros for the Serial port device.
 */


//MACROS
/**
 * @brief COM1 access address
 * 
 */
#define COM1 0x3f8

/**
 * @brief COM" access address
 * 
 */
#define COM2 0x2f8

/**
 * @brief COM1 IRQ LINE
 * 
 */
#define COM1_IRQ 4

/**
 * @brief COM2 IRQ LINE
 * 
 */
#define COM2_IRQ 3

/**
 * @brief UART accessible register RBR
 * 
 */
#define UART_RBR 0

/**
 * @brief UART accessible register THR
 * 
 */
#define UART_THR 0

/**
 * @brief UART interrupt enable register
 * 
 */
#define UART_IER 1

/**
 * @brief UART interrupt identification register
 * 
 */
#define UART_IIR 2

/**
 * @brief UART FIFO control register
 * 
 */
#define UART_FCR 2

/**
 * @brief UART line control register
 * 
 */
#define UART_LCR 3 

/**
 * @brief UART modem control register
 * 
 */
#define UART_MCR 4

/**
 * @brief UART line status register
 * 
 */
#define UART_LSR 5

/**
 * @brief UART modem status register
 * 
 */
#define UART_MSR 6

/**
 * @brief UART scratchpad register
 * 
 */
#define UART_SR 7 //Scatchpad register

//LSR - LINE STATUS REGISTER
/**
 * @brief LSR received data bit
 * 
 */
#define REC_DATA BIT(1)

/**
 * @brief LSR Transmitter holding register empty
 * 
 */
#define TRANS_HOL_REG_EMPTY BIT(5) //Transmitter holding register empty

//IER - INTERRUPT ENABLE REGISTER
/**
 * @brief IER enabled data received bit
 * 
 */
#define EN_DATA_RECEIVED BIT(0)

/**
 * @brief IER enabled Transmitter holding register empty bit
 * 
 */
#define EN_TRANS_HOL_REG_EMPTY BIT(1)

//IIR - INTERRUPT IDENTIFICATION REGISTER
/**
 * @brief IIR got interrupt bit
 * 
 */
#define GOT_INTERRUPT BIT(0)

/**
 * @brief IIR receiver line status
 * 
 */
#define RECEIVER_LINE_STATUS BIT(1) | BIT(2)

/**
 * @brief IIR received data available
 * 
 */
#define RECEIVED_DATA_AVAILABLE BIT(2)

/**
 * @brief IIR transmitter holding register empty
 * 
 */
#define TRANS_HOL_REG_EMPTY_IIR BIT(1)

/**
 * @brief COM1 interrupt subscription for reading
 * 
 * @param bit_no - hook id
 */
void serial_subscribe1_read_int(uint8_t *bit_no);

/**
 * @brief unsubscribe COM1 interrupt for reading
 * 
 */
void serial_unsubscribe1_read_int();

/**
 * @brief COM1 interrupt subscription for transmitting
 * 
 * @param bit_no - hook id
 */
void serial_subscribe1_send_int(uint8_t *bit_no);

/**
 * @brief unsubscribe COM1 interrupt for transmitting
 * 
 */
void serial_unsubscribe1_send_int();

/**
 * @brief COM2 interrupt subscription for reading
 * 
 * @param bit_no - hook id
 */
void serial_subscribe2_read_int(uint8_t *bit_no);

/**
 * @brief unsubscribe COM2 interrupt for reading
 * 
 */
void serial_unsubscribe2_read_int();

/**
 * @brief COM2 interrupt subscription for transmitting
 * 
 * @param bit_no - hook id
 */
void serial_subscribe2_send_int(uint8_t *bit_no);

/**
 * @brief unsubscribe COM2 interrupt for transmitting
 * 
 */
void serial_unsubscribe2_send_int();

/**
 * @brief Subscribe interrupts for both serial ports
 * 
 * @param device - device to be subscribed
 * @param hook_id1 - hook id for COM1
 * @param hook_id2 - hook id for COM2
 */
void serial_subscribe_int(int device, uint8_t *hook_id1, uint8_t *hook_id2);

/**
 * @brief Un
 * 
 * @param device - device to be unsubscribed
 */
void serial_unsubscribe_int(int device);

/**
 * @brief Receives information from other device
 * 
 * @param device - device that receives information
 * @return uint32_t - information received
 */
uint32_t receive_info(int device);

/**
 * @brief Sends information to other device
 * 
 * @param info - information to be sent
 * @param device - device that sends information
 */
void send_info(uint32_t info, int device);

/**
 * @brief Interrupt handler
 * 
 * @param device - device to be handled
 */
void serial_ih(int device);

/**
 * @brief Detects if connection has been established
 * 
 * @param device 
 * @return true - connection established
 * @return false - waiting for connection
 */
bool connection_linker(int device);

/**@} end of Serial*/

#endif
