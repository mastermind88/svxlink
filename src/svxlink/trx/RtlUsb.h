/**
@file	 RtlUsb.h
@brief   An interface class for communicating to a RTL dongle through USB
@author  Tobias Blomberg / SM0SVX
@date	 2015-04-10

\verbatim
SvxLink - A Multi Purpose Voice Services System for Ham Radio Use
Copyright (C) 2003-2015 Tobias Blomberg / SM0SVX

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
\endverbatim
*/

#ifndef RTL_USB_INCLUDED
#define RTL_USB_INCLUDED


/****************************************************************************
 *
 * System Includes
 *
 ****************************************************************************/

#include <string>
#include <rtl-sdr.h>
#include <pthread.h>


/****************************************************************************
 *
 * Project Includes
 *
 ****************************************************************************/

#include <AsyncTimer.h>


/****************************************************************************
 *
 * Local Includes
 *
 ****************************************************************************/

#include "RtlSdr.h"


/****************************************************************************
 *
 * Forward declarations
 *
 ****************************************************************************/

namespace Async
{
  class FdWatch;
};


/****************************************************************************
 *
 * Namespace
 *
 ****************************************************************************/

//namespace MyNameSpace
//{


/****************************************************************************
 *
 * Forward declarations of classes inside of the declared namespace
 *
 ****************************************************************************/

  

/****************************************************************************
 *
 * Defines & typedefs
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Exported Global Variables
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Class definitions
 *
 ****************************************************************************/

/**
@brief  An interface class for communicating to a RTL dongle through USB
@author Tobias Blomberg / SM0SVX
@date   2014-07-16

Use this class to communicate to an RTL2832u DVB-T dongle using USB.
*/
class RtlUsb : public RtlSdr
{
  public:
    /**
     * @brief 	Constructor
     * @param   match Device match string
     */
    RtlUsb(const std::string &match);
  
    /**
     * @brief 	Destructor
     */
    virtual ~RtlUsb(void);
  
    /**
     * @brief   Find out if the RTL dongle is ready for operation
     * @returns Returns \em true if the dongle is ready for operation
     */
    virtual bool isReady(void) const { return dev != NULL; }

    /**
     * @brief   Return a string which identifies the specific dongle
     * @returns Returns a string that uniquely identifies the dongle
     *
     * This function returns a string that uniquely identifies the specific
     * dongle used for this instance of RtlSdr. The string is for example used
     * when printing out messages associated with the dongle.
     */
    virtual const std::string displayName(void) const { return dev_name; }

  protected:
    /**
     * @brief   Set tuner IF gain for the specified stage
     * @param   stage The number of the gain stage to set
     * @param   gain The gain in tenths of a dB to set (105=10.5dB)
     *
     * Use this function to set the IF gain for a specific stage in the tuner.
     * How many stages that are available is tuner dependent. For example,
     * the E4000 tuner have stages 1 to 6.
     */
    virtual void handleSetTunerIfGain(uint16_t stage, int16_t gain);

    /**
     * @brief   Set the center frequency of the tuner
     * @param   fq The new center frequency, in Hz, to set
     */
    virtual void handleSetCenterFq(uint32_t fq);

    /**
     * @brief   Set the tuner sample rate
     * @param   rate The new sample, in Hz, rate to set
     */
    virtual void handleSetSampleRate(uint32_t rate);

    /**
     * @brief   Set the gain mode
     * @param   mode The gain mode to set: 0=automatic, 1=manual
     *
     * Use this function to choose if automatic or manual gain mode should
     * be used. When set to manual, the setGain function can be used to set
     * the desired gain.
     */
    virtual void handleSetGainMode(uint32_t mode);

    /**
     * @brief   Set manual gain
     * @param   gain The gain in tenths of a dB to set (105=10.5dB)
     *
     * Use this function to set the gain when manual gain mode is selected.
     * Set the gain mode using the setGainMode function.
     */
    virtual void handleSetGain(int32_t gain);

    /**
     * @brief   Set frequency correction factor
     * @param   corr The frequency correction factor in PPM
     *
     * Use this function to set the frequency correction factor for the tuner.
     * The correction factor is given in parts per million (PPM). That is,
     * how many Hz per MHz the tuner is off.
     */
    virtual void handleSetFqCorr(int corr);

    /**
     * @brief   Enable or disable test mode
     * @param   enable Set to \em true to enable testing
     *
     * Use this function to enable a testing mode in the tuner. Instead of
     * returning real samples the tuner will return a 8 bit counter value
     * instead. This can be used to verify that no samples are dropped.
     */
    virtual void handleEnableTestMode(bool enable);

    /**
     * @brief   Enable or disable the digital AGC of the RTL2832
     * @param   enable Set to \em true to enable the digital AGC
     */
    virtual void handleEnableDigitalAgc(bool enable);

    
  private:
    class SampleBuffer;

    static const unsigned RECONNECT_INTERVAL = 5000;

    Async::Timer    reconnect_timer;
    rtlsdr_dev_t    *dev;
    pthread_t       rtl_reader_thread;
    std::string     dev_match;
    std::string     dev_name;
    SampleBuffer    *sample_buf;
    bool            rtl_reader_thread_started;

    static void *startRtlReader(void *data);
    static void rtlsdrCallback(unsigned char *buf, uint32_t len, void *ctx);

    RtlUsb(const RtlUsb&);
    RtlUsb& operator=(const RtlUsb&);
    void rtlReader(void);
    void rtlSamplesReceived(void);
    void initializeDongle(void);
    void verboseClose(void);
    int verboseDeviceSearch(const char *s);
    
};  /* class RtlUsb */



//} /* namespace */

#endif /* RTL_USB_INCLUDED */


/*
 * This file has not been truncated
 */
