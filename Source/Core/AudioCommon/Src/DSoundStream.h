// Copyright (C) 2003-2009 Dolphin Project.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// A copy of the GPL 2.0 should have been included with the program.
// If not, see http://www.gnu.org/licenses/

// Official SVN repository and contact information can be found at
// http://code.google.com/p/dolphin-emu/

#ifndef _DSOUNDSTREAM_H_
#define _DSOUNDSTREAM_H_

#include "SoundStream.h"
#include "Thread.h"

#ifdef _WIN32
#include <mmsystem.h>
#include <dsound.h>

#define BUFSIZE 32768
#define MAXWAIT 70   // miliseconds
#endif

class DSound : public SoundStream
{
    Common::Thread *thread;
    Common::CriticalSection soundCriticalSection;
    Common::Event soundSyncEvent;
    void  *hWnd;

    IDirectSound8* ds;
    IDirectSoundBuffer* dsBuffer;
    
    int bufferSize;     //i bytes
    int totalRenderedBytes;
    
    // playback position
    int currentPos;
    int lastPos;
    short realtimeBuffer[1024 * 1024];
    
    inline int FIX128(int x) {
		return x & (~127);
    }

    inline int ModBufferSize(int x) {
		return (x + bufferSize) % bufferSize;
    }

    bool CreateBuffer();
    bool WriteDataToBuffer(DWORD dwOffset, char* soundData,
			   DWORD dwSoundBytes);

public:
	DSound(CMixer *mixer, void *hWnd = NULL) : SoundStream(mixer),
											   bufferSize(0),
											   totalRenderedBytes(0),
											   currentPos(0),lastPos(0) {}

    virtual ~DSound() {}
 
	virtual bool Start();
    virtual void SoundLoop();
    virtual void Stop();
    static bool isValid() { return true; }
    virtual bool usesMixer() const { return true; }
    virtual void Update();
};

#endif //_DSOUNDSTREAM_H_
