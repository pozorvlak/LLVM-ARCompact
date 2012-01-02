//===----- ARCompactAsmPrinter.cpp - ARCompact LLVM assembly writer -------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains a printer that converts from our internal representation
// of machine-dependent LLVM code to ARCOMPACT assembly language.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "asm-printer"

#include "ARCompactAsmPrinter.h"

using namespace llvm;

// Pull in the printInstruction method, etc.
#include "ARCompactGenAsmWriter.inc"

// Static initialization.
extern "C" void LLVMInitializeARCompactAsmPrinter() { 
  RegisterAsmPrinter<ARCompactAsmPrinter> X(TheARCompactEncoreTarget);
}
