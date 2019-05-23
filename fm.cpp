#include "fm.h"

int FM::powerOn(void)
{
    if(0 == fmPower){
        if(0 != sendCmdAndWaitForResp("AT+FMOPEN?\r\n", "+FMOPEN: 1,1\r\n", DEFAULT_TIMEOUT))
        {
            if(0 != sendCmdAndWaitForResp("AT+FMOPEN=1\r\n", "OK", DEFAULT_TIMEOUT)) {  // connect tcp
                ERROR("\r\nERROR:fmPowerOn\r\n");
                return -1;
            }
            fmPower = 1;
        }
        else
            fmPower = 1;
    }
    
    return 0;
}

int FM::powerOff(void)
{
    if(1 == fmPower){
        if(0 != sendCmdAndWaitForResp("AT+FMCLOSE\r\n", "OK", DEFAULT_TIMEOUT)) {
            ERROR("\r\nERROR:fmPowerOff\r\n");
            return -1;
        }
    }
    fmPower = 0;
    return 0;
}

int FM::setFreq(int freq)
{
    char cmd[30];
    if(!fmPower)
        return -1;
        
    if(freq >= 875 && freq <= 1080){ //87.5MHZ~108.0MHZ
        sprintf(cmd,"AT+FMFREQ=%d\r\n",freq);
        if(0 != sendCmdAndWaitForResp(cmd, "OK", DEFAULT_TIMEOUT)){
            ERROR("\r\nERROR:fmSetFreq\r\n");
            return -1;
        }
        if(0 != sendCmdAndWaitForResp("AT+CHFA=0\r\n", "OK", DEFAULT_TIMEOUT)){
            ERROR("\r\nERROR:AT+CHFA\r\n");
            return -1;
        }
    }
    return 0;
}

int FM::setVolume(int volume)
{
    char cmd[30];
    if(volume >= 0 && volume <=6){
        sprintf(cmd,"AT+FMVOLUME=%d\r\n",volume);
        if(0 != sendCmdAndWaitForResp(cmd, "OK", DEFAULT_TIMEOUT)){
            ERROR("\r\nERROR:fmSetVolume\r\n");
            return -1;
        }
    }    
    return 0;
}

int FM::scanChannel(void)
{
    char gprsBuffer[128];
    char tmp[5];
    int j = 0;
    if(!fmPower)
        return -1;
        
    fmChannelCount = 0;
    fmCurrentChannel = 0;
    cleanBuffer(gprsBuffer,128);
    cleanBuffer(tmp,5);
    sendCmd("AT+FMSCAN\r\n");
    readBuffer(gprsBuffer,128,DEFAULT_TIMEOUT*10);
    for(int i = 0; i < 128; i++){
        if(gprsBuffer[i] >= '0' && gprsBuffer[i] <= '9'){
            tmp[j++] = gprsBuffer[i];
        }else{
            if(j){
                int channelFreq = atoi(tmp);
                if(channelFreq <= 1080 && channelFreq >= 875){
                    fmChannel[fmChannelCount++] =  channelFreq;
                    j = 0;
                    cleanBuffer(tmp,5);
                }
            }
        }
    }
    if(fmChannelCount == 0)
        ERROR("\r\nWARNING: no Channel!\r\n");
    return 0;
}

int FM::checkSignalStrength(int channelFreq)
{
    char cmd[30];
    char gprsBuffer[30];
    int signalStrength;
    if(!fmPower)
        return -1;
    
    sprintf(cmd,"AT+FMSIGNAL=%d\r\n",channelFreq);
    sendCmd(cmd);
    readBuffer(gprsBuffer,sizeof(gprsBuffer),DEFAULT_TIMEOUT);
    if(sscanf(gprsBuffer, "$$+FMSIGNAL: freq[%d]:%d",&channelFreq, &signalStrength) > 0) {
        if(signalStrength >= 0 && signalStrength <= 112){
            return signalStrength;
        }
    }
    return -1;
}

int FM::channelNext(void)
{
    if(fmChannelCount == 0)
        return -1;
    if(fmCurrentChannel == fmChannelCount)
        fmCurrentChannel = 0;
    setFreq(fmChannel[fmCurrentChannel++]);
    return 0;
}