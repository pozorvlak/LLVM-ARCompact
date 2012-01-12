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

void ARCompactInstrInfo::storeRegToStackSlot(MachineBasicBlock &MBB,
    MachineBasicBlock::iterator MI, unsigned SrcReg, bool isKill, int FrameIdx,
    const TargetRegisterClass *RC, const TargetRegisterInfo *TRI) const {
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

static bool IsComment(MachineInstr* I) {
  return  I->getNumOperands() == 1 && I->getOperand(0).isMetadata();
}

bool ARCompactInstrInfo::AnalyzeBranch(MachineBasicBlock &MBB,
    MachineBasicBlock *&TBB, MachineBasicBlock *&FBB,
    SmallVectorImpl<MachineOperand> &Cond, bool AllowModify) const {
  // Start from the bottom of the block and work up, examining the
  // terminator instructions.
  MachineBasicBlock::iterator I = MBB.end();
  while (I != MBB.begin()) {
    --I;

    if (I->isDebugValue() | IsComment(I)) {
      continue;
    }

    // Working from the bottom, when we see a non-terminator
    // instruction, we're done.
    // TODO: Delay slots?
    if (!isUnpredicatedTerminator(I)) {
      break;
    }

    // A terminator that isn't a branch can't easily be handled
    // by this analysis.
    if (!I->getDesc().isBranch()) {
      return true;
    }

    // Handle unconditional branches.
    if (I->getOpcode() == ARC::B) {
      // If we can modify the branch, there are multiple optimisations
      // we can apply.
      if (AllowModify) {
        // If the block has any instructions after the unconditional branch,
        // delete them.
        // TODO: Check for delay slot!
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

      // TBB is used to indicate the unconditional destination. If there is
      // a preceding conditional branch, this will be moved to FBB then.
      TBB = I->getOperand(0).getMBB();
      continue;
    }

    // Handle conditional branches.
    assert(I->getOpcode() == ARC::BCC && "Invalid branch");
    ARCCC::CondCodes BranchCode =
        static_cast<ARCCC::CondCodes>(I->getOperand(1).getImm());

    if (BranchCode == ARCCC::COND_INVALID) {
      return true;  // Can't handle weird stuff, so just fail.
    }

    // Cond is empty only for the first conditional branch.
    if (Cond.empty()) {
      // A successor unconditional branch may have set TBB, so move it to FBB.
      FBB = TBB;
      TBB = I->getOperand(0).getMBB();
      Cond.push_back(MachineOperand::CreateImm(BranchCode));
      continue;
    }

    // Handle subsequent conditional branches.
    assert(Cond.size() == 1);
    assert(TBB);

    // Only handle the case where all conditional branches branch to
    // the same destination.
    // TODO: Why?
    if (TBB != I->getOperand(0).getMBB()) {
      return true;
    }

    ARCCC::CondCodes OldBranchCode = (ARCCC::CondCodes) Cond[0].getImm();
    // If both branches have the same conditions, we can leave them alone.
    // TODO: Why? When does this happen?
    if (OldBranchCode == BranchCode) {
      continue;
    }

    // If we reach the end of this loop, we have failed to analyze a branching
    // instruction, so return true.
    return true;
  }

  return false; // Success!
}

unsigned ARCompactInstrInfo::InsertBranch(MachineBasicBlock &MBB,
    MachineBasicBlock *TBB, MachineBasicBlock *FBB,
    const SmallVectorImpl<MachineOperand> &Cond, DebugLoc DL) const {
  // There is no point inserting a branch for a fallthrough.
  assert(TBB && "InsertBranch must not be told to insert a fallthrough");

  if (Cond.empty()) {
    // Should be an unconditional branch.
    assert(!FBB && "Unconditional branch with multiple successors!");
    BuildMI(&MBB, DL, get(ARC::B)).addMBB(TBB);
    return 1;
  }

  // Conditional branch.
  unsigned Count = 0;
  BuildMI(&MBB, DL, get(ARC::BCC)).addMBB(TBB).addImm(Cond[0].getImm());
  ++Count;

  if (FBB) {
    // Two-way Conditional branch. Insert the second branch.
    BuildMI(&MBB, DL, get(ARC::B)).addMBB(FBB);
    ++Count;
  }
  return Count;
}

unsigned ARCompactInstrInfo::RemoveBranch(MachineBasicBlock &MBB) const {
  MachineBasicBlock::iterator I = MBB.end();
  unsigned Count = 0;

  while (I != MBB.begin()) {
    --I;

    // A non-branch instruction, so we are finished.
    // TODO: Test for debug/metadata?
    if (I->getOpcode() != ARC::B &&
        I->getOpcode() != ARC::BCC) {
      break;
    }

    // Remove the branch.
    I->eraseFromParent();
    I = MBB.end();
    ++Count;
  }

  return Count;
}
