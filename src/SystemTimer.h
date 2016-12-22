/* 
 * File:   SystemTimer.h
 * Author: ben
 *
 * Created on 3 dicembre 2009, 20.39
 */

#ifndef _SYSTEMTIMER_H
#define	_SYSTEMTIMER_H

#include <sys/time.h>

#include "Component.h"

class SystemTimer : public Component {
public:

  enum TimerFeatures : uint32_t {
    TIMER_0100_HZ =     (1 <<  0),
    TIMER_0250_HZ =     (1 <<  1),
    TIMER_0300_HZ =     (1 <<  2),
    TIMER_1000_HZ =     (1 <<  3),
  };

  SystemTimer(const TimerFeatures & features)
  : timerFeatures(features), timerTimeout(0), timePassed(0), lastTimeCheck(0)
  { };

  void put(const ComponentRequestType & request, const int32_t & arg);

  void checkInterruptEvents();

  void setTimer(const TimerFeatures &);
  void stopTimer() { timerTimeout = 0; }
private:

  TimerFeatures timerFeatures;

  uint32_t timerTimeout;
  uint64_t timePassed;

  suseconds_t lastTimeCheck;
};

#endif	/* _SYSTEMTIMER_H */

