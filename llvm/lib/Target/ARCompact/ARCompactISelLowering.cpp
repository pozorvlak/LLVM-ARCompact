//===-- ARCompactISelLowering.cpp - ARCompact DAG Lowering Implementation -===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the interfaces that ARCompact uses to lower LLVM code 
// into a selection DAG.
//
//===----------------------------------------------------------------------===//

#include "ARCompactISelLowering.h"
#include "ARCompactTargetMachine.h"
#include "llvm/DerivedTypes.h"
#include "llvm/Function.h"
#include "llvm/Module.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/ADT/VectorExtras.h"
#include "llvm/Support/ErrorHandling.h"
using namespace llvm;

#include "ARCompactGenCallingConv.inc"

ARCompactTargetLowering::ARCompactTargetLowering(TargetMachine &TM)
    : TargetLowering(TM, new TargetLoweringObjectFileELF()) {
  // Set up the register classes.
  addRegisterClass(MVT::i32, ARC::CPURegsRegisterClass);

  // Compute the derived properties from the register classes.
  computeRegisterProperties();
}

SDValue ARCompactTargetLowering::LowerOperation(SDValue Op, SelectionDAG &DAG) 
    const {
  switch (Op.getOpcode()) {
    default:
      assert(0 && "Unimplemented operand!");
      return SDValue();
  }
}
