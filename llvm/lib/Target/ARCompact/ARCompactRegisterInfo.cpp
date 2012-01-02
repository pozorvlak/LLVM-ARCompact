//===----- ARCompactRegisterInfo.cpp - ARCOMPACT Register Information -----===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the ARCOMPACT implementation of the TargetRegisterInfo 
// class.
//
//===----------------------------------------------------------------------===//

#include "ARCompact.h"
#include "ARCompactRegisterInfo.h"
#include "ARCompactSubtarget.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Target/TargetInstrInfo.h"
#include "llvm/Type.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/STLExtras.h"

#define GET_REGINFO_TARGET_DESC
#include "ARCompactGenRegisterInfo.inc"

using namespace llvm;

ARCompactRegisterInfo::ARCompactRegisterInfo(ARCompactSubtarget &st,
                                     const TargetInstrInfo &tii)
  : ARCompactGenRegisterInfo(ARC::BLINK), Subtarget(st), TII(tii) {
}

const unsigned* ARCompactRegisterInfo::getCalleeSavedRegs(
    const MachineFunction *MF) const {
  // TODO: Check if ARCompact has callee saved regs.
  static const unsigned CalleeSavedRegs[] = { 0 };
  return CalleeSavedRegs;
}

BitVector ARCompactRegisterInfo::getReservedRegs(const MachineFunction &MF) 
    const {
  BitVector Reserved(getNumRegs());

  // TODO: If not needed for this MF, the frame pointer isnt actually reserved.

  // The global pointer, stack pointer, frame pointer, blink and interrupt
  // link registers are reserved.
  Reserved.set(ARC::GP);
  Reserved.set(ARC::SP);
  Reserved.set(ARC::FP);
  Reserved.set(ARC::BLINK);
  Reserved.set(ARC::ILINK1);
  Reserved.set(ARC::ILINK2);

  return Reserved;
}

void ARCompactRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
    int ARCAdj, RegScavenger *RS) const {
  llvm_unreachable("Not implemented yet!");
}

unsigned ARCompactRegisterInfo::getFrameRegister(const MachineFunction &MF) 
    const {
  return ARC::FP;
}
