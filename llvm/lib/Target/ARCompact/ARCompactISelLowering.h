//===----- ARCompactISelLowering.h - ARCompact DAG Lowering Interface -----===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that ARCompact uses to lower LLVM code into
// a selection DAG.
//
//===----------------------------------------------------------------------===//

#ifndef ARCOMPACT_ISELLOWERING_H
#define ARCOMPACT_ISELLOWERING_H

#include "llvm/Target/TargetLowering.h"
#include "ARCompact.h"

namespace llvm {
  class ARCompactTargetLowering : public TargetLowering {
  public:
    ARCompactTargetLowering(TargetMachine &TM);

    /// LowerOperation - Provide custom lowering hooks for some operations.
    virtual SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const;

    /// This hook must be implemented to lower the incoming (formal) arguments,
    /// described by the Ins array, into the specified DAG. The implementation
    /// should fill in the InVals array with legal-type argument values, and
    /// return the resulting token chain value.
    virtual SDValue LowerFormalArguments(SDValue Chain, 
        CallingConv::ID CallConv, bool isVarArg,
        const SmallVectorImpl<ISD::InputArg> &Ins, DebugLoc dl, 
        SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const;
  };
} // end namespace llvm

#endif    // ARCOMPACT_ISELLOWERING_H
