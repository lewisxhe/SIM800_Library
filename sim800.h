#ifndef __SIM800_H__
#define __SIM800_H__

#include "Arduino.h"


#define UART_DEBUG

#ifdef UART_DEBUG
#define ERROR(x)            Serial.println(x)
#define DEBUG(x)            Serial.println(x);
#else
#define ERROR(x)
#define DEBUG(x)
#endif

#define DEFAULT_TIMEOUT     5

/** SIM800 class.
 *  Used for SIM800 communication. attention that SIM800 module communicate with MCU in serial protocol
 */
class SIM800
{

public:
    /** Create SIM800 Instance
     *  @param tx   uart transmit pin to communicate with SIM800
     *  @param rx   uart receive pin to communicate with SIM800
     *  @param baudRate baud rate of uart communication
     */
    SIM800(HardwareSerial &port, int pwkey, int rst, int pwctrl = -1);


    /** Power on SIM800
     */
    bool preInit(void);

    /** Check if SIM800 readable
     */
    int checkReadable(void);

    /** read from SIM800 module and save to buffer array
     *  @param  buffer  buffer array to save what read from SIM800 module
     *  @param  count   the maximal bytes number read from SIM800 module
     *  @param  timeOut time to wait for reading from SIM800 module
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int readBuffer(char *buffer, int count, unsigned int timeOut = DEFAULT_TIMEOUT);


    /** clean Buffer
     *  @param buffer   buffer to clean
     *  @param count    number of bytes to clean
     */
    void cleanBuffer(char *buffer, int count);

    /** send AT command to SIM800 module
     *  @param cmd  command array which will be send to GPRS module
     */
    void sendCmd(const char *cmd);

    /**send "AT" to SIM800 module
     */
    int sendATTest(void);

    /**send '0x1A' to SIM800 Module
     */
    void sendEndMark(void);

    /** check SIM800 module response before time out
     *  @param  *resp   correct response which SIM800 module will return
     *  @param  *timeout    waiting seconds till timeout
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int waitForResp(const char *resp, unsigned timeout);

    /** send AT command to GPRS module and wait for correct response
     *  @param  *cmd    AT command which will be send to GPRS module
     *  @param  *resp   correct response which GPRS module will return
     *  @param  *timeout    waiting seconds till timeout
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int sendCmdAndWaitForResp(const char *cmd, const char *resp, unsigned timeout);


    /** used for serial debug, you can specify tx and rx pin and then communicate with GPRS module with common AT commands
     */
    void serialDebug(void);

    HardwareSerial *_port;

private:
    int _pwkey,  _rst, _pwctrl;

};

#endif