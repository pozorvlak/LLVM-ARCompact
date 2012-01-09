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

  // Complex Pattern Selectors.
  bool SelectADDRri(SDValue N, SDValue &Base, SDValue &Offset);
  bool SelectADDRli(SDValue N, SDValue &AddrOut);
  bool SelectADDRrr(SDValue N, SDValue &R1, SDValue &R2);
  bool SelectADDRrli(SDValue N, SDValue &R1, SDValue &Offset);
  bool SelectADDRlir(SDValue N, SDValue &R1, SDValue &Offset);

  virtual const char *getPassName() const {
    return "ARCOMPACT DAG->DAG Pattern Instruction Selection";
  }

  // Include the pieces autogenerated from the target description.
#include "ARCompactGenDAGISel.inc"

private:
  SDNode *Select(SDNode *N);
};
}  // end anonymous namespace

/// Sets Base to the value of the memory address operand, and Offset
/// to the value of it's offset, or returns false if unable to process
/// the memory address.
bool ARCompactDAGToDAGISel::SelectADDRri(SDValue Addr, SDValue &Base,
    SDValue &Offset) {
  // Select a frame index node, if there is one. The offset will be
  // computed later, in eliminateFrameIndex.
  if (FrameIndexSDNode *FIN = dyn_cast<FrameIndexSDNode>(Addr)) {
    Base = CurDAG->getTargetFrameIndex(FIN->getIndex(), MVT::i32);
    Offset = CurDAG->getTargetConstant(0, MVT::i32);
    return true;
  }

  // If the address is calculated from an addition, we may as well
  // absorb the addition into the address.
  if (Addr.getOpcode() == ISD::ADD) {
    // TODO: Is using a 32-bit limm actually better, or should we restrict
    // to only the 9-bit simm case?
    if (ConstantSDNode *CN = dyn_cast<ConstantSDNode>(Addr.getOperand(1))) {
      uint64_t CVal = CN->getZExtValue();
      // If shifting left,right 32 bits (64 - 32) doesnt change CVal, it's i32.
      if (((CVal << 32) >> 32) == CVal) {
        SDValue N0 = Addr.getOperand(0);
        if (FrameIndexSDNode *FIN = dyn_cast<FrameIndexSDNode>(N0))
          Base = CurDAG->getTargetFrameIndex(FIN->getIndex(), MVT::i32);
        else
          Base = N0;

        Offset = CurDAG->getTargetConstant(CVal, MVT::i32);
        return true;
      }
    }
  }

  Base = Addr;
  Offset = CurDAG->getTargetConstant(0, MVT::i32);
  return true;
}

/// Sets AddrOut to the value of the memory operands address, or
/// return false if unable to process the memory address (e.g. if
/// it is not a limm address).
bool ARCompactDAGToDAGISel::SelectADDRli(SDValue Addr, SDValue &AddrOut) {
  return false;
}


/// Sets R1 and R2 to the values of the memory address operands, or
/// return false if unable to process the memory address (e.g. if
/// it is not a register, register address).
bool ARCompactDAGToDAGISel::SelectADDRrr(SDValue Addr, SDValue &R1,
    SDValue &R2) {
  return false;
}

/// Sets R1 and Offset to the values of the memory address operands, or
/// return false if unable to process the memory address (e.g. if
/// it is not a register, long-immediate address).
bool ARCompactDAGToDAGISel::SelectADDRrli(SDValue Addr, SDValue &R1,
    SDValue &Offset) {
  return false;
}

/// Sets Offset and R1 to the values of the memory address operands, or
/// return false if unable to process the memory address (e.g. if
/// it is not a long-immediate, register address).
bool ARCompactDAGToDAGISel::SelectADDRlir(SDValue Addr, SDValue &Offset,
    SDValue &R1) {
  return false;
}

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
