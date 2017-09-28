/*
 * TicToc.c
 *
 *      Author: Carl Christian Lexow - cc.lexow -at- gmail.com
 *
 *
 *  MIT License
 *
 *  Copyright (c) [2017] [Carl Christian Lexow]
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */


#include "TicToc.h"

#ifdef ESP8266
	#define //xSerial Serial
#else
#ifdef STM32_MCU_SERIES
	#define xSerial Serial1
#endif
#endif
TicToc::TicToc(void)
{
	u32GlobalTimeRef = 0;
	u32SubTimeRef = 0;
	/* not used */
	delete[] ptrTimeStamps;
	delete[] ptrTimeStampNames;
	u08MaxTimeStampCnt = 0;
	u08TimeStampWriteIdx = 0;
}

TicToc::TicToc(String strName)
{
	u32GlobalTimeRef = 0;
	u32SubTimeRef = 0;
	strTicTocName = strName;
	/* not used */
	delete[] ptrTimeStamps;
	delete[] ptrTimeStampNames;
	u08MaxTimeStampCnt = 0;
	u08TimeStampWriteIdx = 0;
}

TicToc::TicToc(String strName, uint8_t u08TicTocEventMaxCount)
{
	u32GlobalTimeRef = 0;
	u32SubTimeRef = 0;
	strTicTocName = strName;
	u08TimeStampWriteIdx = 0;
	/* create array for time stamps and names */
	ptrTimeStamps = new uint32_t[u08TicTocEventMaxCount];
	ptrTimeStampNames = new String[u08TicTocEventMaxCount];
	/* check if memory was allocated */
	if((0!=ptrTimeStamps) && (0!=ptrTimeStampNames))
	{
		u08MaxTimeStampCnt = u08TicTocEventMaxCount;
	}
	else
	{
		u08MaxTimeStampCnt = 0;
	}
}

TicToc::~TicToc()
{
	delete[] ptrTimeStamps;
}

/* Sets the global time reference to the current time
 * returns the current time in microseconds */
uint32_t TicToc::u32GlobalTic(void)
{
	u32GlobalTimeRef = u32GetSystemTime();
	u32SubTimeRef = u32GlobalTimeRef;
	return u32GlobalTimeRef;
}

/* returns the delta in microseconds to the global time reference */
uint32_t TicToc::u32GlobalToc(void)
{
	uint32_t u32Help = u32GetSystemTime();
	u32Help -= u32GlobalTimeRef;
	return u32Help;
}

/* returns the delta in microseconds to the global time reference
 * and prints the time + given message to the serial port */
uint32_t TicToc::u32GlobalToc(String strMessage)
{
	uint32_t u32Help = u32GlobalToc();
	xSerial.print(strTicTocName);
	xSerial.print(" Toc: ");
	xSerial.print(u32Help);
	xSerial.print(" us - ");
	xSerial.println(strMessage);
	return u32Help;
}

/* returns the delta in microseconds to the last time reference
 * (either global or sub) */
uint32_t TicToc::u32SubToc(void)
{
	uint32_t u32TimeDelta;
	uint32_t u32CurrentTime = u32GetSystemTime();

	/* calculate time delta */
	u32TimeDelta = u32CurrentTime - u32SubTimeRef;
	/* set time reference */
	u32SubTimeRef = u32CurrentTime;
	return u32TimeDelta;
}
/* returns the delta in microseconds to the last time reference
 * (either global or sub)
 * and prints the time + given message to the serial port */
uint32_t TicToc::u32SubToc(String strMessage)
{
	uint32_t u32TimeDelta = u32SubToc();
	xSerial.print(strTicTocName);
	xSerial.print(" Toc: ");
	xSerial.print(u32TimeDelta);
	xSerial.print(" us - ");
	xSerial.println(strMessage);
	/* update sub time reference */
	u32SubTimeRef = u32GetSystemTime();
	return u32TimeDelta;
}


/* takes a time stamp relative to the global Tic
 * and stores it into an internal array */
void TicToc::SilentGlobalToc(void)
{
	/* get system time */
	uint32_t u32TimeDelta = u32GetSystemTime();
	/* and calculated delta */
	u32TimeDelta -= u32GlobalTimeRef;
	/* perform only, if space in array */
	if(u08TimeStampWriteIdx < u08MaxTimeStampCnt)
	{
		/* save into array */
		ptrTimeStamps[u08TimeStampWriteIdx] = u32TimeDelta;
		/* increment write idx */
		u08TimeStampWriteIdx++;
	}
}

/* takes a time stamp relative to the last time reference
 *  and stores it into an internal array */
void TicToc::SilentSubToc(void)
{
	/* get time delta */
	uint32_t u32TimeDelta = u32SubToc();
	/* perform only, if space in array */
	if(u08TimeStampWriteIdx < u08MaxTimeStampCnt)
	{
		/* save into array */
		ptrTimeStamps[u08TimeStampWriteIdx] = u32TimeDelta;
		/* increment write idx */
		u08TimeStampWriteIdx++;
	}
}

/* returns a string containing all stored toc time stamps */
String TicToc::strGetSilentTocEvents(void)
{
	String strRetVal = "Time stamps of <" + strTicTocName + ">:\r\n";
	/* convert all time stamps into a string */
	for(uint8_t u08Idx=0; u08Idx<u08TimeStampWriteIdx; u08Idx++)
	{
		if(ptrTimeStampNames[u08Idx])
		{
			strRetVal += String(u08Idx+1) + ": " + ptrTimeStampNames[u08Idx] + " - " + String(ptrTimeStamps[u08Idx]) + " us\r\n";
		}
		else
		{
			strRetVal += String(u08Idx+1) + ": " + String(ptrTimeStamps[u08Idx]) + " us\r\n";
		}
	}
	return strRetVal;
}

/* sets the string describing the given event */
bool TicToc::bSetSilentEventName(uint8_t u08EventIdx, String strEventName)
{
	bool bRetVal = false;
	/* ensure, that event idx is valid */
	if(u08EventIdx < u08MaxTimeStampCnt)
	{
		/* take over data */
		ptrTimeStampNames[u08EventIdx] = strEventName;
		bRetVal = true;
	}

	return bRetVal;
}

/* resets stored toc stamps */
void TicToc::ResetSilentTocEvents(void)
{
	/* delete array content and write idx */
	for(uint8_t u08Idx=0; u08Idx < u08MaxTimeStampCnt; u08Idx++)
	{
		ptrTimeStamps[u08Idx] = 0;
	}
	u08TimeStampWriteIdx = 0;
}

inline uint32_t TicToc::u32GetSystemTime(void)
{
	/* ToDo: implement functionality for the different targets (ESP / Arduino / etc.) */
#ifdef ESP8266
	return system_get_time();
#else
	return micros();
#endif
}
