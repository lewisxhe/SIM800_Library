#include "sim800.h"

SIM800::SIM800(HardwareSerial &port, int pwkey, int rst, int pwctrl)
{
    _port = &port;
    _pwctrl = pwctrl;
    _pwkey = pwkey;
    _rst = rst;
};


bool SIM800::preInit()
{
    if (_pwctrl > 0) {
        pinMode(_pwctrl, OUTPUT);
        digitalWrite(_pwctrl, HIGH);
    }

    pinMode(_pwkey, OUTPUT);
    digitalWrite(_pwkey, LOW);
    delay(1000);
    digitalWrite(_pwkey, HIGH);


    pinMode(_rst, OUTPUT);
    digitalWrite(_rst, HIGH);
    delay(1);
    digitalWrite(_rst, LOW);
    delay(1);
    digitalWrite(_rst, HIGH);

    delay(10);

    if (sendATTest() != 0) {
        digitalWrite(_pwkey, HIGH);
        delay(500);
        digitalWrite(_pwkey, LOW);
        delay(1000);
        digitalWrite(_pwkey, HIGH);
        delay(500);
    }
    if (sendATTest() != 0) {
        ERROR("Begin SIM800 FAIL");
        return false;
    } else {
        ERROR("Begin SIM800 PASS");
    }
    return true;
}

int SIM800::checkReadable(void)
{
    return _port->available();
}

int SIM800::readBuffer(char *buffer, int count, unsigned int timeOut)
{
    int i = 0;
    unsigned long timerStart, timerEnd;
    timerStart = millis();
    while (1) {
        while (_port->available()) {
            char c = _port->read();
            if (c == '\r' || c == '\n') c = '$';
            buffer[i++] = c;
            if (i > count - 1)break;
        }
        if (i > count - 1)break;
        timerEnd = millis();
        if (timerEnd - timerStart > 1000 * timeOut) {
            break;
        }
    }
    delay(500);
    while (_port->available()) {  // display the other thing..
        _port->read();
    }
    return 0;
}

void SIM800::cleanBuffer(char *buffer, int count)
{
    for (int i = 0; i < count; i++) {
        buffer[i] = '\0';
    }
}

void SIM800::sendCmd(const char *cmd)
{
    _port->write(cmd);
}

int SIM800::sendATTest(void)
{
    int ret = sendCmdAndWaitForResp("AT\r\n", "OK", DEFAULT_TIMEOUT);
    return ret;
}

int SIM800::waitForResp(const char *resp, unsigned int timeout)
{
    int len = strlen(resp);
    int sum = 0;
    unsigned long timerStart, timerEnd;
    timerStart = millis();

    while (1) {
        if (_port->available()) {
            char c = _port->read();
            sum = (c == resp[sum]) ? sum + 1 : 0;
            if (sum == len)break;
        }
        timerEnd = millis();
        if (timerEnd - timerStart > 1000 * timeout) {
            return -1;
        }
    }

    while (_port->available()) {
        _port->read();
    }

    return 0;
}

void SIM800::sendEndMark(void)
{
    _port->println((char)26);
}


int SIM800::sendCmdAndWaitForResp(const char *cmd, const char *resp, unsigned timeout)
{
    sendCmd(cmd);
    return waitForResp(resp, timeout);
}

void SIM800::serialDebug(void)
{
    while (1) {
        if (_port->available()) {
            Serial.write(_port->read());
        }
        if (Serial.available()) {
            _port->write(Serial.read());
        }
    }
}
