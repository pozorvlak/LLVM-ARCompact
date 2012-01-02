//==- ARCompactISelDAGToDAG.cpp - A dag to dag inst selector for ARCompact -==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines an instruction selector for the ARCOMPACT target.
//
//===----------------------------------------------------------------------===//

#include "ARCompactTargetMachine.h"
#include "llvm/Intrinsics.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

//===----------------------------------------------------------------------===//
// Instruction Selector Implementation
//===----------------------------------------------------------------------===//

/// ARCompactDAGToDAGISel - ARCOMPACT specific code to select ARCOMPACT machine
/// instructions for SelectionDAG operations.
namespace {
class ARCompactDAGToDAGISel : public SelectionDAGISel {
  const ARCompactSubtarget &Subtarget;
  ARCompactTargetMachine& TM;
public:
  explicit ARCompactDAGToDAGISel(ARCompactTargetMachine &tm)
    : SelectionDAGISel(tm),
      Subtarget(tm.getSubtarget<ARCompactSubtarget>()),
      TM(tm) {
  }

  virtual const char *getPassName() const {
    return "ARCOMPACT DAG->DAG Pattern Instruction Selection";
  }

  // Include the pieces autogenerated from the target description.
#include "ARCompactGenDAGISel.inc"

private:
  SDNode *Select(SDNode *N);
};
}  // end anonymous namespace


SDNode *ARCompactDAGToDAGISel::Select(SDNode *Op) {
  DebugLoc dl = Op->getDebugLoc();

  if (Op->isMachineOpcode()) {
    // This is an ARCompact instruction - it has already been selected.
    return NULL;
  }

  // Check if we need to do anything extra for the node.
  switch (Op->getOpcode()) {
    default: 
      // Do nothing - let SelectCode handle it.
      break;
  }

  // SelectCode is generated by tablegen.
  return SelectCode(Op);
}

/// Converts a legalized DAG into an  ARCOMPACT-specific DAG, ready for 
/// instruction scheduling.
FunctionPass *llvm::createARCompactISelDag(ARCompactTargetMachine &TM) {
  return new ARCompactDAGToDAGISel(TM);
}
