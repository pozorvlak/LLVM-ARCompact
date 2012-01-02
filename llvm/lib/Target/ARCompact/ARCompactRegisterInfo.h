//===---- ARCompactRegisterInfo.h - ARCompact Register Information Impl ---===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the ARCompact implementation of the TargetRegisterInfo 
// class.
//
//===----------------------------------------------------------------------===//

#ifndef ARCOMPACTREGISTERINFO_H
#define ARCOMPACTREGISTERINFO_H

#include "llvm/Target/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "ARCompactGenRegisterInfo.inc"

namespace llvm {

class ARCompactSubtarget;
class TargetInstrInfo;
class Type;

struct ARCompactRegisterInfo : public ARCompactGenRegisterInfo {
  ARCompactSubtarget &Subtarget;
  const TargetInstrInfo &TII;

  ARCompactRegisterInfo(ARCompactSubtarget &st, const TargetInstrInfo &tii);

  /// Code Generation virtual methods...
  const unsigned *getCalleeSavedRegs(const MachineFunction *MF = 0) const;

  BitVector getReservedRegs(const MachineFunction &MF) const;

  void eliminateFrameIndex(MachineBasicBlock::iterator II,
                           int ARCAdj, RegScavenger *RS = NULL) const;

  void processFunctionBeforeFrameFinalized(MachineFunction &MF) const;

  // Debug information queries.
  unsigned getFrameRegister(const MachineFunction &MF) const;
};

} // end namespace llvm

#endif
