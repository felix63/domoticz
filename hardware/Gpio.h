/*
This code implements basic I/O hardware via the Raspberry Pi's GPIO port, using the wiringpi library.
WiringPi is Copyright (c) 2012-2013 Gordon Henderson. <projects@drogon.net>
It must be installed beforehand following instructions at http://wiringpi.com/download-and-install/

    Copyright (C) 2014 Vicne <vicnevicne@gmail.com>

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
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
    MA 02110-1301 USA.

This is a derivative work based on the samples included with wiringPi where distributed
under the GNU Lesser General Public License version 3
Source: http://wiringpi.com
*/
#pragma once

#include "DomoticzHardware.h"
#include "GpioPin.h"
#include "../main/RFXtrx.h"

class CGpio : public CDomoticzHardwareBase
{

public:
	explicit CGpio(const int ID, const int debounce, const int period, const int pollinterval);
	~CGpio();
	bool WriteToHardware(const char *pdata, const unsigned char length);
	static std::vector<CGpioPin> GetPinList();
	static CGpioPin* GetPPinById(int id);
	uint32_t m_period;
	uint32_t m_debounce;
	uint32_t m_pollinterval;
private:
	int GPIORead(int pin, const char* param);
	int GPIOReadFd(int fd);
	int GPIOWrite(int pin, bool value);
	int GetReadResult(int bytecount, char* value_str);
	int waitForInterrupt(int fd, const int mS);
	int SetSchedPriority(const int s, const int pri, const int x);
	bool InitPins();
	bool StartHardware();
	bool StopHardware();
	//bool CreateDomoticzDevices();
	void InterruptHandler();
	void Poller();
	void UpdateDeviceStates(bool forceUpdate);
	void UpdateStartup();
	void UpdateSwitch(const int gpioId, const bool value);
	void GetSchedPriority(int *scheduler, int *priority);

	boost::mutex m_pins_mutex;
	boost::shared_ptr<boost::thread> m_thread, m_thread_poller, m_thread_updatestartup;
	static std::vector<CGpioPin> pins;
	volatile bool m_stoprequested;
	volatile int pinPass;
	tRBUF IOPinStatusPacket;
};
