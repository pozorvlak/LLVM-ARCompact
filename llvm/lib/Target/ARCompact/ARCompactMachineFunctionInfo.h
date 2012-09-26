//===--- ARCompactMachineFuctionInfo.h - ARCompact machine function info --===//
//
//                     The LLVM Compiler Infrastructure
//
//===----------------------------------------------------------------------===//
//
// This file declares ARCompact-specific per-machine-function information.
//
//===----------------------------------------------------------------------===//

#ifndef ARCCOMPACTMACHINEFUNCTIONINFO_H
#define ARCCOMPACTMACHINEFUNCTIONINFO_H

#include "ARCompactSubtarget.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/Target/TargetRegisterInfo.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/ADT/BitVector.h"

namespace llvm {

/// ARCompactFunctionInfo - This class is derived from MachineFunctionInfo and
/// contains private ARCompact-specific information for each MachineFunction.
class ARCompactFunctionInfo : public MachineFunctionInfo {

  /// VarArgsRegSaveSize - Size of the register save area for vararg functions.
  unsigned VarArgsRegSaveSize;

  /// VarArgsFrameIndex - FrameIndex for start of varargs area.
  int VarArgsFrameIndex;

public:
  ARCompactFunctionInfo()
      : VarArgsRegSaveSize(0),
        VarArgsFrameIndex(0) {
  }

  explicit ARCompactFunctionInfo(MachineFunction &MF)
      : VarArgsRegSaveSize(0),
        VarArgsFrameIndex(0) {
  }

  unsigned getVarArgsRegSaveSize() const { return VarArgsRegSaveSize; }
  void setVarArgsRegSaveSize(unsigned s) { VarArgsRegSaveSize = s; }

  int getVarArgsFrameIndex() const { return VarArgsFrameIndex; }
  void setVarArgsFrameIndex(int Index) { VarArgsFrameIndex = Index; }
};
} // End llvm namespace

#endif // ARCOMPACTMACHINEFUNCTIONINFO_H
