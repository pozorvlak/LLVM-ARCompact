//===------ ARCompactMCTargetDesc.h - ARCompact Target Descriptions -------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides ARCompact specific target descriptions.
//
//===----------------------------------------------------------------------===//

#ifndef ARCOMPACTMCTARGETDESC_H
#define ARCOMPACTMCTARGETDESC_H

namespace llvm {
class MCSubtargetInfo;
class Target;
class StringRef;

extern Target TheARCompactEncoreTarget;

} // End llvm namespace

// Defines symbolic names for ARCompact registers.  This defines a mapping from
// register name to register number.
#define GET_REGINFO_ENUM
#include "ARCompactGenRegisterInfo.inc"

// Defines symbolic names for the ARCompact instructions.
#define GET_INSTRINFO_ENUM
#include "ARCompactGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "ARCompactGenSubtargetInfo.inc"

#endif
