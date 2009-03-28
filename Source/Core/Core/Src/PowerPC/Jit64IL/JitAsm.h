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

#ifndef _JITASM_H
#define _JITASM_H

#include "x64Emitter.h"

// In Dolphin, we don't use inline assembly. Instead, we generate all machine-near
// code at runtime. In the case of fixed code like this, after writing it, we write
// protect the memory, essentially making it work just like precompiled code.

// There are some advantages to this approach:
//   1) No need to setup an external assembler in the build.
//   2) Cross platform, as long as it's x86/x64.
//   3) Can optimize code at runtime for the specific CPU model.
// There aren't really any disadvantages other than having to maintain a x86 emitter,
// which we have to do anyway :)
// 
// To add a new asm routine, just add another const here, and add the code to Generate.
// Also, possibly increase the size of the code buffer.

class AsmRoutineManager : public Gen::XCodeBlock
{
private:
	void Generate();
	void GenerateCommon();
	void GenFifoWrite(int size);
	void GenFifoFloatWrite();
	void GenFifoXmm64Write();  // yes, 32 & 64-bit compatible
	void GenQuantizedLoads();
	void GenQuantizedStores();

public:
	void Init() {
		AllocCodeSpace(8192);
		Generate();
		WriteProtect();
	}

	void Shutdown() {
		FreeCodeSpace();
	}


	// Public generated functions. Just CALL(M((void*)func)) them.

	const u8 *enterCode;

	const u8 *dispatcher;
	const u8 *dispatcherNoCheck;
	const u8 *dispatcherPcInEAX;

	const u8 *fpException;
	const u8 *computeRc;
	const u8 *computeRcFp;
	const u8 *testExceptions;
	const u8 *dispatchPcInEAX;
	const u8 *doTiming;

	const u8 *fifoDirectWrite8;
	const u8 *fifoDirectWrite16;
	const u8 *fifoDirectWrite32;
	const u8 *fifoDirectWriteFloat;
	const u8 *fifoDirectWriteXmm64;

	const u8 *breakpointBailout;

	const u8 *doReJit;

	const u8 *pairedLoadQuantized[8];
	const u8 *pairedStoreQuantized[8];

	bool compareEnabled;
};

extern AsmRoutineManager asm_routines;

#endif
