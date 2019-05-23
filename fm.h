#ifndef __FM_H__
#define __FM_H__

#include "sim800.h"

/** FM class.
 *  used to use FM function
 */
class FM: public SIM800
{

public:
    FM(HardwareSerial &port, int pwkey, int rst, int pwctrl): SIM800( port,  pwkey,  rst,  pwctrl)
    {
        fmChannelCount = 0;
        fmCurrentChannel = 0;
    };

    /** power on FM module
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int powerOn(void);

    /** power off FM module
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int powerOff(void);

    /** set radio frequency
     *  @param  freq    freq that will be chosen
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int setFreq(int freq);

    /** set volume of FM output
     *  @param  volume  volume:0/1/2/3/4/5/6
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int setVolume(int volume);

    /** search for channel
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int scanChannel(void);

    /** check channel's signal strength
     *  @param  channelFreq the channel that will be checked
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int checkSignalStrength(int channelFreq);

    /** jump to next channel
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int channelNext(void);

private:
    /** FM power flag
     */
    bool fmPower;

    /** channel array to save valid channel
     */
    int fmChannel[10];

    /** FM valid channel count record
     */
    int fmChannelCount;

    /** current channel index
     */
    int fmCurrentChannel;
};
#endif