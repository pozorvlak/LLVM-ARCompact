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
  };
} // end namespace llvm

#endif    // ARCOMPACT_ISELLOWERING_H
