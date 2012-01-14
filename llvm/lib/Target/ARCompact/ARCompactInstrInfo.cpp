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
#include "llvm/Support/BranchProbability.h"
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

  BuildMI(MBB, MI, DL, get(ARC::STrri))
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

bool ARCompactInstrInfo::ReverseBranchCondition(
    SmallVectorImpl<MachineOperand> &Cond) const {
  ARCCC::CondCodes CC = (ARCCC::CondCodes) (int) Cond[0].getImm();
  Cond[0].setImm(ARCCC::getOppositeCondition(CC));
  return false;
}

bool ARCompactInstrInfo::PredicateInstruction (MachineInstr *MI,
    const SmallVectorImpl<MachineOperand> &Pred) const {
  unsigned Opc = MI->getOpcode();

  int PIdx = MI->findFirstPredOperandIdx();
  if (PIdx != -1) {
    // Get the predicate operand, and replace it with the condition
    // code to check and the STATUS32 register.
    MachineOperand &PMO = MI->getOperand(PIdx);
    PMO.setImm(Pred[0].getImm());
    return true;
  }
  return false;
}

bool ARCompactInstrInfo::isPredicable(MachineInstr *MI) const {
  // If an instruction does not have a PredicateOperand, it definitely
  // cannot be predicated.
  if (!MI->getDesc().isPredicable()) {
    return false;
  }

  // Otherwise, either the first register must be 0 (*NOT YET MODELLED*)
  // or the dst and src1 registers must be the same.
  // TODO: Check to see if there are any other cases.
  if (MI->getOperand(0).getReg() != MI->getOperand(1).getReg()) {
    return false;
  }

  return true;
}

bool ARCompactInstrInfo::SubsumesPredicate(
    const SmallVectorImpl<MachineOperand> &Pred1,
    const SmallVectorImpl<MachineOperand> &Pred2) const {
  // Sanity check.
  // TODO: Why not just llvm_unreachable?
  if (Pred1.size() > 2 || Pred2.size() > 2) {
    return false;
  }

  ARCCC::CondCodes CC1 = (ARCCC::CondCodes) Pred1[0].getImm();
  ARCCC::CondCodes CC2 = (ARCCC::CondCodes) Pred2[0].getImm();

  // Identical opcodes trivially subsume one another.
  if (CC1 == CC2) {
  return true;
  }

  switch (CC1) {
    // AL contains all other predicates.
    case ARCCC::COND_AL:
      return true;
    // TODO: Does HS not include EQ?
    case ARCCC::COND_HS:
      return CC2 == ARCCC::COND_HI;
    case ARCCC::COND_LS:
      return CC2 == ARCCC::COND_LO || CC2 == ARCCC::COND_EQ;
    case ARCCC::COND_GE:
      return CC2 == ARCCC::COND_GT;
    case ARCCC::COND_LE:
      return CC2 == ARCCC::COND_LT;
    default:
      return false;
    }
}

bool ARCompactInstrInfo::DefinesPredicate(MachineInstr *MI,
    std::vector<MachineOperand> &Pred) const {
  // TODO: Does this pick up on CMP?
  const MCInstrDesc &MCID = MI->getDesc();
  if (!MCID.getImplicitDefs() && !MCID.hasOptionalDef()) {
    return false;
  }

  bool Found = false;
  for (unsigned i = 0, e = MI->getNumOperands(); i != e; ++i) {
    const MachineOperand &MO = MI->getOperand(i);
    if (MO.isReg() && MO.getReg() == ARC::STATUS32) {
      Pred.push_back(MO);
      Found = true;
      break;
    }
  }

  return Found;
}

bool ARCompactInstrInfo::isProfitableToIfCvt(MachineBasicBlock &MBB,
    unsigned NumCycles, unsigned ExtraPredCycles,
    const BranchProbability &Probability) const {
  MBB.dump();
  if (!NumCycles) {
    return false;
  }

  // Attempt to estimate the relative costs of predication versus branching.
  // Branching costs:
  //    (prob_branch * num_branch_cycles) + branch_cost + mispredict_penalty
  //
  // Predicating costs:
  //    num_branch_cycles + extra_predicate_cycles

  unsigned UnpredCost = Probability.getNumerator() * NumCycles;
  UnpredCost /= Probability.getDenominator();
  UnpredCost += 1; // The branch itself.
  // TODO: Do ARCompact misprediction penalty costs.
  //UnpredCost += Subtarget.getMispredictionPenalty() / 10;
  UnpredCost += 10 / 10;

  return (NumCycles + ExtraPredCycles) <= UnpredCost;
}

bool ARCompactInstrInfo::isProfitableToIfCvt(MachineBasicBlock &TMBB,
    unsigned TCycles, unsigned TExtra, MachineBasicBlock &FMBB,
    unsigned FCycles, unsigned FExtra, const BranchProbability &Probability)
    const {
  if (!TCycles || !FCycles) {
    return false;
  }

  // Attempt to estimate the relative costs of predication versus branching.
  // Branching costs:
  //    (prob_t_branch * num_t_branch_cycles) +
  //    (prob_f_branch * num_f_branch_cycles) +
  //    branch_cost +
  //    misdirect_penalty
  //
  // Predicating costs:
  //    num_t_branch_cycles +
  //    num_f_branch_cycles +
  //    extra_t_predicate_cycles +
  //    extra_f_predicate_cycles

  unsigned TUnpredCost = Probability.getNumerator() * TCycles;
  TUnpredCost /= Probability.getDenominator();

  // TODO: Check the math.
  uint32_t Comp = Probability.getDenominator() - Probability.getNumerator();
  unsigned FUnpredCost = Comp * FCycles;
  FUnpredCost /= Probability.getDenominator();

  unsigned UnpredCost = TUnpredCost + FUnpredCost;
  UnpredCost += 1; // The branch itself
  // TODO: Do ARCompact misprediction penalty costs.
  //UnpredCost += Subtarget.getMispredictionPenalty() / 10;
  UnpredCost += 10 / 10;

  return (TCycles + FCycles + TExtra + FExtra) <= UnpredCost;
}
