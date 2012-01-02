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

  /// Returns the RegisterInfo for the Target. 
  virtual const ARCompactRegisterInfo &getRegisterInfo() const {
    return RI;
  }
};

}

#endif
