/*
 * TicToc.h
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

#ifndef LIB_TICTOC_TICTOC_H_
#define LIB_TICTOC_TICTOC_H_
#ifdef ESP8266
extern "C"{
  #include "user_interface.h"
}
#endif
#include <Arduino.h>

class TicToc
{
public:

	TicToc(void);
	TicToc(String strName);
	TicToc(String strName, uint8_t u08TicTocEventMaxCount);
	~TicToc(void);

	/* Sets the global time reference to the current time
	 * returns the current time in microseconds */
	uint32_t u32GlobalTic(void);

	/* returns the delta in microseconds to the global time reference */
	uint32_t u32GlobalToc(void);

	/* returns the delta in microseconds to the global time reference
	 * and prints the time + given message to the serial port */
	uint32_t u32GlobalToc(String strMessage);

	/* returns the delta in microseconds to the last time reference
	 * (either global or sub) */
	uint32_t u32SubToc(void);

	/* returns the delta in microseconds to the last time reference
	 * (either global or sub)
	 * and prints the time + given message to the serial port */
	uint32_t u32SubToc(String strMessage);

	/* takes a time stamp relative to the global Tic
	 * and stores it into an internal array */
	void SilentGlobalToc(void);

	/* takes a time stamp relative to the last time reference
	 *  and stores it into an internal array */
	void SilentSubToc(void);

	/* returns a string containing all stored toc time stamps */
	String strGetSilentTocEvents(void);

	/* sets the string describing the given event */
	bool bSetSilentEventName(uint8_t u08EventIdx, String strEventName);

	/* resets stored toc stamps */
	void ResetSilentTocEvents(void);

private:
	inline uint32_t u32GetSystemTime(void);
	uint32_t u32GlobalTimeRef;
	uint32_t u32SubTimeRef;
	String strTicTocName;
	uint32_t * ptrTimeStamps;
	uint8_t u08MaxTimeStampCnt;
	uint8_t u08TimeStampWriteIdx;
	String * ptrTimeStampNames;

};



#endif /* LIB_TICTOC_TICTOC_H_ */
