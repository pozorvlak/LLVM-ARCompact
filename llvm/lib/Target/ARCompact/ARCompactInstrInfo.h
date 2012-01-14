//===------ ARCompactInstrInfo.h - ARCompact Instruction Information ------===//
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

#ifndef ARCOMPACTINSTRUCTIONINFO_H
#define ARCOMPACTINSTRUCTIONINFO_H

#include "llvm/Target/TargetInstrInfo.h"
#include "ARCompactRegisterInfo.h"

#define GET_INSTRINFO_HEADER
#include "ARCompactGenInstrInfo.inc"

namespace llvm {

class ARCompactInstrInfo : public ARCompactGenInstrInfo {
  const ARCompactRegisterInfo RI;
  const ARCompactSubtarget& Subtarget;
public:
  explicit ARCompactInstrInfo(ARCompactSubtarget &ST);

  /// Inserts instructions to copy a pair of physical registers, moving the
  /// contents of SrcReg to DestReg. If KillSrc is set, the source register is
  /// no longer needed.
  virtual void copyPhysReg(MachineBasicBlock &MBB,
                           MachineBasicBlock::iterator I, DebugLoc DL,
                           unsigned DestReg, unsigned SrcReg,
                           bool KillSrc) const;

  /// Store the specified register of the given register class to the specified
  /// stack frame index. The store instruction is to be added to the given
  /// machine basic block before the specified machine instruction. If isKill
  /// is true, the register operand is the last use and must be marked kill.
  virtual void storeRegToStackSlot(MachineBasicBlock &MBB,
      MachineBasicBlock::iterator MI,
      unsigned SrcReg, bool isKill, int FrameIndex,
      const TargetRegisterClass *RC,
      const TargetRegisterInfo *TRI) const;

  /// Load the specified register of the given register class from the
  /// specified stack frame index. The load instruction is to be added to the
  /// given machine basic block before the specified machine instruction.
  virtual void loadRegFromStackSlot(MachineBasicBlock &MBB,
      MachineBasicBlock::iterator MI,
      unsigned DestReg, int FrameIdx,
      const TargetRegisterClass *RC,
      const TargetRegisterInfo *TRI) const;

  /// Analyze the branching code at the end of MBB, returning true if it cannot
  /// be understood (e.g. it's a switch dispatch or isn't implemented for a
  /// target). Upon success, this returns false and returns with the following
  /// information in various cases:
  ///
  ///   1. If this block ends with no branches (it just falls through to its
  ///      succ) just return false, leaving TBB/FBB null.
  ///   2. If this block ends with only an unconditional branch, it sets TBB
  ///      to be the destination block.
  ///   3. If this block ends with a conditional branch and it falls through
  ///      to a successor block, it sets TBB to be the branch destination block
  ///      and a list of operands that evaluate the condition. These operands
  ///      can be passed to other TargetInstrInfo methods to create new
  ///      branches.
  ///   4. If this block ends with a conditional branch followed by an
  ///      unconditional branch, it returns the 'true' destination in TBB, the
  ///      'false' destination in FBB, and a list of operands that evaluate the
  ///      condition. These operands can be passed to other TargetInstrInfo
  ///      methods to create new branches.
  ///
  ///  If AllowModify is true, then this routine is allowed to modify the basic
  ///  block (e.g. by deleting instructions after the unconditional branch).
  bool AnalyzeBranch(MachineBasicBlock &MBB, MachineBasicBlock *&TBB,
      MachineBasicBlock *&FBB, SmallVectorImpl<MachineOperand> &Cond,
      bool AllowModify) const;

  /// Insert branch code into the end of the specified MachineBasicBlock.
  /// Returns the number of branch instructions inserted.
  unsigned InsertBranch(MachineBasicBlock &MBB, MachineBasicBlock *TBB,
      MachineBasicBlock *FBB, const SmallVectorImpl<MachineOperand> &Cond,
      DebugLoc DL) const;

  /// Remove the branching code at the end of the specific MachineBasicBlock.
  /// Returns the number of instructions that were removed.
  unsigned RemoveBranch(MachineBasicBlock &MBB) const;

  /// Reverses the branch condition of the specified condition list, returning
  /// false on success and true if it cannot be reversed.
  virtual bool ReverseBranchCondition(SmallVectorImpl<MachineOperand> &Cond)
      const;

  /// Converts the instruction into a predicated instruction. Returns true if
  /// the operation was successful.
  virtual bool PredicateInstruction (MachineInstr *MI,
      const SmallVectorImpl<MachineOperand> &Pred) const;

  /// Return true if the specified instruction can be predicated.
  virtual bool isPredicable(MachineInstr *MI) const;

  /// Returns true if the first specified predicate subsumes (contains) the
  /// second, e.g. GE subsumes GT.
  virtual bool SubsumesPredicate(const SmallVectorImpl<MachineOperand> &Pred1,
      const SmallVectorImpl<MachineOperand> &Pred2) const;

  /// If the specified instruction defines any predicate or condition code
  /// register(s) used for predication, returns true as well as a reference
  /// to the operand that defines the predicate.
  virtual bool DefinesPredicate(MachineInstr *MI, std::vector<MachineOperand> &Pred)
      const;

  /// Returns true if it's profitable to predicate a set of instructions with
  /// an accumulated instruction latency of "NumCycles" of the specified basic
  /// block, where the probability of the instructions being executed is given
  /// by Probability, and Confidence is a measure of our confidence that it
  /// will be properly predicted.
  virtual bool isProfitableToIfCvt(MachineBasicBlock &MBB, unsigned NumCycles,
      unsigned ExtraPredCycles, const BranchProbability &Probability) const;

  /// Second variant of isProfitableToIfCvt, this one checks for the case
  /// where two basic blocks from true and false path of a if-then-else
  /// (diamond) are predicated on mutally exclusive predicates, where the
  /// probability of the true path being taken is given by Probability, and
  /// Confidence is a measure of our confidence that it will be properly
  /// predicted.
  virtual bool isProfitableToIfCvt(MachineBasicBlock &TMBB, unsigned TCycles,
      unsigned TExtra, MachineBasicBlock &FMBB, unsigned FCycles,
      unsigned FExtra, const BranchProbability &Probability) const;

  /// Returns the RegisterInfo for the Target.
  virtual const ARCompactRegisterInfo &getRegisterInfo() const {
    return RI;
  }
};

}

#endif
