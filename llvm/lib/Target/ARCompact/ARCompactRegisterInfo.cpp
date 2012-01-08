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

void ARCompactRegisterInfo::eliminateCallFramePseudoInstr(MachineFunction &MF,
    MachineBasicBlock &MBB, MachineBasicBlock::iterator I) const {
  // Simply discard ADJCALLSTACKDOWN, ADJCALLSTACKUP instructions.
  MBB.erase(I);
}

void ARCompactRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
    int SPAdj, RegScavenger *RS) const {
  assert(SPAdj == 0 && "Unexpected non-zero adjustment!");

  // Find the frame index operand.
  unsigned i = 0;
  MachineInstr &MI = *II;
  while (!MI.getOperand(i).isFI()) {
    ++i;
    assert(i < MI.getNumOperands() && "Instr doesn't have FrameIndex operand!");
  }
  int FrameIndex = MI.getOperand(i).getIndex();

  // Addressable stack objects are accessed using negative offsets from the 
  // frame pointer.
  MachineFunction &MF = *MI.getParent()->getParent();
  int Offset = MF.getFrameInfo()->getObjectOffset(FrameIndex) +
    MI.getOperand(i + 1).getImm();

  // Replace frame index with a frame pointer reference.
  // TODO: Check what size of offset can fit in an immediate field. This will 
  // depend on the exact instruction being used.
  if (Offset >= -4096 && Offset <= 4095) {
    // If the offset is small enough to fit in the immediate field, directly
    // encode it.
    MI.getOperand(i).ChangeToRegister(ARC::FP, false);
    MI.getOperand(i+1).ChangeToImmediate(Offset);
  } else {
    // TODO: What is this? Left over from Sparc!
    MI.getOperand(i).ChangeToRegister(ARC::T1, false);
    MI.getOperand(i+1).ChangeToImmediate(Offset & ((1 << 10)-1));
  }
}

// Return-Address Register.
unsigned ARCompactRegisterInfo::getRARegister() const {
    return ARC::BLINK;
}

unsigned ARCompactRegisterInfo::getFrameRegister(const MachineFunction &MF) 
    const {
  return ARC::FP;
}
