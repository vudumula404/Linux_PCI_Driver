// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>


unsigned char fnGetFrequencyB2(unsigned char);
unsigned char fnGetFrequency(unsigned char);
unsigned char fnGetWordCountB2(unsigned char);
unsigned char fnGetWordCount(unsigned char);
unsigned char fnGetChannelSelB2(unsigned char);
unsigned char fnGetChannelSel(unsigned char);
unsigned char fnGetDestSubAddressB2(unsigned char);
unsigned char fnGetDestSubAddress(unsigned char);
unsigned char fnGetSourceSubAddressB2(unsigned char);
unsigned char fnGetSourceSubAddress(unsigned char);
unsigned char fnGetDestAddressB2(unsigned char);
unsigned char fnGetDestAddress(unsigned char);
unsigned char fnGetSourceAddressB2(unsigned char);
unsigned char fnGetSourceAddress(unsigned char);
unsigned char fnGetMinorFrame(unsigned char);
unsigned char fnGetMinorFrameB2(unsigned char);
void fnGetMessageNameB2(unsigned char , char *);
void fnGetMessageName(unsigned char, char *);
void fnAllocateMemoryForReceiveDataWordsB2(char *);
void fnAllocateMemoryForReceiveDataWords(char *);
void fnAllocateMemoryForTransmitDataWordsB2(char *);
void fnAllocateMemoryForTransmitDataWords(char *);
void fnDefineSyncMsg();
void fnDefineSyncMsgB2();

void fnExtractMessages(char *);
unsigned int fnHandle1553BCommunication();
void fnUpdateTimeBroadcastInfo(void);