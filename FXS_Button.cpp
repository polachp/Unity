#include "FXS_Button.h"
#include <stdlib.h>

Button:: Button()
{
  // Initialization of properties
  Debounce = 20;
  DblClickDelay = 250;
  LongPressDelay = 1000;
  VLongPressDelay = 6000;
 
  // Initialization of variables
  _state = true;
  _lastState = true;
  _dblClickWaiting = false;
  _dblClickOnNextUp = false;
  _singleClickOK = false; //Default = true
  _downTime = -1;
  _upTime = -1;
  _ignoreUP = false;
  _waitForUP = false;
  _longPressHappened = false;
  _vLongPressHappened = false;
}

void Button::Configure(int pin, int pullMode)
{
  _pin = pin;
  _pullMode =  pullMode;
  pinMode(_pin, INPUT_PULLUP);
}

void Button::CheckBP(void)
{
  int resultEvent = 0;
  long millisRes = millis();
  _state = digitalRead(_pin) == HIGH;

  // Button pressed down
  if (_state != _pullMode && _lastState == _pullMode && (millisRes - _upTime) > Debounce)
  {
    _downTime = millisRes;
    _ignoreUP = false;
    _waitForUP = false;
    _singleClickOK = true;
    _longPressHappened = false;
    _vLongPressHappened = false;
    if ((millisRes - _upTime) < DblClickDelay && _dblClickOnNextUp == false && _dblClickWaiting == true)
      _dblClickOnNextUp = true;
    else
      _dblClickOnNextUp = false;
    _dblClickWaiting = false;
  }
  // Button released
  else if (_state == _pullMode && _lastState != _pullMode && (millisRes - _downTime) > Debounce)
  {
    if (_ignoreUP == false) //Replace "(!_ignoreUP)" by "(not _ignoreUP)"
    {
      _upTime = millisRes;
      if (_dblClickOnNextUp == false) _dblClickWaiting = true;
      else
      {
        resultEvent = 2;
        _dblClickOnNextUp = false;
        _dblClickWaiting = false;
        _singleClickOK = false;
      }
    }
  }

  // Test for normal click event: DblClickDelay expired
  if (_state == _pullMode && (millisRes - _upTime) >= DblClickDelay && _dblClickWaiting == true && _dblClickOnNextUp == false && _singleClickOK == true && resultEvent != 2)
  {
    resultEvent = 1;
    _dblClickWaiting = false;
  }

  // Test for hold
  if (_state == _pullMode && _lastState != _pullMode && (millisRes - _downTime) >= LongPressDelay && (millisRes - _downTime) < VLongPressDelay)
  {
    // Trigger "normal" hold
    if (_longPressHappened == false)
    {
      resultEvent = 3;
      _waitForUP = true;
      _ignoreUP = true;
      _dblClickOnNextUp = false;
      _dblClickWaiting = false;
      //_downTime = millis();
      _longPressHappened = true;
    }
  }
  // Test for hold
  if (_state != _pullMode && (millisRes - _downTime) >= VLongPressDelay)
  {
    // Trigger "normal" hold
    if (_longPressHappened == false)
    {
      resultEvent = 4;
      _waitForUP = true;
      _ignoreUP = true;
      _dblClickOnNextUp = false;
      _dblClickWaiting = false;
      //_downTime = millis();
      _longPressHappened = true;
    }
  }
  

  _lastState = _state;

  if (resultEvent == 1 && OnClick) OnClick(_pin);
  if (resultEvent == 2 && OnDblClick) OnDblClick(_pin);
  if (resultEvent == 3 && OnLongPress) OnLongPress(_pin);
  if (resultEvent == 4 && OnVLongPress) OnVLongPress(_pin);
  }

