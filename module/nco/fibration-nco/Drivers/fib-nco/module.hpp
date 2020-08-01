/*
    A general module class for modular synth project Fibration.
    Copyright (C) 2020 Lukas Neverauskis

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "stdint.h"
#include <cstdlib>
#include <vector>

namespace FIB {

////////////////////////////////////////////////////////////////////////////////
// Hardware wrapper
////////////////////////////////////////////////////////////////////////////////

#include "main.h"

extern ADC_HandleTypeDef hadc1;
extern DAC_HandleTypeDef hdac;
extern UART_HandleTypeDef huart1;

typedef struct pin_
{
	GPIO_TypeDef * pPort;
	uint16_t pinNum;
} pin_t;

class digitalIn
{
	digitalIn()
	{
		_btnNum = _pinsActive;
		_pinsActive++;
		/* already initialized in main.c autogen code*/
	};

	~digitalIn()
	{
		_pinsActive--;
	};

	bool read()
	{
		return (bool)HAL_GPIO_ReadPin(InputPinLUT[_btnNum].pPort, InputPinLUT[_btnNum].pinNum);
	}

	uint8_t _btnNum;

	static const pin_t InputPinLUT[];
	static uint8_t _pinsActive;
};

class digitalOut
{
	digitalOut()
	{
		_btnNum = _pinsActive;
		_pinsActive++;
		/* already initialized in main.c autogen code*/
	};

	~digitalOut()
	{
		_pinsActive--;
	};

	void digitalWrite(bool state)
	{
		HAL_GPIO_WritePin(OutputPinLUT[_btnNum].pPort, OutputPinLUT[_btnNum].pinNum,
				(GPIO_PinState)state);
	}

	void toggle()
	{
		HAL_GPIO_TogglePin(OutputPinLUT[_btnNum].pPort, OutputPinLUT[_btnNum].pinNum);
	}

	uint8_t activeInTotal()
	{
		return _pinsActive;
	}

	uint8_t _btnNum;

	static const pin_t OutputPinLUT[];
	static uint8_t _pinsActive;
};

class ADC
{

};

class Serial
{


};

class DAC
{

};

////////////////////////////////////////////////////////////////////////////////
// Module I/O
////////////////////////////////////////////////////////////////////////////////

class Button {

	digitalIn dIn;

};


////////////////////////////////////////////////////////////////////////////////
// LED stuff
////////////////////////////////////////////////////////////////////////////////

typedef enum
{
	LED_POLARITY_ON_IS_LOW = false,
	LED_POLARITY_ON_IS_HIGH = true
} LED_polarity_e

class LED
{
	LED(LED_polarity_e polarity) : _polarity(polarity) {  };
	void turnOn() { dOut.digitalWrite(_polarity); };
	void turnOff() { dOut.digitalWrite(!_polarity); };
	void toggle(void) { dOut.toggle();};

	digitalOut dOut;
	LED_polarity_e _polarity;
};

class Blinker : public LED {

private:
	static std::vector<Blinker*> BlinkerVec;
	static void task(void * p);
	uint8_t 	_ledPin, _times;
	uint16_t 	_periodMs, _duty, _tick;

public:

	Blinker(uint8_t ledPin) : _ledPin(ledPin), _times(0), _periodMs(0),
		_duty(50), _tick(0)
	{
		BlinkerVec.push_back(this);
		if(BlinkerVec.size() < 2) {
			xTaskCreate(task, "blinker_tsk", 1024, NULL, configMAX_PRIORITIES - 3, NULL);
		}
	};

	~Blinker() {
		for(auto it = BlinkerVec.begin(); it != BlinkerVec.end(); ++it) {
			if(*it == this)
			{
				BlinkerVec.erase(it);
				gpio_set_level((gpio_num_t)_ledPin, 0);
				return;
			}
		}
	}

	int8_t Blinker::blink(uint16_t periodMs, uint8_t times, uint8_t duty) {
		if(periodMs && times)
		{
			_periodMs = periodMs;
			_times = times;
			_tick = 0;
			if(!duty)
			{
				_duty = (times == 1) ? 100 : 50;
			}
			else
			{
				_duty = duty;
			}
			gpio_set_direction((gpio_num_t)_ledPin, GPIO_MODE_OUTPUT);
			return 0;
		}
		return -1;
	};
};

////////////////////////////////////////////////////////////////////////////////
// Inputs
////////////////////////////////////////////////////////////////////////////////

class Potentiometer {


};

class Slider {


}

class Switch {


};

class SigIn : public ADC {



};

class SigOut : public DAC {



};

////////////////////////////////////////////////////////////////////////////////
// Module
////////////////////////////////////////////////////////////////////////////////


class Module {


};

} // namespace FIB
