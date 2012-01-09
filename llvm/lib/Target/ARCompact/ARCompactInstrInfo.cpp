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

bool ARCompactInstrInfo::AnalyzeBranch(MachineBasicBlock &MBB,
    MachineBasicBlock *&TBB, MachineBasicBlock *&FBB,
    SmallVectorImpl<MachineOperand> &Cond, bool AllowModify) const {
  // Only handle the unconditional case at the moment, to eliminate
  // wasteful code.

  // Start from the bottom of the block and work up, examining the
  // terminator instructions.
  MachineBasicBlock::iterator I = MBB.end();
  while (I != MBB.begin()) {
    --I;
    if (I->isDebugValue())
      continue;

    // A terminator that isn't a branch can't easily be handled
    // by this analysis.
    if (!I->getDesc().isBranch())
      return true;

    // Handle unconditional branches.
    if (I->getOpcode() == ARC::BRANCH) {
      // If we can modify the branch, there are multiple optimisations
      // we can apply.
      if (AllowModify) {
        // If the block has any instructions after the unconditional branch,
        // delete them.
        while (llvm::next(I) != MBB.end()) {
          llvm::next(I)->eraseFromParent();
        }

        // There should be no condition nor false block for an unconditional
        // branch.
        Cond.clear();
        FBB = 0;

        // Delete the branch if it's equivalent to a fall-through.
        if (MBB.isLayoutSuccessor(I->getOperand(0).getMBB())) {
          TBB = 0;
          I->eraseFromParent();
          I = MBB.end();
          continue;
        }
      }
      // TBB is used to indicate the unconditinal destination.
      TBB = I->getOperand(0).getMBB();
      continue;
    }

    // If we reach the end of this loop, we have failed to analyze a branching
    // instruction, so return true.
    return true;
  }

  return false; // Success!
}
