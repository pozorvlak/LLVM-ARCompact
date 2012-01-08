//===----- ARCompactInstrInfo.cpp - ARCompact Instruction Information -----===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the ARCompact implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#include "ARCompactInstrInfo.h"
#include "ARCompact.h"
#include "ARCompactSubtarget.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SmallVector.h"

#define GET_INSTRINFO_CTOR
#include "ARCompactGenInstrInfo.inc"

#include <iostream>

using namespace llvm;

ARCompactInstrInfo::ARCompactInstrInfo(ARCompactSubtarget &ST)
  : ARCompactGenInstrInfo(ARC::ADJCALLSTACKDOWN, ARC::ADJCALLSTACKUP),
    RI(ST, *this), Subtarget(ST) {
}

void ARCompactInstrInfo::copyPhysReg(MachineBasicBlock &MBB, 
    MachineBasicBlock::iterator I, DebugLoc DL, unsigned int DestReg,
    unsigned int SrcReg, bool KillSrc) const {
  BuildMI(MBB, I, DL, get(ARC::MOVrr), DestReg)
      .addReg(SrcReg, getKillRegState(KillSrc));
}

#include <iostream>
void ARCompactInstrInfo::storeRegToStackSlot(MachineBasicBlock &MBB,
    MachineBasicBlock::iterator MI, unsigned SrcReg, bool isKill, int FrameIdx,
    const TargetRegisterClass *RC, const TargetRegisterInfo *TRI) const {
  std::cerr << "LOLOLOLLOLOLOL" << std::endl;
  DebugLoc DL;
  if (MI != MBB.end()) DL = MI->getDebugLoc();

  BuildMI(MBB, MI, DL, get(ARC::STrli))
      .addFrameIndex(FrameIdx).addImm(0)
      .addReg(SrcReg, getKillRegState(isKill));
}

void ARCompactInstrInfo::loadRegFromStackSlot(MachineBasicBlock &MBB,
    MachineBasicBlock::iterator MI, unsigned DestReg, int FrameIdx,
    const TargetRegisterClass *RC, const TargetRegisterInfo *TRI) const {
  DebugLoc DL;
  if (MI != MBB.end()) DL = MI->getDebugLoc();

  BuildMI(MBB, MI, DL, get(ARC::LDrli))
      .addReg(DestReg)
      .addFrameIndex(FrameIdx).addImm(0);
}
